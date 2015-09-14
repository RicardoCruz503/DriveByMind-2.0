/*
 * File: gp_pwelch.h
 *
 * MATLAB Coder version            : 2.8
 * C/C++ source code generated on  : 11-Sep-2015 21:01:56
 */

#ifndef __GP_PWELCH_H__
#define __GP_PWELCH_H__

/* Include Files */
#include <math.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "rtwtypes.h"
#include "gp_RPSSVEP_types.h"

/* Function Declarations */
extern void gp_pwelch(const double x[512], const double win_data[], double
                      overlap, double NFFT, double fs, emxArray_real_T *Psd,
                      emxArray_real_T *f);

#endif

/*
 * File trailer for gp_pwelch.h
 *
 * [EOF]
 */
