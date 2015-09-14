/*
 * gp_RPSSVEP_sil.c
 *
 * Automatically generated host I/O interface for:
 * Component: gp_RPSSVEP
 * Execution Mode: SIL
 *
 */

#include <stdio.h>
#include <string.h>
#include "mex.h"
#include "coder/connectivity_core/xilutils/xilutils.h"
#include "rtiostream_utils.h"
#include "coder/connectivity/xilcomms_rtiostream/xilcomms_rtiostream.h"
#include "coder/connectivity/xilservice/host/CInterface.h"
#include "_coder_gp_RPSSVEP_api.h"
#include "_coder_gp_RPSSVEP_info.h"

/* Response case labels */
enum ResponseIDs {
  RESPONSE_ERROR = 0,
  RESPONSE_OUTPUT_DATA = 1,
  RESPONSE_PRINTF = 2,
  RESPONSE_FOPEN = 3,
  RESPONSE_FPRINTF = 4
};

typedef struct {
  FILE ** Fd;
  mwSize size;
  int32_T fidOffset;
} targetIOFd_T;

static targetIOFd_T *targetIOFdPtr = NULL;
static XIL_RtIOStreamData_T* rtIOStreamDataPtr = NULL;
static SIL_DEBUGGING_DATA_T* silDebuggingDataPtr = NULL;
static XIL_IOBuffer_T* IOBufferPtr = NULL;
static int *pCommErrorOccurred = 0;
static void* pXILService = NULL;
static void* pComms = NULL;
static void* pXILUtils = NULL;
static void* pMemUnitTransformer = NULL;
static boolean_T firstTime= 0;

/* implements calls into MATLAB */
static int callMATLAB(
                      int nlhs,
                      mxArray * plhs[],
                      int nrhs,
                      mxArray * prhs[],
                      const char * functionName,
                      int withTrap)
{
  int errorOccurred = 0;
  if (withTrap) {
    mxArray * mException;
    mException = mexCallMATLABWithTrap(nlhs, plhs, nrhs, prhs, functionName);

    {
      int i;
      for (i=0; i<nrhs; i++) {
        mxDestroyArray(prhs[i]);
      }                                /* for */
    }

    if (mException != NULL) {
      mxArray * rhsDisplayMException[1];
      errorOccurred = 1;
      rhsDisplayMException[0] = mException;
      mException = mexCallMATLABWithTrap(0, NULL, 1, rhsDisplayMException,
        "rtw.pil.SILPILInterface.displayMException");
      mxDestroyArray(rhsDisplayMException[0]);
      if (mException != NULL) {
        mxDestroyArray(mException);
        mexErrMsgTxt(
                     "Error calling rtw.pil.SILPILInterface.displayMException on mException object.");
      } else {
        mexErrMsgTxt( "See the preceding MException report for more details.");
      }                                /* if */
    }                                  /* if */
  } else {
    errorOccurred = mexCallMATLAB(nlhs, plhs, nrhs, prhs, functionName);

    {
      int i;
      for (i=0; i<nrhs; i++) {
        mxDestroyArray(prhs[i]);
      }                                /* for */
    }

    if (errorOccurred) {
      mexErrMsgTxt( "mexCallMATLAB failed!");
    }                                  /* if */
  }                                    /* if */

  return errorOccurred;
}

static void xilDisplayException(void* pXILUtils)
{
  if (pXILUtils == NULL) {
    mexPrintf("pXILUtils is NULL!");
    return;
  }                                    /* if */

  {
    mxArray * pMException = xilMATLABUtilsGetException(pXILUtils);
    if (pMException == NULL) {
      mexPrintf("pMException is NULL!\n");
    } else {
      mxArray * prhs[1];
      prhs[0] = pMException;
      mexCallMATLAB(0, NULL, 1, prhs,
                    "rtw.pil.SILPILInterface.displayMException");
      mxDestroyArray(pMException);
    }                                  /* if */
  }
}

/* grow the buffer for target I/O Fd array
 * targetIOFd->Fd is NULL on failure */
static void growTargetIOFd(targetIOFd_T * IOFd, mwSize requiredSize)
{
  if (IOFd->size < requiredSize) {
    IOFd->Fd = mxRealloc(IOFd->Fd, requiredSize * sizeof(FILE*));
    if (IOFd->Fd == NULL) {
      mexErrMsgTxt( "growTargetIOFd: mxRealloc failed.");
    } else {
      mexMakeMemoryPersistent(IOFd->Fd);
      IOFd->size = requiredSize;
    }                                  /* if */
  }                                    /* if */
}

static void closeAndFreeTargetIOFd()
{
  int i;
  if (targetIOFdPtr != NULL) {
    if (targetIOFdPtr->Fd != NULL) {
      for (i=0; i<targetIOFdPtr->size; i++) {
        if (targetIOFdPtr->Fd[i] != NULL) {
          fclose(targetIOFdPtr->Fd[i]);
        }                              /* if */
      }                                /* for */

      mxFree(targetIOFdPtr->Fd);
    }                                  /* if */
  }                                    /* if */
}

/* receive one packet of data and dispatch to owning application */
static boolean_T recvData(void* pComms)
{
  if (pCommErrorOccurred == NULL) {
    mexErrMsgTxt( "pCommErrorOccurred is NULL.");
    return XILSERVICE_ERROR;
  }                                    /* if */

  if (pXILUtils == NULL) {
    mexErrMsgTxt( "pXILUtils is NULL.");
    return XILSERVICE_ERROR;
  }                                    /* if */

  *pCommErrorOccurred = (xilCommsRun(pComms, pXILUtils) !=
    XILCOMMS_RTIOSTREAM_SUCCESS);
  return (*pCommErrorOccurred?XILSERVICE_ERROR:XILSERVICE_SUCCESS);
}

