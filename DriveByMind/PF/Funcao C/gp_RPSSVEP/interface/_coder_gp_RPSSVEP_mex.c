/*
 * File: _coder_gp_RPSSVEP_mex.c
 *
 * MATLAB Coder version            : 2.8
 * C/C++ source code generated on  : 11-Sep-2015 21:01:56
 */

/* Include Files */
#include "_coder_gp_RPSSVEP_api.h"
#include "_coder_gp_RPSSVEP_mex.h"

/* Function Declarations */
static void gp_RPSSVEP_mexFunction(int32_T nlhs, mxArray *plhs[2], int32_T nrhs,
  const mxArray *prhs[9]);

/* Function Definitions */

/*
 * Arguments    : int32_T nlhs
 *                const mxArray *plhs[2]
 *                int32_T nrhs
 *                const mxArray *prhs[9]
 * Return Type  : void
 */
static void gp_RPSSVEP_mexFunction(int32_T nlhs, mxArray *plhs[2], int32_T nrhs,
  const mxArray *prhs[9])
{
  int32_T n;
  const mxArray *inputs[9];
  const mxArray *outputs[2];
  int32_T b_nlhs;
  emlrtStack st = { NULL, NULL, NULL };

  st.tls = emlrtRootTLSGlobal;

  /* Check for proper number of arguments. */
  if (nrhs != 9) {
    emlrtErrMsgIdAndTxt(&st, "EMLRT:runTime:WrongNumberOfInputs", 5, 12, 9, 4,
                        10, "gp_RPSSVEP");
  }

  if (nlhs > 2) {
    emlrtErrMsgIdAndTxt(&st, "EMLRT:runTime:TooManyOutputArguments", 3, 4, 10,
                        "gp_RPSSVEP");
  }

  /* Temporary copy for mex inputs. */
  for (n = 0; n < nrhs; n++) {
    inputs[n] = prhs[n];
  }

  /* Call the function. */
  gp_RPSSVEP_api(inputs, outputs);

  /* Copy over outputs to the caller. */
  if (nlhs < 1) {
    b_nlhs = 1;
  } else {
    b_nlhs = nlhs;
  }

  emlrtReturnArrays(b_nlhs, plhs, outputs);

  /* Module termination. */
  gp_RPSSVEP_terminate();
}

/*
 * Arguments    : int32_T nlhs
 *                const mxArray * const plhs[]
 *                int32_T nrhs
 *                const mxArray * const prhs[]
 * Return Type  : void
 */
void mexFunction(int32_T nlhs, mxArray *plhs[], int32_T nrhs, const mxArray
                 *prhs[])
{
  /* Initialize the memory manager. */
  mexAtExit(gp_RPSSVEP_atexit);

  /* Module initialization. */
  gp_RPSSVEP_initialize();

  /* Dispatch the entry-point. */
  gp_RPSSVEP_mexFunction(nlhs, plhs, nrhs, prhs);
}

/*
 * File trailer for _coder_gp_RPSSVEP_mex.c
 *
 * [EOF]
 */
