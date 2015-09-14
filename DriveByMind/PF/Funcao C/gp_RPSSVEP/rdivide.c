/*
 * File: rdivide.c
 *
 * MATLAB Coder version            : 2.8
 * C/C++ source code generated on  : 11-Sep-2015 21:01:56
 */

/* Include Files */
#include "gp_RPSSVEP.h"
#include "rdivide.h"
#include "gp_RPSSVEP_emxutil.h"

/* Function Definitions */

/*
 * Arguments    : const emxArray_real_T *x
 *                double y
 *                emxArray_real_T *z
 * Return Type  : void
 */
void rdivide(const emxArray_real_T *x, double y, emxArray_real_T *z)
{
  int i1;
  int loop_ub;
  i1 = z->size[0] * z->size[1];
  z->size[0] = 1;
  z->size[1] = x->size[1];
  emxEnsureCapacity((emxArray__common *)z, i1, (int)sizeof(double));
  loop_ub = x->size[0] * x->size[1];
  for (i1 = 0; i1 < loop_ub; i1++) {
    z->data[i1] = x->data[i1] / y;
  }
}

/*
 * File trailer for rdivide.c
 *
 * [EOF]
 */