/* send data via xil comms */
static boolean_T sendData(void* pXILService, XIL_IOBuffer_T* IOBuffer, mwSize
  sendSize)
{
  if (pCommErrorOccurred == NULL) {
    mexErrMsgTxt( "pCommErrorOccurred is NULL.");
    return XILSERVICE_ERROR;
  }                                    /* if */

  *pCommErrorOccurred = (xilServiceSend(pXILService, IOBuffer->data, sendSize)
    != XILSERVICE_SUCCESS);
  return (*pCommErrorOccurred?XILSERVICE_ERROR:XILSERVICE_SUCCESS);
}

/* implements command dispatch */
static boolean_T commandDispatch(XIL_IOBuffer_T* IOBuffer, mwSize dataOutSize)
{
  if (pXILService == NULL) {
    mexErrMsgTxt( "pXILService is NULL!");
    return XILSERVICE_ERROR;
  }                                    /* if */

  /* send the data */
  if (sendData(pXILService, IOBuffer, dataOutSize) != XILSERVICE_SUCCESS) {
    xilDisplayException(pXILUtils);
    return XILSERVICE_ERROR;
  }                                    /* if */

  return XILSERVICE_SUCCESS;
}

/* implements command response */
static boolean_T commandResponse(mwSize * dataInSize, XILCommandResponseType*
  commandType)
{
  if (pComms == NULL) {
    mexErrMsgTxt( "pComms is NULL!");
    return XILSERVICE_ERROR;
  }                                    /* if */

  if (pXILService == NULL) {
    mexErrMsgTxt( "pXILService is NULL!");
    return XILSERVICE_ERROR;
  }                                    /* if */

  {
    /* receive the response data */
    uint8_T COMMAND_COMPLETE = 0;
    while (!COMMAND_COMPLETE) {
      xilServiceSetIsResponseComplete(pXILService, 0);
      if (recvData(pComms) != XILSERVICE_SUCCESS) {
        xilDisplayException(pXILUtils);
        return XILSERVICE_ERROR;
      }                                /* if */

      COMMAND_COMPLETE = xilServiceGetIsResponseComplete(pXILService);
    }                                  /* while */

    /* determine command response type */
    *commandType = (XILCommandResponseType) COMMAND_COMPLETE;
    *dataInSize = xilServiceGetPayloadSizeForOneStep(pXILService);
    return XILSERVICE_SUCCESS;
  }
}

static void copyIOData(void * const dstPtr, void * const srcPtr, uint8_T **
  const tgtPtrPtr, size_t actualNumElements, size_t numElements, size_t
  cTypeSize)
{
  size_t maxBytesConsumed = numElements * cTypeSize;
  size_t actualBytesConsumed = actualNumElements * cTypeSize;
  memcpy(dstPtr, srcPtr, actualBytesConsumed);
  (*tgtPtrPtr)+=(maxBytesConsumed/sizeof(**tgtPtrPtr));
}

static boolean_T processResponseError( uint8_T ** mxMemUnitPtrPtr, const int
  withTrap)
{
  uint8_T errorId = **mxMemUnitPtrPtr;
  (*mxMemUnitPtrPtr)++;
  if (errorId) {
    {
      mxArray * prhs[ 2 ];
      prhs[0] = mxCreateString("PIL:pilverification:PILError");
      prhs[1] = mxCreateDoubleScalar(errorId);
      callMATLAB( 0, NULL, 2, prhs, "DAStudio.error", withTrap);
      return XILSERVICE_ERROR;
    }
  }                                    /* if */

  return XILSERVICE_SUCCESS;
}

static boolean_T processResponsePrintf( uint8_T ** mxMemUnitPtrPtr, const int
  withTrap)
{
  const int TARGET_IO_SUCCESS = 0;
  uint8_T PRINTF_ERROR;
  uint16_T PRINTF_SIZE;

  {
    uint8_T * simDataMemUnitPtr;
    simDataMemUnitPtr = (uint8_T *) &PRINTF_ERROR;

    {
      copyIOData( simDataMemUnitPtr, *mxMemUnitPtrPtr, &(*mxMemUnitPtrPtr), 1, 1,
                 sizeof(uint8_T));
    }
  }

  {
    uint8_T * simDataMemUnitPtr;
    simDataMemUnitPtr = (uint8_T *) &PRINTF_SIZE;

    {
      copyIOData( simDataMemUnitPtr, *mxMemUnitPtrPtr, &(*mxMemUnitPtrPtr), 1, 1,
                 sizeof(uint16_T));
    }
  }

  if (PRINTF_ERROR != TARGET_IO_SUCCESS) {
    {
      mxArray * prhs[ 2 ];
      prhs[0] = mxCreateString("PIL:pil:TargetIOError");
      prhs[1] = mxCreateDoubleScalar(PRINTF_ERROR);
      callMATLAB( 0, NULL, 2, prhs, "DAStudio.error", withTrap);
      return XILSERVICE_ERROR;
    }
  } else {
    uint8_T *pPrintBuff;
    pPrintBuff = *mxMemUnitPtrPtr;
    if (pPrintBuff[PRINTF_SIZE-1] == '\0') {
      mexPrintf("%s", pPrintBuff);
    }                                  /* if */
  }                                    /* if */

  (*mxMemUnitPtrPtr) = (*mxMemUnitPtrPtr) + PRINTF_SIZE;
  return XILSERVICE_SUCCESS;
}

