/*
 * File: xil_interface.c
 *
 * SIL generated interface for code: "gp_RPSSVEP"
 *
 */

#include "xil_interface.h"
#include "gp_RPSSVEP.h"
#include "gp_RPSSVEP_initialize.h"
#include "gp_RPSSVEP_terminate.h"

/* Functions with a C call interface */
#ifdef __cplusplus

extern "C" {

#endif

#include "xil_data_stream.h"
#ifdef __cplusplus

}
#endif

/* interface data */
double yr;

/* interface data */
double ys[4096];

/* interface data */
double f_interest[6];

/* interface data */
double canais[4];

/* interface data */
double win_type;

/* interface data */
double overlap;

/* interface data */
double NFFT;

/* interface data */
double fs;

/* interface data */
double class_type;

/* interface data */
double GTargets[3];

/* interface data */
double idx;
static XILIOData xil_fcnid0_task1_u[10];
static XILIOData xil_fcnid0_task1_y[3];

/* In-the-Loop Interface functions - see xil_interface.h */
XIL_INTERFACE_ERROR_CODE xilProcessParams(uint32_T xilFcnId)
{
  /* Single In-the-Loop Component */
  if (xilFcnId != 0) {
    return XIL_INTERFACE_UNKNOWN_FCNID;
  }

  return XIL_INTERFACE_SUCCESS;
}

XIL_INTERFACE_ERROR_CODE xilGetDataTypeInfo(void)
{
  {
    /* send response id code */
    MemUnit_T memUnitData = XIL_RESPONSE_TYPE_SIZE;
    if (xilWriteData(&memUnitData, sizeof(memUnitData)) !=
        XIL_DATA_STREAM_SUCCESS) {
      return XIL_INTERFACE_COMMS_FAILURE;
    }

    /* send type id */
    memUnitData = 6;
    if (xilWriteData(&memUnitData, sizeof(memUnitData)) !=
        XIL_DATA_STREAM_SUCCESS) {
      return XIL_INTERFACE_COMMS_FAILURE;
    }

    /* PIL_DOUBLE_SIZE should only be already defined for MathWorks testing */
#ifndef PIL_DOUBLE_SIZE
#define PIL_DOUBLE_SIZE                sizeof(double)
#endif

    /* send size in bytes */
    memUnitData = (MemUnit_T) PIL_DOUBLE_SIZE;

#ifndef HOST_WORD_ADDRESSABLE_TESTING

    /* convert MemUnits to bytes */
    memUnitData *= MEM_UNIT_BYTES;

#endif

    if (xilWriteData(&memUnitData, sizeof(memUnitData)) !=
        XIL_DATA_STREAM_SUCCESS) {
      return XIL_INTERFACE_COMMS_FAILURE;
    }
  }

  return XIL_INTERFACE_SUCCESS;
}

XIL_INTERFACE_ERROR_CODE xilInitialize(uint32_T xilFcnId)
{
  XIL_INTERFACE_ERROR_CODE errorCode = XIL_INTERFACE_SUCCESS;

  /* initialize output storage owned by In-the-Loop */
  /* Call all Get class methods */
  {
    {
      uint32_T elementIdx;
      for (elementIdx=0; elementIdx<3; elementIdx++) {
        GTargets[elementIdx] = 0;
      }
    }
  }

  {
    idx = 0;
  }

  /* Single In-the-Loop Component */
  if (xilFcnId == 0) {
    gp_RPSSVEP_initialize();
  } else {
    errorCode = XIL_INTERFACE_UNKNOWN_FCNID;
  }

  return errorCode;
}

XIL_INTERFACE_ERROR_CODE xilInitializeConditions(uint32_T xilFcnId)
{
  XIL_INTERFACE_ERROR_CODE errorCode = XIL_INTERFACE_SUCCESS;

  /* Single In-the-Loop Component */
  if (xilFcnId == 0) {
    /* No Initialize Conditions Function to Call */
  } else {
    errorCode = XIL_INTERFACE_UNKNOWN_FCNID;
  }

  return errorCode;
}

