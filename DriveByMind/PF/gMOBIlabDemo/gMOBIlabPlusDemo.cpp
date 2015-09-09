/* gMOBIlabPlusDemo.cpp : Defines the entry point for the console application.
 *
 * This application reads data from a g.MOBIlab+ device and writes received data to a binary output file.
 * This binary output file can be read by using MATLAB for example. The file consists of consecutive float values (4 bytes each) that are the read values in microvolts from the devices.
 * One complete scan consists of the consecutive float values for all selected channels, exactly one value per channel. The file contains a number of those scans, one complete scan following the other.
 * Channel 1 of each scan represents the first selected channel of the device in the slaveSerials array.
 * 
 * Copyright (c) 2014 by g.tec medical engineering GmbH
 */

#include "stdafx.h"
#include <afxwin.h>
#include <afxmt.h>
#include "ringbuffer.h"
#include <iostream>
#include <fstream>
#include <string>
#include <WinSock2.h>
#include <SDL_thread.h>

using namespace std;

#include "gMOBIlabplus.h"

#ifdef _WIN64
#pragma comment(lib, "gMOBIlabplus_x64.lib")
#else
#pragma comment(lib, "gMOBIlabplus.lib")
#endif
#pragma comment(lib, "Ws2_32.lib")

//configuration
//const long NUM_SECONDS_RUNNING = 2;		//the number of seconds that the application should acquire data (after this time elapsed the application will be stopped)
float BUFFER_SIZE_SECONDS = 1;	//the size of the application buffer in seconds
unsigned int NUMBER_OF_CHANNELS = 8;	//the number of channels that should be acquired (must be less than or equal 8)
unsigned int NUMBER_OF_SCANS = 8;		//the number of scans that should be received simultaneously per GT_GetData call


//variaveis particulares
static bool running = false;
float deltaSendTime;
unsigned int bufferCapacity;

WSADATA WinSockData;
WORD dllVersion;
SOCKET sock;
SOCKADDR_IN address;
char * szLocalIP;
//--------------------------


LPSTR _serialPort = "COM3";					//the name of the serial port where your g.MOBIlab+ is connected to
BOOL _recordAllDigitalChannels = FALSE;		//TRUE to record all digital channels configured as input; FALSE to record no digital channel
BOOL _enableTestmode = TRUE;				//TRUE to set the device to testmode and receive a test signal instead of the real measured signal
BOOL _streamToSDcard = FALSE;				//TRUE to additionally stream the measured data to an inserted SD card in the device
LPSTR _filenameOnSDcard = "GMOBILAB";		//the filename of the file on the SD card where data will be recorded to when streaming to SD card is enabled

//global variables
_AIN _analogCh;
_DIO _digitalCh;
__CFG _devConfig;

//function prototypes
HANDLE OpenAndInitDevice(LPSTR serialPort) throw(string);
void StartAcquisition();
void StopAcquisition();
unsigned int DoAcquisition(LPVOID pArgs);
void CreateOutputFile(CFile *outputFile, LPCTSTR outputFilename) throw(string);
bool ReadData(short* destBuffer, int numberOfScans, int *errorCode, string *errorMessage);
static int StartReceiveData(void *ptr);
bool InitSocket();
static int SendDataOverTime(void *ptr);
void OnExit();
double **bufferToBiArray();

//global variables
HANDLE _hDevice;
CWinThread* _dataAcquisitionThread;		//the thread that performs data acquisition
bool _isRunning;						//flag that indicates if the thread is currently running
CMutex _bufferLock;						//mutex used to manage concurrent thread access to the application buffer
CRingBuffer<short> _buffer;				//the application buffer where received data will be stored for each device
CEvent _newDataAvailable;				//event to avoid polling the application data buffer for new data
CEvent _dataAcquisitionStopped;			//event that signals that data acquisition thread has been stopped
bool _bufferOverrun;	//flag indicating if an overrun occured at the application buffer
UINT receivedDataLength;
short *receivedData;