static boolean_T processResponseFopen( uint8_T ** mxMemUnitPtrPtr, const int
  withTrap)
{
  uint16_T FOPEN_FID;
  uint16_T FOPEN_NAME_SIZE;

  {
    uint8_T * simDataMemUnitPtr;
    simDataMemUnitPtr = (uint8_T *) &FOPEN_FID;

    {
      copyIOData( simDataMemUnitPtr, *mxMemUnitPtrPtr, &(*mxMemUnitPtrPtr), 1, 1,
                 sizeof(uint16_T));
    }
  }

  {
    uint8_T * simDataMemUnitPtr;
    simDataMemUnitPtr = (uint8_T *) &FOPEN_NAME_SIZE;

    {
      copyIOData( simDataMemUnitPtr, *mxMemUnitPtrPtr, &(*mxMemUnitPtrPtr), 1, 1,
                 sizeof(uint16_T));
    }
  }

  if (targetIOFdPtr != NULL) {
    /* check fid increments by 1 */
    if (targetIOFdPtr->fidOffset + 1 == FOPEN_FID) {
      targetIOFdPtr->fidOffset = FOPEN_FID;
      growTargetIOFd(targetIOFdPtr, targetIOFdPtr->fidOffset + 1);
      if (targetIOFdPtr->Fd != NULL) {
        uint8_T *pFopenBuff;
        targetIOFdPtr->Fd[targetIOFdPtr->fidOffset] = NULL;
        pFopenBuff = (*mxMemUnitPtrPtr);
        if (pFopenBuff[FOPEN_NAME_SIZE-1] == '\0') {
          FILE * tmpFd = NULL;
          tmpFd = fopen((char *) pFopenBuff,"w");
          if (tmpFd != NULL) {
            /* save the file descriptor */
            targetIOFdPtr->Fd[targetIOFdPtr->fidOffset] = tmpFd;
          } else {
            {
              mxArray * prhs[ 2 ];
              prhs[0] = mxCreateString("PIL:pil:TargetIOFopenError");
              prhs[1] = mxCreateString((char *) pFopenBuff);
              callMATLAB( 0, NULL, 2, prhs, "DAStudio.error", withTrap);
              return XILSERVICE_ERROR;
            }
          }                            /* if */
        }                              /* if */
      }                                /* if */
    } else {
      {
        mxArray * prhs[ 2 ];
        prhs[0] = mxCreateString("PIL:pil:TargetIOFopenFidError");
        prhs[1] = mxCreateDoubleScalar(FOPEN_FID);
        callMATLAB( 0, NULL, 2, prhs, "DAStudio.error", withTrap);
        return XILSERVICE_ERROR;
      }
    }                                  /* if */
  }                                    /* if */

  (*mxMemUnitPtrPtr) = (*mxMemUnitPtrPtr) + FOPEN_NAME_SIZE;
  return XILSERVICE_SUCCESS;
}

static boolean_T processResponseFprintf( uint8_T ** mxMemUnitPtrPtr, const int
  withTrap)
{
  const int TARGET_IO_SUCCESS = 0;
  uint8_T FPRINTF_ERROR;
  uint16_T FPRINTF_FID;
  uint16_T FPRINTF_SIZE;

  {
    uint8_T * simDataMemUnitPtr;
    simDataMemUnitPtr = (uint8_T *) &FPRINTF_ERROR;

    {
      copyIOData( simDataMemUnitPtr, *mxMemUnitPtrPtr, &(*mxMemUnitPtrPtr), 1, 1,
                 sizeof(uint8_T));
    }
  }

  {
    uint8_T * simDataMemUnitPtr;
    simDataMemUnitPtr = (uint8_T *) &FPRINTF_FID;

    {
      copyIOData( simDataMemUnitPtr, *mxMemUnitPtrPtr, &(*mxMemUnitPtrPtr), 1, 1,
                 sizeof(uint16_T));
    }
  }

  {
    uint8_T * simDataMemUnitPtr;
    simDataMemUnitPtr = (uint8_T *) &FPRINTF_SIZE;

    {
      copyIOData( simDataMemUnitPtr, *mxMemUnitPtrPtr, &(*mxMemUnitPtrPtr), 1, 1,
                 sizeof(uint16_T));
    }
  }

  if (FPRINTF_ERROR != TARGET_IO_SUCCESS) {
    {
      mxArray * prhs[ 2 ];
      prhs[0] = mxCreateString("PIL:pil:TargetIOError");
      prhs[1] = mxCreateDoubleScalar(FPRINTF_ERROR);
      callMATLAB( 0, NULL, 2, prhs, "DAStudio.error", withTrap);
      return XILSERVICE_ERROR;
    }
  } else {
    if (targetIOFdPtr != NULL) {
      if (targetIOFdPtr->size > FPRINTF_FID) {
        if (targetIOFdPtr->Fd[FPRINTF_FID] != NULL) {
          uint8_T *pFprintfBuff;
          pFprintfBuff = (*mxMemUnitPtrPtr);
          if (pFprintfBuff[FPRINTF_SIZE-1] == '\0') {
            fprintf(targetIOFdPtr->Fd[FPRINTF_FID], "%s", pFprintfBuff);
          }                            /* if */
        }                              /* if */
      }                                /* if */
    }                                  /* if */
  }                                    /* if */

  (*mxMemUnitPtrPtr) = (*mxMemUnitPtrPtr) + FPRINTF_SIZE ;
  return XILSERVICE_SUCCESS;
}

static boolean_T processErrorAndTargetIOResponseCases( const int responseId,
  uint8_T ** mxMemUnitPtrPtr, const int withTrap)
{
  switch (responseId) {
   case RESPONSE_ERROR:
    {
      return processResponseError( mxMemUnitPtrPtr, withTrap);
    }

   case RESPONSE_PRINTF:
    {
      return processResponsePrintf( mxMemUnitPtrPtr, withTrap);
    }

   case RESPONSE_FOPEN:
    {
      return processResponseFopen( mxMemUnitPtrPtr, withTrap);
    }

   case RESPONSE_FPRINTF:
    {
      return processResponseFprintf( mxMemUnitPtrPtr, withTrap);
    }

   default:
    {
      {
        mxArray * prhs[ 2 ];
        prhs[0] = mxCreateString("PIL:pilverification:UnknownResponseId");
        prhs[1] = mxCreateDoubleScalar(responseId);
        callMATLAB( 0, NULL, 2, prhs, "DAStudio.error", withTrap);
        return XILSERVICE_ERROR;
      }
    }
  }                                    /* switch */
}