XIL_INTERFACE_ERROR_CODE xilGetHostToTargetData(uint32_T xilFcnId,
  XIL_COMMAND_TYPE_ENUM xilCommandType, uint32_T xilCommandIdx, XILIOData
  ** xilIOData)
{
  XIL_INTERFACE_ERROR_CODE errorCode = XIL_INTERFACE_SUCCESS;
  *xilIOData = 0;

  /* Single In-the-Loop Component */
  if (xilFcnId != 0) {
    errorCode = XIL_INTERFACE_UNKNOWN_FCNID;
    return errorCode;
  }

  switch (xilCommandType) {
   case XIL_STEP_COMMAND:
    {
      static int initComplete = 0;
      if (!initComplete) {
        uint32_T tableIdx = 0;

        {
          void * dataAddress = (void *) &(yr);
          xil_fcnid0_task1_u[tableIdx].memUnitLength = 1 * sizeof(double);
          xil_fcnid0_task1_u[tableIdx++].address = (MemUnit_T *) dataAddress;
        }

        {
          void * dataAddress = (void *) &(ys[0]);
          xil_fcnid0_task1_u[tableIdx].memUnitLength = 4096 * sizeof(double);
          xil_fcnid0_task1_u[tableIdx++].address = (MemUnit_T *) dataAddress;
        }

        {
          void * dataAddress = (void *) &(f_interest[0]);
          xil_fcnid0_task1_u[tableIdx].memUnitLength = 6 * sizeof(double);
          xil_fcnid0_task1_u[tableIdx++].address = (MemUnit_T *) dataAddress;
        }

        {
          void * dataAddress = (void *) &(canais[0]);
          xil_fcnid0_task1_u[tableIdx].memUnitLength = 4 * sizeof(double);
          xil_fcnid0_task1_u[tableIdx++].address = (MemUnit_T *) dataAddress;
        }

        {
          void * dataAddress = (void *) &(win_type);
          xil_fcnid0_task1_u[tableIdx].memUnitLength = 1 * sizeof(double);
          xil_fcnid0_task1_u[tableIdx++].address = (MemUnit_T *) dataAddress;
        }

        {
          void * dataAddress = (void *) &(overlap);
          xil_fcnid0_task1_u[tableIdx].memUnitLength = 1 * sizeof(double);
          xil_fcnid0_task1_u[tableIdx++].address = (MemUnit_T *) dataAddress;
        }

        {
          void * dataAddress = (void *) &(NFFT);
          xil_fcnid0_task1_u[tableIdx].memUnitLength = 1 * sizeof(double);
          xil_fcnid0_task1_u[tableIdx++].address = (MemUnit_T *) dataAddress;
        }

        {
          void * dataAddress = (void *) &(fs);
          xil_fcnid0_task1_u[tableIdx].memUnitLength = 1 * sizeof(double);
          xil_fcnid0_task1_u[tableIdx++].address = (MemUnit_T *) dataAddress;
        }

        {
          void * dataAddress = (void *) &(class_type);
          xil_fcnid0_task1_u[tableIdx].memUnitLength = 1 * sizeof(double);
          xil_fcnid0_task1_u[tableIdx++].address = (MemUnit_T *) dataAddress;
        }

        xil_fcnid0_task1_u[tableIdx].memUnitLength = 0;
        xil_fcnid0_task1_u[tableIdx++].address = (MemUnit_T *) 0;
        initComplete = 1;
      }

      *xilIOData = &xil_fcnid0_task1_u[0];
      break;
    }

   default:
    errorCode = XIL_INTERFACE_UNKNOWN_TID;
    break;
  }

  UNUSED_PARAMETER(xilCommandIdx);
  return errorCode;
}