void main(int argc, _TCHAR* argv[])
{
	std::atexit(OnExit);
	cout << "Buffer Size in Seconds: ";
	cin >> BUFFER_SIZE_SECONDS;
	cout << "Number of Channels to Scan: ";
	cin >> NUMBER_OF_CHANNELS;
	cout << "Number of Scans: ";
	cin >> NUMBER_OF_SCANS;
	cout << "Time delay between Data sends: ";
	cin >> deltaSendTime;
	cout << endl;

	try
	{
		cout << "Opening and initializing device..." << "\n";

		//open and initialize devices and store device handle for further usage
		_hDevice = OpenAndInitDevice(_serialPort);
		//initialize temporary data buffer where data from the application buffer will be copied out
		receivedDataLength = NUMBER_OF_SCANS * (NUMBER_OF_CHANNELS + _recordAllDigitalChannels);
		receivedData = new short[receivedDataLength];
	}
	catch (string& exception)
	{
		//in case an error occured during opening and initialization, report it and stop execution
		cout << exception << "\n\nPress enter to exit...";
		system("PAUSE");
		exit(1);
	}
	cout << endl;

	if(InitSocket()== false){
		cout << "Couldn't connect to " << szLocalIP << endl;
		cout << "Exiting now..." << endl;
		system("PAUSE");
		exit(1);
	}
	running = true;

	SDL_Thread *DataReadThread;
	int DataReadThreadReturnValue;

	SDL_Thread *SocketSendThread;
	int SocketSendThreadReturnValue;

	SDL_Thread *SocketReceiveThread;
	int SocketReceiveThreadReturnValue;

	DataReadThread = SDL_CreateThread(StartReceiveData, "StartReceiveData", (void *)NULL);
	SocketSendThread = SDL_CreateThread(SendDataOverTime, "SendDataOverTime", NULL);
	
	SDL_WaitThread(DataReadThread, &DataReadThreadReturnValue);
	SDL_WaitThread(SocketSendThread, &SocketSendThreadReturnValue);

	cout << "Success" << endl;
	system("PAUSE");
	

}

bool InitSocket(){
	long success;

	dllVersion = MAKEWORD(2, 1);
	success = WSAStartup(dllVersion, &WinSockData);

	std::string response, converter;
	char message[200];
	
	sock = socket(AF_INET, SOCK_STREAM, NULL);

	char szHostName[255];
	gethostname(szHostName, 255);
	struct hostent *host_entry;
	host_entry = gethostbyname(szHostName);
	szLocalIP = inet_ntoa(*(struct in_addr *)*host_entry->h_addr_list);
	
	address.sin_addr.s_addr = inet_addr(szLocalIP);
	address.sin_family = AF_INET;
	address.sin_port = htons(10100);
	
	std::cout << "Your current IP address is: " << szLocalIP << std::endl;
	std::cout << "Do you want to connect to the server? (S/N)" << std::endl;
	std::cin >> response;

	response[0] = tolower(response[0]);

	if (response == "n"){
		std::cout << "Ok, exiting program" << std::endl;
	}
	else if (response == "s"){
		if(connect(sock, (SOCKADDR*)&address, sizeof(address))!=0){
			return false;
		}
		
		converter = message;
		std::cout << "Message from server " << converter << std::endl;
	}
	else{
		std::cout << "innapropriate response, exiting now" << std::endl;
		return false;
	}
	return true;
}