static void callStopHookAndFreeSFcnMemory()
{
  closeAndFreeTargetIOFd();

  {
    {
      if (pXILUtils) {
        mxArray *rhs[3];
        const char * simulinkBlockPath =
          "C:\\gpires\\Dropbox\\_gpires\\desenvolvimento\\DriveByMind_Simulink_SSVEP_IPT\\Online_Simulink_model_SSVEP_Unity\\algoritmo_C\\codegen\\lib\\gp_RPSSVEP\\gp_RPSSVEP";
        rhs[ 0 ] = mxCreateString(
          "@coder.connectivity.MATLABInterface.getSILPILInterface");
        rhs[ 1 ] = mxCreateDoubleScalar( 3 );
        rhs[ 2 ] = mxCreateString(simulinkBlockPath);
        if (xilUtilsCallMATLAB(pXILUtils, 0, NULL, 3, rhs,
                               "rtw.pil.SILPILInterface.sfunctionPILStopHook")!=
            XIL_UTILS_SUCCESS) {
          xilDisplayException(pXILUtils);
        }                              /* if */
      }                                /* if */
    }
  }

  if (pCommErrorOccurred != NULL) {
    mxFree(pCommErrorOccurred);
  }                                    /* if */

  if (rtIOStreamDataPtr != NULL) {
    {
      int errorStatus = rtIOStreamUnloadLib(&rtIOStreamDataPtr->libH);
      if (errorStatus) {
        mexErrMsgTxt( "rtIOStreamUnloadLib failed.");
      }                                /* if */
    }

    mxFree(rtIOStreamDataPtr->lib);
    mxDestroyArray(rtIOStreamDataPtr->MATLABObject);
    mxFree(rtIOStreamDataPtr);
  }                                    /* if */

  if (silDebuggingDataPtr != NULL) {
    mxFree(silDebuggingDataPtr->componentBlockPath);
    mxFree(silDebuggingDataPtr->SILPILInterfaceFcnStr);
    mxFree(silDebuggingDataPtr);
  }                                    /* if */

  if (IOBufferPtr != NULL) {
    mxFree(IOBufferPtr->data);
    mxFree(IOBufferPtr);
  }                                    /* if */

  closeAndFreeTargetIOFd();
  if (pXILUtils != NULL) {
    xilUtilsDestroy(pXILUtils);
  }                                    /* if */

  if (pComms != NULL) {
    xilCommsDestroy(pComms);
  }                                    /* if */

  if (pXILService != NULL) {
    xilServiceDestroy(pXILService);
  }                                    /* if */
}

static void mdlInitializeSizes()
{
  {
    mxArray * lhs[1];
    mxArray * error = NULL;
    char * installVersion;
    error = mexCallMATLABWithTrap(1, lhs, 0, NULL, "rtw.pil.getPILVersion");
    if (error != NULL) {
      mxDestroyArray(error);
      mexErrMsgTxt(
                   "Failed to determine the installed In-the-Loop version for comparison against the In-the-Loop s-function version (release 6.8 (R2015a)_12). To avoid this error, remove the In-the-Loop s-function from your MATLAB path (e.g. delete it or move to a clean working directory).");
      return;
    }                                  /* if */

    if (mxIsEmpty(lhs[0])) {
      mexErrMsgTxt( "rtw.pil.getPILVersion returned empty!");
      return;
    }                                  /* if */

    installVersion = mxArrayToString(lhs[0]);
    mxDestroyArray(lhs[0]);
    if (installVersion == NULL) {
      mexErrMsgTxt( "Failed to determine installed In-the-Loop version.");
      return;
    }                                  /* if */

    if (strcmp(installVersion, "6.8 (R2015a)_12") != 0) {
      mexErrMsgTxt(
                   "The In-the-Loop s-function is incompatible with the installed In-the-Loop version (see ver('ecoder')); it was generated for release 6.8 (R2015a)_12. To avoid this error, remove the In-the-Loop s-function from your MATLAB path (e.g. delete it or move to a clean working directory)");
      return;
    }                                  /* if */

    mxFree(installVersion);
  }
}

#define MDL_START                                                /* Change to #undef to remove function */
#if defined(MDL_START)

