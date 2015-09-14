/*
 * File: gp_RPSSVEP.h
 *
 * MATLAB Coder version            : 2.8
 * C/C++ source code generated on  : 11-Sep-2015 21:01:56
 */

#ifndef __GP_RPSSVEP_H__
#define __GP_RPSSVEP_H__

/* Include Files */
#include <math.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "rtwtypes.h"
#include "gp_RPSSVEP_types.h"

/* Function Declarations */
extern void gp_RPSSVEP(double yr, const double ys[4096], const double
  f_interest[6], const double canais[4], double win_type, double overlap, double
  NFFT, double fs, double class_type, double GTargets[3], double *idx);

#endif

/*
 * File trailer for gp_RPSSVEP.h
 *
 * [EOF]
 */