static int StartReceiveData(void *ptr){
	int i = 0;	
	try
		{
			//for receiving error messages from ReadData
			int errorCode;
			string errorMessage;

			//initialize file stream where received data should be written to and write header information to this file
			//CFile outputFile;
			//CreateOutputFile(&outputFile, "receivedData.bin");

			try
			{
				cout << "Starting acquisition...";

				//start data acquisition thread
				StartAcquisition();

				cout << " started!\n";

				//to stop the application after a specified time, get start time
				//long startTime = clock();
				//long endTime = startTime + NUM_SECONDS_RUNNING * CLOCKS_PER_SEC;

				//this is the data processing thread; data received from the devices will be written out to a file here
				int j = 0;
				while (running)
				{
					//j++;
					//to release CPU resources wait until the acquisition thread tells that new data has been received and is available in the buffer.
					WaitForSingleObject(_newDataAvailable.m_hObject, 1000);

					//read data from the application buffer and stop application if buffer overrun
					if (!ReadData(receivedData, NUMBER_OF_SCANS, &errorCode, &errorMessage))
					{
						if (errorCode == 2)
							continue;
						else
							throw errorMessage;
					}
				}
				
			}
			catch (string& exception)
			{
				//an exception occured
				cout << "\t" << exception << "\n";

				//continue execution in every case to clean up allocated resources (since no finally-block exists)
			}

			// 
			//in every case, stop data acquisition and clean up allocated resources 
			//since no finally statement exists in c++ and we can't mix it with the C __finally statement, the clean-up code must follow the try-catch block.
			//

			cout << "Closing devices...\n";

			//stop data acquisition
			StopAcquisition();

			//close device
			GT_CloseDevice(_hDevice);
					
			//close output file
			//outputFile.Close();
		}
		catch (string& exception)
		{
			//an exception occured
			cout << "\t" << exception << "\n";

			//continue execution in every case to clean up allocated resources (since no finally-block exists)
		}

		//free allocated resources
		delete [] receivedData;

		cout << "Clean up complete. Bye bye!" << "\n";
		return i;
	}

static int SendDataOverTime(void *ptr){

	long startTime = clock();
	long endTime = startTime + deltaSendTime * CLOCKS_PER_SEC;
	/*string lel = "A";
	while(running){
		if(clock() >= endTime){
			cout << "Sent" << lel.c_str() << endl;
			if(lel == "A"){
				send(sock, "A", 1, NULL);
				lel = "B";
			} else {
				send(sock, "B", 1, NULL);
				lel = "A";
			}
			endTime = clock() + deltaSendTime*CLOCKS_PER_SEC;
		}
	}*/
	bool done = false;
	while(running && !done)
	if(clock() >= endTime){
		double ** test = bufferToBiArray();
		int capacityPerChannel = bufferCapacity / NUMBER_OF_CHANNELS;
		for(int i =0; i < NUMBER_OF_CHANNELS; i++){
			cout << endl;
			cout  << "Channel " << i+1 << endl;
			cout << "------------------------------------" << endl;
			for(int j = 0; j < capacityPerChannel; j++){
				cout << test[i][j] << " ";
				if(j % 8 == 0)
					cout << endl;
			}
			cout << endl;
		}
		done = true;
	}
	return 0;
}


