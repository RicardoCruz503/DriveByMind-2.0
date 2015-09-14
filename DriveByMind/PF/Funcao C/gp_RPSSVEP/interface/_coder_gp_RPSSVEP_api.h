/*
 * File: _coder_gp_RPSSVEP_api.h
 *
 * MATLAB Coder version            : 2.8
 * C/C++ source code generated on  : 11-Sep-2015 21:01:56
 */

#ifndef ___CODER_GP_RPSSVEP_API_H__
#define ___CODER_GP_RPSSVEP_API_H__

/* Include Files */
#include "tmwtypes.h"
#include "mex.h"
#include "emlrt.h"
#include <stddef.h>
#include <stdlib.h>
#include "_coder_gp_RPSSVEP_api.h"

/* Variable Declarations */
extern emlrtCTX emlrtRootTLSGlobal;
extern emlrtContext emlrtContextGlobal;

/* Function Declarations */
extern void gp_RPSSVEP(real_T yr, real_T ys[4096], real_T f_interest[6], real_T
  canais[4], real_T win_type, real_T overlap, real_T NFFT, real_T fs, real_T
  class_type, real_T GTargets[3], real_T *idx);
extern void gp_RPSSVEP_api(const mxArray *prhs[9], const mxArray *plhs[2]);
extern void gp_RPSSVEP_atexit(void);
extern void gp_RPSSVEP_initialize(void);
extern void gp_RPSSVEP_terminate(void);
extern void gp_RPSSVEP_xil_terminate(void);

#endif

/*
 * File trailer for _coder_gp_RPSSVEP_api.h
 *
 * [EOF]
 */