static void mdlStart()
{
  {
    {
      pCommErrorOccurred = (int *) mxCalloc(1, sizeof(int));
      if (pCommErrorOccurred == NULL) {
        mexErrMsgTxt(
                     "Error in allocating memory for pCommErrorOccurred through mxCalloc.");
        return;
      }                                /* if */

      *pCommErrorOccurred = 0;
      mexMakeMemoryPersistent(pCommErrorOccurred);
    }
  }

  {
    {
      mxArray *rhs[4];
      const char * simulinkBlockPath =
        "C:\\gpires\\Dropbox\\_gpires\\desenvolvimento\\DriveByMind_Simulink_SSVEP_IPT\\Online_Simulink_model_SSVEP_Unity\\algoritmo_C\\codegen\\lib\\gp_RPSSVEP\\gp_RPSSVEP";
      rhs[ 0 ] = mxCreateString(
        "@coder.connectivity.MATLABInterface.getSILPILInterface");
      rhs[ 1 ] = mxCreateDoubleScalar( 3 );
      rhs[ 2 ] = mxCreateString(simulinkBlockPath);
      rhs[3] = mxCreateString("uint8");
      callMATLAB( 0, NULL, 4, rhs,
                 "rtw.pil.SILPILInterface.sfunctionPILStartHook", 0);
    }
  }

  {
    mxArray *rhs[3];
    mxArray *lhs[5];
    const char * simulinkBlockPath =
      "C:\\gpires\\Dropbox\\_gpires\\desenvolvimento\\DriveByMind_Simulink_SSVEP_IPT\\Online_Simulink_model_SSVEP_Unity\\algoritmo_C\\codegen\\lib\\gp_RPSSVEP\\gp_RPSSVEP";
    rhs[ 0 ] = mxCreateString(
      "@coder.connectivity.MATLABInterface.getSILPILInterface");
    rhs[ 1 ] = mxCreateDoubleScalar( 3 );
    rhs[ 2 ] = mxCreateString(simulinkBlockPath);
    callMATLAB( 5, lhs, 3, rhs,
               "rtw.pil.SILPILInterface.sfunctionGetRtIOStreamInfoHook", 0);

    {
      rtIOStreamDataPtr = (XIL_RtIOStreamData_T*) mxCalloc(1, sizeof
        (XIL_RtIOStreamData_T));
      if (rtIOStreamDataPtr == NULL) {
        mexErrMsgTxt( "Error in allocating memory through mxCalloc.");
        return;
      }                                /* if */

      rtIOStreamDataPtr->lib = mxArrayToString(lhs[0]);
      rtIOStreamDataPtr->MATLABObject = mxDuplicateArray(lhs[1]);
      mexMakeMemoryPersistent(rtIOStreamDataPtr);
      mexMakeMemoryPersistent(rtIOStreamDataPtr->lib);
      mexMakeArrayPersistent(rtIOStreamDataPtr->MATLABObject);
      rtIOStreamDataPtr->streamID = *mxGetPr(lhs[2]);
      rtIOStreamDataPtr->recvTimeout = *mxGetPr(lhs[3]);
      rtIOStreamDataPtr->sendTimeout = *mxGetPr(lhs[4]);
      rtIOStreamDataPtr->isRtIOStreamCCall = 1;
      rtIOStreamDataPtr->ioMxClassID = mxUINT8_CLASS;
      rtIOStreamDataPtr->ioDataSize = sizeof(uint8_T);
      rtIOStreamDataPtr->targetRecvBufferSizeBytes = 50000;
      rtIOStreamDataPtr->targetSendBufferSizeBytes = 50000;

      {
        int errorStatus = rtIOStreamLoadLib(&rtIOStreamDataPtr->libH,
          rtIOStreamDataPtr->lib);
        if (errorStatus) {
          mexErrMsgTxt( "rtIOStreamLoadLib failed.");
          return;
        }                              /* if */
      }
    }

    {
      int i;
      for (i=0; i<5; i++) {
        mxDestroyArray(lhs[i]);
      }                                /* for */
    }
  }

  {
    IOBufferPtr = (XIL_IOBuffer_T *) mxCalloc(1, sizeof(XIL_IOBuffer_T));
    if (IOBufferPtr == NULL) {
      mexErrMsgTxt( "Error in allocating memory through mxCalloc.");
      return;
    }                                  /* if */

    mexMakeMemoryPersistent(IOBufferPtr);
  }

  {
    const char * simulinkBlockPath =
      "C:\\gpires\\Dropbox\\_gpires\\desenvolvimento\\DriveByMind_Simulink_SSVEP_IPT\\Online_Simulink_model_SSVEP_Unity\\algoritmo_C\\codegen\\lib\\gp_RPSSVEP\\gp_RPSSVEP";
    silDebuggingDataPtr = (SIL_DEBUGGING_DATA_T*) mxCalloc(1, sizeof
      (SIL_DEBUGGING_DATA_T));
    if (silDebuggingDataPtr == NULL) {
      mexErrMsgTxt(
                   "Error in allocating memory through mxCalloc for SIL_DEBUGGING_DATA_T.");
      return;
    }                                  /* if */

    silDebuggingDataPtr->componentBlockPath = strcpy((char *) mxCalloc(strlen
      (simulinkBlockPath)+1, sizeof(char)), simulinkBlockPath);
    silDebuggingDataPtr->SILPILInterfaceFcnStr = strcpy((char*) mxCalloc(55,
      sizeof(char)), "@coder.connectivity.MATLABInterface.getSILPILInterface");
    silDebuggingDataPtr->inTheLoopType = 3;
    mexMakeMemoryPersistent(silDebuggingDataPtr);
    mexMakeMemoryPersistent(silDebuggingDataPtr->componentBlockPath);
    mexMakeMemoryPersistent(silDebuggingDataPtr->SILPILInterfaceFcnStr);
  }

  {
    targetIOFdPtr = (targetIOFd_T *) mxCalloc(1, sizeof(targetIOFd_T));
    if (targetIOFdPtr == NULL) {
      return;
    }                                  /* if */

    mexMakeMemoryPersistent(targetIOFdPtr);
    targetIOFdPtr->size = 0;
    targetIOFdPtr->Fd = NULL;
    targetIOFdPtr->fidOffset = -1;
  }

  {
    int retValXILUtils;
    uint8_T memUnitSizeBytes = 1;
    uint8_T ioDataTypeSizeBytes = sizeof(uint8_T);
    retValXILUtils = xilMATLABUtilsCreate(&pXILUtils);
    if (retValXILUtils!=XIL_UTILS_SUCCESS) {
      mexErrMsgTxt( "Error instantiating XIL Utils!");
      return;
    }                                  /* if */

    if (xilCommsCreate(&pComms, rtIOStreamDataPtr, silDebuggingDataPtr,
                       memUnitSizeBytes, pMemUnitTransformer, pXILUtils) !=
        XILCOMMS_RTIOSTREAM_SUCCESS) {
      xilDisplayException(pXILUtils);
      return;
    }                                  /* if */

    if (xilServiceCreate(&pXILService, pComms, pXILUtils, IOBufferPtr,
                         memUnitSizeBytes, ioDataTypeSizeBytes, 0) !=
        XILSERVICE_SUCCESS) {
      xilDisplayException(pXILUtils);
      return;
    }                                  /* if */

    xilCommsRegisterApplication(pComms, pXILService);
  }

  {
    uint8_T * mxMemUnitPtr;
    mwSize dataInSize = 0;
    if (IOBufferPtr != NULL) {
      if (pXILService != NULL) {
        if (xilServiceGrowIOBuffer(pXILService, IOBufferPtr, 5, 0)!=
            XILSERVICE_SUCCESS) {
          xilDisplayException(pXILUtils);
          return;
        }                              /* if */

        if (IOBufferPtr->data != NULL) {
          mxMemUnitPtr = (uint8_T *) IOBufferPtr->data;

          /* write command id */
          *mxMemUnitPtr = 0;
          mxMemUnitPtr++;

          {
            uint8_T * simDataMemUnitPtr;
            uint32_T commandDataFcnid = 0;
            simDataMemUnitPtr = (uint8_T *) &commandDataFcnid;

            {
              copyIOData( mxMemUnitPtr, simDataMemUnitPtr, &(mxMemUnitPtr), 1, 1,
                         sizeof(uint32_T));
            }
          }

          /* dispatch command to the target */
          if (commandDispatch(IOBufferPtr, 5)!=XILSERVICE_SUCCESS) {
            return;
          }                            /* if */

          {
            XILCommandResponseType commandResponseType =
              XIL_COMMAND_NOT_COMPLETE;
            while (commandResponseType != XIL_STEP_COMPLETE) {
              /* receive command from the target */
              mwSize dataInSize = 0;
              if (commandResponse(&dataInSize, &commandResponseType) !=
                  XILSERVICE_SUCCESS) {
                return;
              }                        /* if */

              if (dataInSize > 0) {
                size_t dataInMemUnitSize = dataInSize;
                uint8_T responseId = 0;
                uint8_T * mxMemUnitPtrEnd;

#define RESPONSE_TYPE_SIZE             5

                mxMemUnitPtr = (uint8_T *) IOBufferPtr->data;
                mxMemUnitPtrEnd = mxMemUnitPtr + dataInMemUnitSize - 1;
                while (mxMemUnitPtr <= mxMemUnitPtrEnd) {
                  /* read response id */
                  responseId = *mxMemUnitPtr;
                  mxMemUnitPtr++;
                  switch (responseId) {
                   case RESPONSE_ERROR:
                   case RESPONSE_PRINTF:
                   case RESPONSE_FOPEN:
                   case RESPONSE_FPRINTF:
                    {
                      if (processErrorAndTargetIOResponseCases( responseId,
                           &mxMemUnitPtr, 0)== XILSERVICE_ERROR) {
                        return;
                      }                /* if */
                      break;
                    }

                   case RESPONSE_TYPE_SIZE:
                    {
                      uint8_T typeId = *mxMemUnitPtr;
                      uint8_T typeBytes;
                      mxMemUnitPtr++;
                      typeBytes = *mxMemUnitPtr;
                      mxMemUnitPtr++;
                      switch (typeId) {
                       case mxSINGLE_CLASS:
                        {
                          if (typeBytes != 4) {
                            {
                              mxArray * prhs[ 3 ];
                              prhs[0] = mxCreateString(
                                "PIL:pilverification:SingleUnsupported");
                              prhs[1] = mxCreateDoubleScalar(4);
                              prhs[2] = mxCreateDoubleScalar(typeBytes);
                              callMATLAB( 0, NULL, 3, prhs, "DAStudio.error", 0);
                              return;
                            }
                          }            /* if */
                          break;
                        }

                       case mxDOUBLE_CLASS:
                        {
                          if (typeBytes != 8) {
                            {
                              mxArray * prhs[ 3 ];
                              prhs[0] = mxCreateString(
                                "PIL:pilverification:DoubleUnsupported");
                              prhs[1] = mxCreateDoubleScalar(8);
                              prhs[2] = mxCreateDoubleScalar(typeBytes);
                              callMATLAB( 0, NULL, 3, prhs, "DAStudio.error", 0);
                              return;
                            }
                          }            /* if */
                          break;
                        }

                       default:
                        {
                          {
                            mxArray * prhs[ 2 ];
                            prhs[0] = mxCreateString(
                              "PIL:pilverification:UnknownTypeId");
                            prhs[1] = mxCreateDoubleScalar(typeId);
                            callMATLAB( 0, NULL, 2, prhs, "DAStudio.error", 0);
                            return;
                          }
                          break;
                        }
                      }                /* switch */
                      break;
                    }

                   default:
                    {
                      {
                        mxArray * prhs[ 2 ];
                        prhs[0] = mxCreateString(
                          "PIL:pilverification:UnknownResponseId");
                        prhs[1] = mxCreateDoubleScalar(responseId);
                        callMATLAB( 0, NULL, 2, prhs, "DAStudio.error", 0);
                        return;
                      }
                      break;
                    }
                  }                    /* switch */
                }                      /* while */
              }                        /* if */
            }                          /* while */
          }
        }                              /* if */
      }                                /* if */
    }                                  /* if */
  }

  {
    uint8_T * mxMemUnitPtr;
    mwSize dataInSize = 0;
    if (IOBufferPtr != NULL) {
      if (pXILService != NULL) {
        if (xilServiceGrowIOBuffer(pXILService, IOBufferPtr, 5, 0)!=
            XILSERVICE_SUCCESS) {
          xilDisplayException(pXILUtils);
          return;
        }                              /* if */

        if (IOBufferPtr->data != NULL) {
          mxMemUnitPtr = (uint8_T *) IOBufferPtr->data;

          /* write command id */
          *mxMemUnitPtr = 1;
          mxMemUnitPtr++;

          {
            uint8_T * simDataMemUnitPtr;
            uint32_T commandDataFcnid = 0;
            simDataMemUnitPtr = (uint8_T *) &commandDataFcnid;

            {
              copyIOData( mxMemUnitPtr, simDataMemUnitPtr, &(mxMemUnitPtr), 1, 1,
                         sizeof(uint32_T));
            }
          }

          /* dispatch command to the target */
          if (commandDispatch(IOBufferPtr, 5)!=XILSERVICE_SUCCESS) {
            return;
          }                            /* if */

          {
            XILCommandResponseType commandResponseType =
              XIL_COMMAND_NOT_COMPLETE;
            while (commandResponseType != XIL_STEP_COMPLETE) {
              /* receive command from the target */
              mwSize dataInSize = 0;
              if (commandResponse(&dataInSize, &commandResponseType) !=
                  XILSERVICE_SUCCESS) {
                return;
              }                        /* if */

              if (dataInSize > 0) {
                size_t dataInMemUnitSize = dataInSize;
                uint8_T responseId = 0;
                uint8_T * mxMemUnitPtrEnd;
                mxMemUnitPtr = (uint8_T *) IOBufferPtr->data;
                mxMemUnitPtrEnd = mxMemUnitPtr + dataInMemUnitSize - 1;
                while (mxMemUnitPtr <= mxMemUnitPtrEnd) {
                  /* read response id */
                  responseId = *mxMemUnitPtr;
                  mxMemUnitPtr++;
                  switch (responseId) {
                   case RESPONSE_ERROR:
                   case RESPONSE_PRINTF:
                   case RESPONSE_FOPEN:
                   case RESPONSE_FPRINTF:
                    {
                      if (processErrorAndTargetIOResponseCases( responseId,
                           &mxMemUnitPtr, 0)== XILSERVICE_ERROR) {
                        return;
                      }                /* if */
                      break;
                    }

                   default:
                    {
                      {
                        mxArray * prhs[ 2 ];
                        prhs[0] = mxCreateString(
                          "PIL:pilverification:UnknownResponseId");
                        prhs[1] = mxCreateDoubleScalar(responseId);
                        callMATLAB( 0, NULL, 2, prhs, "DAStudio.error", 0);
                        return;
                      }
                      break;
                    }
                  }                    /* switch */
                }                      /* while */
              }                        /* if */
            }                          /* while */
          }
        }                              /* if */
      }                                /* if */
    }                                  /* if */
  }
}