//opens and initializes a device on the specified port and returns a handle to the device.
HANDLE OpenAndInitDevice(LPSTR serialPort) throw(string)
{
	//construct the correct port identifier from the given port name
	CString portIdentifier;
	portIdentifier.Format("\\\\.\\%s\0", serialPort);

	//try to open a g.MOBIlab+ device on the specified port
	HANDLE hDevice = GT_OpenDevice(portIdentifier.GetBuffer());
	portIdentifier.ReleaseBuffer();

	try
	{
		if (hDevice == NULL)
			throw string("Error on GT_OpenDevice: Couldn't open device on ").append(serialPort);

		//query device configuration
		if (!GT_GetConfig(hDevice, &_devConfig))
			throw string("Error on GT_GetConfig: Couldn't read configuration from device on ").append(serialPort);

		printf("\tSerial number: \t%s\n\tDevice version:\t%d\n", _devConfig.serial, _devConfig.version);

		//query device state to see if device is running
		__DEVICESTATE devState;
		if (!GT_GetDeviceStatus(hDevice, &devState))
		{
			UINT errorCode;
			GT_GetLastError(&errorCode);

			if (errorCode == 16)
			{
				//device is not streaming but function succeeded; continue
			}
			else
				throw string("Error on GT_GetDeviceStatus: Couldn't read state from device ").append(_devConfig.serial);
		}
		
		//query SD card state
		UINT sdSize;
		if (!GT_GetSDcardStatus(hDevice, &sdSize))
			throw string("Error on GT_GetSDcardStatus: Couldn't read state of SD card from device ").append(_devConfig.serial);

		if (sdSize == 0)
			printf("\tSD card: \tno SD card inserted\n");
		else
			printf("\tSD card: \t%dBytes free\n", sdSize);

		//initialize analog channels (record all analog channels)
		_analogCh.ain1 = NUMBER_OF_CHANNELS >= 1;
		_analogCh.ain2 = NUMBER_OF_CHANNELS >= 2;
		_analogCh.ain3 = NUMBER_OF_CHANNELS >= 3;
		_analogCh.ain4 = NUMBER_OF_CHANNELS >= 4;
		_analogCh.ain5 = NUMBER_OF_CHANNELS >= 5;
		_analogCh.ain6 = NUMBER_OF_CHANNELS >= 6;
		_analogCh.ain7 = NUMBER_OF_CHANNELS >= 7;
		_analogCh.ain8 = NUMBER_OF_CHANNELS >= 8;

		//initialize digital channels (record all channels, set direction to input)
		_digitalCh.dio1_enable = _recordAllDigitalChannels;
		_digitalCh.dio2_enable = _recordAllDigitalChannels;
		_digitalCh.dio3_enable = _recordAllDigitalChannels;
		_digitalCh.dio4_enable = _recordAllDigitalChannels;
		_digitalCh.dio5_enable = _recordAllDigitalChannels;
		_digitalCh.dio6_enable = _recordAllDigitalChannels;
		_digitalCh.dio7_enable = _recordAllDigitalChannels;
		_digitalCh.dio8_enable = _recordAllDigitalChannels;
		_digitalCh.dio4_direction = FALSE;
		_digitalCh.dio5_direction = FALSE;
		_digitalCh.dio6_direction = FALSE;
		_digitalCh.dio7_direction = FALSE;

		//set device to testmode, if specified
		if (!GT_SetTestmode(hDevice, _enableTestmode))
			throw string("Error on GT_SetTestmode: couldn't set specified mode for device ").append(_devConfig.serial);

		//enable or disable streaming to SD card
		if (!GT_EnableSDcard(hDevice, _streamToSDcard) && _streamToSDcard)
			throw string("Error on GT_EnableSDcard: couldn't set specified streaming option for device ").append(_devConfig.serial);

		/*if (_streamToSDcard)
		{
			int filenameLength = (int)string(_filenameOnSDcard).length();

			if (filenameLength > 9)
				throw string("Error: filename for file on SD card must be less than 9 characters");

			//set the filename for the file recorded to the SD card
			if (!GT_SetFilename(hDevice, _filenameOnSDcard, filenameLength))
				throw string("Error on GT_SetFilename: couldn't set filename for file on SD card for device ").append(_devConfig.serial);
		}*/

		//initialize channels
		if (!GT_InitChannels(hDevice, _analogCh, _digitalCh))
			throw string("Error: on GT_InitChannels: couldn't initialize analog and/or digital channels for device ").append(_devConfig.serial);

		printf("\tg.MOBIlab+ %s initialized!\n", _devConfig.serial);

		//return handle of the device
		return hDevice;
	}
	catch (string& exception)
	{
		//ensure that handle is closed...
		if (hDevice != NULL)
			GT_CloseDevice(hDevice);

		//...and rethrow exception to notify the caller of this method about the error
		throw exception;
	}
}

//starts the thread that does the data acquisition.
void StartAcquisition()
{
	_isRunning = true;
	_bufferOverrun = false;

	//give main process (the data processing thread) high priority
	HANDLE hProcess = GetCurrentProcess();
	SetPriorityClass(hProcess, HIGH_PRIORITY_CLASS);

	//initialize application data buffer to the specified number of seconds
	_buffer.Initialize(BUFFER_SIZE_SECONDS * 256 * (NUMBER_OF_CHANNELS + _recordAllDigitalChannels));
	bufferCapacity = _buffer.GetCapacity();
	//reset event
	_dataAcquisitionStopped.ResetEvent();

	//create data acquisition thread with high priority
	_dataAcquisitionThread = AfxBeginThread(DoAcquisition, NULL, THREAD_PRIORITY_TIME_CRITICAL,0, CREATE_SUSPENDED);
	_dataAcquisitionThread->ResumeThread();
}

