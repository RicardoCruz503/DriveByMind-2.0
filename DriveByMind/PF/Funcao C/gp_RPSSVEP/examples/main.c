/*
 * File: main.c
 *
 * MATLAB Coder version            : 2.8
 * C/C++ source code generated on  : 11-Sep-2015 21:01:56
 */

/*************************************************************************/
/* This automatically generated example C main file shows how to call    */
/* entry-point functions that MATLAB Coder generated. You must customize */
/* this file for your application. Do not modify this file directly.     */
/* Instead, make a copy of this file, modify it, and integrate it into   */
/* your development environment.                                         */
/*                                                                       */
/* This file initializes entry-point function arguments to a default     */
/* size and value before calling the entry-point functions. It does      */
/* not store or use any values returned from the entry-point functions.  */
/* If necessary, it does pre-allocate memory for returned values.        */
/* You can use this file as a starting point for a main function that    */
/* you can deploy in your application.                                   */
/*                                                                       */
/* After you copy the file, and before you deploy it, you must make the  */
/* following changes:                                                    */
/* * For variable-size function arguments, change the example sizes to   */
/* the sizes that your application requires.                             */
/* * Change the example values of function arguments to the values that  */
/* your application requires.                                            */
/* * If the entry-point functions return values, store these values or   */
/* otherwise use them as required by your application.                   */
/*                                                                       */
/*************************************************************************/
/* Include Files */
#include "gp_RPSSVEP.h"
#include "main.h"
#include "gp_RPSSVEP_terminate.h"
#include "gp_RPSSVEP_initialize.h"

/* Function Declarations */
static void argInit_1x4_real_T(double result[4]);
static void argInit_1x6_real_T(double result[6]);
static void argInit_8x512_real_T(double result[4096]);
static double argInit_real_T(void);
static void main_gp_RPSSVEP(void);

/* Function Definitions */

/*
 * Arguments    : double result[4]
 * Return Type  : void
 */
static void argInit_1x4_real_T(double result[4])
{
  int b_j1;

  /* Loop over the array to initialize each element. */
  for (b_j1 = 0; b_j1 < 4; b_j1++) {
    /* Set the value of the array element.
       Change this value to the value that the application requires. */
    result[b_j1] = argInit_real_T();
  }
}

/*
 * Arguments    : double result[6]
 * Return Type  : void
 */
static void argInit_1x6_real_T(double result[6])
{
  int b_j1;

  /* Loop over the array to initialize each element. */
  for (b_j1 = 0; b_j1 < 6; b_j1++) {
    /* Set the value of the array element.
       Change this value to the value that the application requires. */
    result[b_j1] = argInit_real_T();
  }
}

/*
 * Arguments    : double result[4096]
 * Return Type  : void
 */
static void argInit_8x512_real_T(double result[4096])
{
  int b_j0;
  int b_j1;

  /* Loop over the array to initialize each element. */
  for (b_j0 = 0; b_j0 < 8; b_j0++) {
    for (b_j1 = 0; b_j1 < 512; b_j1++) {
      /* Set the value of the array element.
         Change this value to the value that the application requires. */
      result[b_j0 + (b_j1 << 3)] = argInit_real_T();
    }
  }
}

/*
 * Arguments    : void
 * Return Type  : double
 */
static double argInit_real_T(void)
{
  return 0.0;
}

/*
 * Arguments    : void
 * Return Type  : void
 */
static void main_gp_RPSSVEP(void)
{
  double dv1[4096];
  double dv2[6];
  double dv3[4];
  double idx;
  double GTargets[3];

  /* Initialize function 'gp_RPSSVEP' input arguments. */
  /* Initialize function input argument 'ys'. */
  /* Initialize function input argument 'f_interest'. */
  /* Initialize function input argument 'canais'. */
  /* Call the entry-point 'gp_RPSSVEP'. */
  argInit_8x512_real_T(dv1);
  argInit_1x6_real_T(dv2);
  argInit_1x4_real_T(dv3);
  gp_RPSSVEP(argInit_real_T(), dv1, dv2, dv3, argInit_real_T(), argInit_real_T(),
             argInit_real_T(), argInit_real_T(), argInit_real_T(), GTargets,
             &idx);
}

/*
 * Arguments    : int argc
 *                const char * const argv[]
 * Return Type  : int
 */
int main(int argc, const char * const argv[])
{
  (void)argc;
  (void)argv;

  /* Initialize the application.
     You do not need to do this more than one time. */
  gp_RPSSVEP_initialize();

  /* Invoke the entry-point functions.
     You can call entry-point functions multiple times. */
  main_gp_RPSSVEP();

  /* Terminate the application.
     You do not need to do this more than one time. */
  gp_RPSSVEP_terminate();
  return 0;
}

/*
 * File trailer for main.c
 *
 * [EOF]
 */