#endif                                 /* MDL_START */

void gp_RPSSVEP( real_T yr, real_T ys[4096], real_T f_interest[6], real_T
                canais[4], real_T win_type, real_T overlap, real_T NFFT, real_T
                fs, real_T class_type, real_T GTargets[3], real_T* idx )
{
  if (!firstTime) {
    firstTime = 1;
    mdlInitializeSizes();
    mdlStart();
  }                                    /* if */

  {
    uint8_T * mxMemUnitPtr;
    mwSize dataInSize = 0;
    if (IOBufferPtr != NULL) {
      if (pXILService != NULL) {
        if (xilServiceGrowIOBuffer(pXILService, IOBufferPtr, 32905, 0)!=
            XILSERVICE_SUCCESS) {
          xilDisplayException(pXILUtils);
          return;
        }                              /* if */

        if (IOBufferPtr->data != NULL) {
          mxMemUnitPtr = (uint8_T *) IOBufferPtr->data;

          /* write command id */
          *mxMemUnitPtr = 3;
          mxMemUnitPtr++;

          {
            uint8_T * simDataMemUnitPtr;
            uint32_T commandDataFcnidTID[2] = { 0, 1 };

            simDataMemUnitPtr = (uint8_T *) &commandDataFcnidTID[0];

            {
              copyIOData( mxMemUnitPtr, simDataMemUnitPtr, &(mxMemUnitPtr), 2, 2,
                         sizeof(uint32_T));
            }
          }

          {
            uint8_T * simDataMemUnitPtr;
            simDataMemUnitPtr = ( uint8_T *) &(yr);

            {
              copyIOData( mxMemUnitPtr, simDataMemUnitPtr, &(mxMemUnitPtr), 1, 1,
                         sizeof(real64_T));
            }
          }

          {
            uint8_T * simDataMemUnitPtr;
            simDataMemUnitPtr = ( uint8_T *) &(ys[0]);

            {
              copyIOData( mxMemUnitPtr, simDataMemUnitPtr, &(mxMemUnitPtr), 4096,
                         4096, sizeof(real64_T));
            }
          }

          {
            uint8_T * simDataMemUnitPtr;
            simDataMemUnitPtr = ( uint8_T *) &(f_interest[0]);

            {
              copyIOData( mxMemUnitPtr, simDataMemUnitPtr, &(mxMemUnitPtr), 6, 6,
                         sizeof(real64_T));
            }
          }

          {
            uint8_T * simDataMemUnitPtr;
            simDataMemUnitPtr = ( uint8_T *) &(canais[0]);

            {
              copyIOData( mxMemUnitPtr, simDataMemUnitPtr, &(mxMemUnitPtr), 4, 4,
                         sizeof(real64_T));
            }
          }

          {
            uint8_T * simDataMemUnitPtr;
            simDataMemUnitPtr = ( uint8_T *) &(win_type);

            {
              copyIOData( mxMemUnitPtr, simDataMemUnitPtr, &(mxMemUnitPtr), 1, 1,
                         sizeof(real64_T));
            }
          }

          {
            uint8_T * simDataMemUnitPtr;
            simDataMemUnitPtr = ( uint8_T *) &(overlap);

            {
              copyIOData( mxMemUnitPtr, simDataMemUnitPtr, &(mxMemUnitPtr), 1, 1,
                         sizeof(real64_T));
            }
          }

          {
            uint8_T * simDataMemUnitPtr;
            simDataMemUnitPtr = ( uint8_T *) &(NFFT);

            {
              copyIOData( mxMemUnitPtr, simDataMemUnitPtr, &(mxMemUnitPtr), 1, 1,
                         sizeof(real64_T));
            }
          }

          {
            uint8_T * simDataMemUnitPtr;
            simDataMemUnitPtr = ( uint8_T *) &(fs);

            {
              copyIOData( mxMemUnitPtr, simDataMemUnitPtr, &(mxMemUnitPtr), 1, 1,
                         sizeof(real64_T));
            }
          }

          {
            uint8_T * simDataMemUnitPtr;
            simDataMemUnitPtr = ( uint8_T *) &(class_type);

            {
              copyIOData( mxMemUnitPtr, simDataMemUnitPtr, &(mxMemUnitPtr), 1, 1,
                         sizeof(real64_T));
            }
          }

          /* dispatch command to the target */
          if (commandDispatch(IOBufferPtr, 32905)!=XILSERVICE_SUCCESS) {
            return;
          }                            /* if */

          {
            XILCommandResponseType commandResponseType =
              XIL_COMMAND_NOT_COMPLETE;
            while (commandResponseType != XIL_STEP_COMPLETE) {
              /* receive command from the target */
              mwSize dataInSize = 0;
              if (commandResponse(&dataInSize, &commandResponseType) !=
                  XILSERVICE_SUCCESS) {
                return;
              }                        /* if */

              if (dataInSize > 0) {
                size_t dataInMemUnitSize = dataInSize;
                uint8_T responseId = 0;
                uint8_T * mxMemUnitPtrEnd;
                mxMemUnitPtr = (uint8_T *) IOBufferPtr->data;
                mxMemUnitPtrEnd = mxMemUnitPtr + dataInMemUnitSize - 1;
                while (mxMemUnitPtr <= mxMemUnitPtrEnd) {
                  /* read response id */
                  responseId = *mxMemUnitPtr;
                  mxMemUnitPtr++;
                  switch (responseId) {
                   case RESPONSE_ERROR:
                   case RESPONSE_PRINTF:
                   case RESPONSE_FOPEN:
                   case RESPONSE_FPRINTF:
                    {
                      if (processErrorAndTargetIOResponseCases( responseId,
                           &mxMemUnitPtr, 0)== XILSERVICE_ERROR) {
                        return;
                      }                /* if */
                      break;
                    }

                   case RESPONSE_OUTPUT_DATA:
                    {
                      {
                        uint8_T * simDataMemUnitPtr;
                        simDataMemUnitPtr = ( uint8_T *) &(GTargets[0]);

                        {
                          copyIOData( simDataMemUnitPtr, mxMemUnitPtr,
                                     &(mxMemUnitPtr), 3, 3, sizeof(real64_T));
                        }
                      }

                      {
                        uint8_T * simDataMemUnitPtr;
                        simDataMemUnitPtr = ( uint8_T *) (idx);

                        {
                          copyIOData( simDataMemUnitPtr, mxMemUnitPtr,
                                     &(mxMemUnitPtr), 1, 1, sizeof(real64_T));
                        }
                      }
                      break;
                    }

                   default:
                    {
                      {
                        mxArray * prhs[ 2 ];
                        prhs[0] = mxCreateString(
                          "PIL:pilverification:UnknownResponseId");
                        prhs[1] = mxCreateDoubleScalar(responseId);
                        callMATLAB( 0, NULL, 2, prhs, "DAStudio.error", 0);
                        return;
                      }
                      break;
                    }
                  }                    /* switch */
                }                      /* while */
              }                        /* if */
            }                          /* while */
          }
        }                              /* if */
      }                                /* if */
    }                                  /* if */
  }
}