//stops the data acquisition thread
void StopAcquisition()
{
	//tell thread to stop data acquisition
	_isRunning = false;

	//wait until the thread has stopped data acquisition
	DWORD ret = WaitForSingleObject(_dataAcquisitionStopped.m_hObject, 5000);

	//reset the main process (data processing thread) to normal priority
	HANDLE hProcess = GetCurrentProcess();
	SetPriorityClass(hProcess, NORMAL_PRIORITY_CLASS);
}

/*
 * Starts data acquisition and acquires data until StopAcquisition is called (i.e., the _isRunning flag is set to false)
 * Then the data acquisition will be stopped.
 */
unsigned int DoAcquisition(LPVOID pArgs)
{
	//initialize overlapped structure with the event used for data transfer between device and PC
	OVERLAPPED ov;
	ov.hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	ov.Offset = 0;
	ov.OffsetHigh = 0;

	//initialize receive buffer
	int receiveBufferSize = NUMBER_OF_SCANS * (NUMBER_OF_CHANNELS + _recordAllDigitalChannels);

	_BUFFER_ST receiveBuffer;
	receiveBuffer.validPoints = 0;
	receiveBuffer.size = receiveBufferSize * sizeof(short);
	receiveBuffer.pBuffer = new short[receiveBufferSize];
	
	__try
	{
		if (!GT_StartAcquisition(_hDevice))
		{
			//throw string("Error on GT_Start: Couldn't start data acquisition of device.");
			cout << "\tError on GT_Start: Couldn't start data acquisition of device.\n";
			return 0;
		}

		while (_isRunning)
		{
			//tell device to send data
			GT_GetData(_hDevice, &receiveBuffer, &ov);

			//wait for notification from the system telling that new data is available
			if (WaitForSingleObject(ov.hEvent, 1000) == WAIT_TIMEOUT)
			{
				cout << "Error on data transfer: timeout occured\n";
				return 0;
			}

			//lock the application buffer to write data to it
			_bufferLock.Lock();

			__try
			{
				//if we are going to overrun on writing the received data into the buffer, set the appropriate flag; the reading thread will handle the overrun
				_bufferOverrun = (_buffer.GetFreeSize() < receiveBufferSize);

				//store received data
				_buffer.Write(receiveBuffer.pBuffer, receiveBufferSize);
			}
			__finally
			{
				//release the previously acquired lock
				_bufferLock.Unlock();
			}

			//signal processing (main) thread that new data is available
			_newDataAvailable.SetEvent();
		}
	}
	__finally
	{
		cout << "Stopping devices and cleaning up..." << "\n";

		//stop device
		GT_StopAcquisition(_hDevice);

		//clean up allocated resources
		CloseHandle(ov.hEvent);

		//clean up allocated resources
		delete [] receiveBuffer.pBuffer;

		//reset _isRunning flag
		_isRunning = false;

		//signal event
		_dataAcquisitionStopped.SetEvent();

		//end thread
		AfxEndThread(0xdead);
	}

	return 0xdead;
}

bool ReadData(short* destBuffer, int numberOfScans, int *errorCode, string *errorMessage)
{
	int validPoints = (NUMBER_OF_CHANNELS + _recordAllDigitalChannels) * numberOfScans;
	
	//wait until requested amount of data is ready
	if (_buffer.GetSize() < validPoints)
	{
		*errorCode = 2;
		*errorMessage = "Not enough data available";
		return false;
	}

	//acquire lock on the application buffer for reading
	_bufferLock.Lock();

	__try
	{
		//if buffer run over report error and reset buffer
		if (_bufferOverrun)
		{
			_buffer.Reset();
			*errorCode = 1;
			*errorMessage = "Error on reading data from the application data buffer: buffer overrun.";
			_bufferOverrun = false;
			return false;
		}

		//copy the data from the application buffer into the destination buffer
		_buffer.Read(destBuffer, validPoints);
	}
	__finally
	{
		_bufferLock.Unlock();
	}

	*errorCode = 0;
	*errorMessage = "No error occured.";
	return true;
}