XIL_INTERFACE_ERROR_CODE xilOutput(uint32_T xilFcnId, uint32_T xilTID)
{
  /* Single In-the-Loop Component */
  if (xilFcnId != 0) {
    return XIL_INTERFACE_UNKNOWN_FCNID;
  }

  switch (xilTID) {
   case 1:
    /* Call all Set class methods */
    gp_RPSSVEP(yr, ys, f_interest, canais, win_type, overlap, NFFT, fs,
               class_type, GTargets, &(idx));

    /* Call all Get class methods */
    break;

   default:
    return XIL_INTERFACE_UNKNOWN_TID;
  }

  return XIL_INTERFACE_SUCCESS;
}

XIL_INTERFACE_ERROR_CODE xilUpdate(uint32_T xilFcnId, uint32_T xilTID)
{
  /* Single In-the-Loop Component */
  if (xilFcnId != 0) {
    return XIL_INTERFACE_UNKNOWN_FCNID;
  }

  /* No Update Function */
  UNUSED_PARAMETER(xilTID);
  return XIL_INTERFACE_SUCCESS;
}

XIL_INTERFACE_ERROR_CODE xilGetTargetToHostData(uint32_T xilFcnId,
  XIL_COMMAND_TYPE_ENUM xilCommandType, uint32_T xilCommandIdx, XILIOData
  ** xilIOData)
{
  XIL_INTERFACE_ERROR_CODE errorCode = XIL_INTERFACE_SUCCESS;

  /* Single In-the-Loop Component */
  *xilIOData = 0;
  if (xilFcnId != 0) {
    errorCode = XIL_INTERFACE_UNKNOWN_FCNID;
    return errorCode;
  }

  switch (xilCommandType) {
   case XIL_STEP_COMMAND:
    {
      static int initComplete = 0;
      if (!initComplete) {
        uint32_T tableIdx = 0;

        {
          void * dataAddress = (void *) &(GTargets[0]);
          xil_fcnid0_task1_y[tableIdx].memUnitLength = 3 * sizeof(double);
          xil_fcnid0_task1_y[tableIdx++].address = (MemUnit_T *) dataAddress;
        }

        {
          void * dataAddress = (void *) &(idx);
          xil_fcnid0_task1_y[tableIdx].memUnitLength = 1 * sizeof(double);
          xil_fcnid0_task1_y[tableIdx++].address = (MemUnit_T *) dataAddress;
        }

        xil_fcnid0_task1_y[tableIdx].memUnitLength = 0;
        xil_fcnid0_task1_y[tableIdx++].address = (MemUnit_T *) 0;
        initComplete = 1;
      }

      *xilIOData = &xil_fcnid0_task1_y[0];
      break;
    }

   default:
    errorCode = XIL_INTERFACE_UNKNOWN_TID;
    break;
  }

  UNUSED_PARAMETER(xilCommandIdx);
  return errorCode;
}

XIL_INTERFACE_ERROR_CODE xilTerminate(uint32_T xilFcnId)
{
  /* Single In-the-Loop Component */
  if (xilFcnId != 0) {
    return XIL_INTERFACE_UNKNOWN_FCNID;
  }

  gp_RPSSVEP_terminate();
  return XIL_INTERFACE_SUCCESS;
}

XIL_INTERFACE_ERROR_CODE xilEnable(uint32_T xilFcnId, uint32_T xilTID)
{
  /* Single In-the-Loop Component */
  if (xilFcnId != 0) {
    return XIL_INTERFACE_UNKNOWN_FCNID;
  }

  UNUSED_PARAMETER(xilTID);

  /* No Enable Function - this function should never be called */
  return XIL_INTERFACE_UNKNOWN_TID;
}

XIL_INTERFACE_ERROR_CODE xilDisable(uint32_T xilFcnId, uint32_T xilTID)
{
  /* Single In-the-Loop Component */
  if (xilFcnId != 0) {
    return XIL_INTERFACE_UNKNOWN_FCNID;
  }

  UNUSED_PARAMETER(xilTID);

  /* No Disable Function - this function should never be called */
  return XIL_INTERFACE_UNKNOWN_TID;
}