void gp_RPSSVEP_xil_terminate()
{
  int isOkToTerminate;
  int commErrorOccurred = 0;

  {
    if (pCommErrorOccurred != NULL) {
      commErrorOccurred = *pCommErrorOccurred;
    }                                  /* if */
  }

  {
    {
      mxArray *rhs[3];
      mxArray *lhs[1];
      const char * simulinkBlockPath =
        "C:\\gpires\\Dropbox\\_gpires\\desenvolvimento\\DriveByMind_Simulink_SSVEP_IPT\\Online_Simulink_model_SSVEP_Unity\\algoritmo_C\\codegen\\lib\\gp_RPSSVEP\\gp_RPSSVEP";
      rhs[ 0 ] = mxCreateString(
        "@coder.connectivity.MATLABInterface.getSILPILInterface");
      rhs[ 1 ] = mxCreateDoubleScalar( 3 );
      rhs[ 2 ] = mxCreateString(simulinkBlockPath);
      callMATLAB( 1, lhs, 3, rhs,
                 "rtw.pil.SILPILInterface.sfunctionIsOkToTerminateHook", 0);
      isOkToTerminate = (int) (*mxGetPr(lhs[0]));
      mxDestroyArray(lhs[0]);
    }
  }

  if (isOkToTerminate) {
    if (!commErrorOccurred) {
      {
        uint8_T * mxMemUnitPtr;
        mwSize dataInSize = 0;
        if (IOBufferPtr != NULL) {
          if (pXILService != NULL) {
            if (xilServiceGrowIOBuffer(pXILService, IOBufferPtr, 5, 0)!=
                XILSERVICE_SUCCESS) {
              xilDisplayException(pXILUtils);
              callStopHookAndFreeSFcnMemory();
              return;
            }                          /* if */

            if (IOBufferPtr->data != NULL) {
              mxMemUnitPtr = (uint8_T *) IOBufferPtr->data;

              /* write command id */
              *mxMemUnitPtr = 4;
              mxMemUnitPtr++;

              {
                uint8_T * simDataMemUnitPtr;
                uint32_T commandDataFcnid = 0;
                simDataMemUnitPtr = (uint8_T *) &commandDataFcnid;

                {
                  copyIOData( mxMemUnitPtr, simDataMemUnitPtr, &(mxMemUnitPtr),
                             1, 1, sizeof(uint32_T));
                }
              }

              /* dispatch command to the target */
              if (commandDispatch(IOBufferPtr, 5)!=XILSERVICE_SUCCESS) {
                callStopHookAndFreeSFcnMemory();
                return;
              }                        /* if */

              {
                XILCommandResponseType commandResponseType =
                  XIL_COMMAND_NOT_COMPLETE;
                while (commandResponseType != XIL_STEP_COMPLETE) {
                  /* receive command from the target */
                  mwSize dataInSize = 0;
                  if (commandResponse(&dataInSize, &commandResponseType) !=
                      XILSERVICE_SUCCESS) {
                    callStopHookAndFreeSFcnMemory();
                    return;
                  }                    /* if */
                }                      /* while */
              }
            }                          /* if */
          }                            /* if */
        }                              /* if */
      }

      {
        uint8_T * mxMemUnitPtr;
        mwSize dataInSize = 0;
        if (IOBufferPtr != NULL) {
          if (pXILService != NULL) {
            if (xilServiceGrowIOBuffer(pXILService, IOBufferPtr, 1, 0)!=
                XILSERVICE_SUCCESS) {
              xilDisplayException(pXILUtils);
              callStopHookAndFreeSFcnMemory();
              return;
            }                          /* if */

            if (IOBufferPtr->data != NULL) {
              mxMemUnitPtr = (uint8_T *) IOBufferPtr->data;

              /* write command id */
              *mxMemUnitPtr = 10;
              mxMemUnitPtr++;

              /* dispatch command to the target */
              if (commandDispatch(IOBufferPtr, 1)!=XILSERVICE_SUCCESS) {
                callStopHookAndFreeSFcnMemory();
                return;
              }                        /* if */
            }                          /* if */
          }                            /* if */
        }                              /* if */
      }
    }                                  /* if */

    callStopHookAndFreeSFcnMemory();
  }                                    /* if */
}