double **bufferToBiArray(){
	cout << "chamado  bufferToBiArray";
	
	int bufferSizePerChannel = _buffer.GetCapacity()/NUMBER_OF_CHANNELS;
	//double **output = (double **) malloc(BUFFER_SIZE_SECONDS * 256 * (NUMBER_OF_CHANNELS + _recordAllDigitalChannels));
	double **output = (double**) malloc(NUMBER_OF_CHANNELS * sizeof(double*));
	for(int i = 0; i < NUMBER_OF_CHANNELS; i++){
		output[i] = (double *) malloc(bufferSizePerChannel * sizeof(double));
	}

	for(int i = 0; i< NUMBER_OF_CHANNELS; i++){
		for(int j = 0; j < bufferSizePerChannel; j++){
			output[i][j] = _buffer.getElementAt(j*NUMBER_OF_CHANNELS + i);
		}
	}

	return output;
}

void OnExit(){
	running = false;
		StopAcquisition();
			//close device
			GT_CloseDevice(_hDevice);
	closesocket(sock);
}

//opens and overwrites or creates a new binary output file with the specified filename and writes the header.
/*void CreateOutputFile(CFile *outputFile, LPCTSTR outputFilename) throw(string)
{
	//open/create output file on disk
	if (!outputFile->Open(outputFilename, CFile::modeCreate | CFile::modeWrite | CFile::typeBinary))
		throw string("Error on creating/opening output file: file 'receivedData.bin' couldn't be opened.");

	//write header
	CString header = _T("gtec\r\ng.MOBIlab+\r\n3.0\r\n256\r\n");

	CString analogChannelIDs;
	analogChannelIDs.Format("%i%i%i%i%i%i%i%i%i%i%i%i%i%i%i%i%i%i%i%i%i%i%i%i\r\n%i\r\n%i\r\n%d\r\n%s\r\n", 
		_analogCh.ain8, _analogCh.ain7, _analogCh.ain6, _analogCh.ain5, _analogCh.ain4, _analogCh.ain3, _analogCh.ain2, _analogCh.ain1, 
		_digitalCh.dio8_enable, _digitalCh.dio7_enable, _digitalCh.dio6_enable, _digitalCh.dio5_enable, _digitalCh.dio4_enable, _digitalCh.dio3_enable, _digitalCh.dio2_enable, _digitalCh.dio1_enable, 
		1, _digitalCh.dio7_direction, _digitalCh.dio6_direction, _digitalCh.dio5_direction, _digitalCh.dio4_direction, 1, 1, 1, 
		3, 2, _devConfig.version, _devConfig.serial);

	header += analogChannelIDs;

	for (int i = 0; i < 8; i++)
	{
		CString channelConfig;
		channelConfig.Format("%g/%g/%g/%g/%c\r\n", _devConfig.channels[i].highpass, _devConfig.channels[i].lowpass, _devConfig.channels[i].sensitivity, _devConfig.channels[i].samplerate, _devConfig.channels[i].polarity);
		header += channelConfig;
	}

	header += _T("EOH\r\n");

	outputFile->Write(header, header.GetLength());
}*/

/*
 * Reads the received numberOfScans scans from all devices. If not enough data is available (errorCode == 2) or the application buffer overruns (errorCode == 1), this method returns false.
 * float* destBuffer:	the array that returns the received data from the application data buffer. 
						Data is aligned as follows: element at position destBuffer[scanIndex * (numberOfChannelsPerDevice * numDevices) + channelIndex] is sample of channel channelIndex (zero-based) of the scan with zero-based scanIndex.
						channelIndex ranges from 0..numDevices*numChannelsPerDevices where numDevices equals the number of recorded devices and numChannelsPerDevice the number of channels from each of those devices.
						It is assumed that all devices provide the same number of channels.
 * int numberOfScans:	the number of scans to retrieve from the application buffer.
 */
