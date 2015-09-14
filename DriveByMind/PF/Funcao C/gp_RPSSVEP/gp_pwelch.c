/*
 * File: gp_pwelch.c
 *
 * MATLAB Coder version            : 2.8
 * C/C++ source code generated on  : 11-Sep-2015 21:01:56
 */

/* Include Files */
#include "gp_RPSSVEP.h"
#include "gp_pwelch.h"
#include "gp_RPSSVEP_emxutil.h"
#include "rdivide.h"
#include "abs.h"

/* Function Declarations */
static void eml_fft(const double x_data[], int n, emxArray_creal_T *y);

/* Function Definitions */

/*
 * Arguments    : const double x_data[]
 *                int n
 *                emxArray_creal_T *y
 * Return Type  : void
 */
static void eml_fft(const double x_data[], int n, emxArray_creal_T *y)
{
  int nd2;
  int minval;
  emxArray_real_T *costab1q;
  int nRowsD2;
  int nRowsD4;
  double e;
  int k;
  emxArray_real_T *costab;
  emxArray_real_T *sintab;
  int b_n;
  int n2;
  int ix;
  int i;
  boolean_T tst;
  double temp_re;
  double temp_im;
  int istart;
  int j;
  double twid_im;
  int ihi;
  nd2 = y->size[0];
  y->size[0] = n;
  emxEnsureCapacity((emxArray__common *)y, nd2, (int)sizeof(creal_T));
  if (n > 256) {
    nd2 = y->size[0];
    y->size[0] = n;
    emxEnsureCapacity((emxArray__common *)y, nd2, (int)sizeof(creal_T));
    for (nd2 = 0; nd2 < n; nd2++) {
      y->data[nd2].re = 0.0;
      y->data[nd2].im = 0.0;
    }
  }

  if (256 <= n) {
    minval = 255;
  } else {
    minval = n - 1;
  }

  emxInit_real_T(&costab1q, 2);
  nRowsD2 = (n + (n < 0)) >> 1;
  nRowsD4 = (nRowsD2 + (nRowsD2 < 0)) >> 1;
  e = 6.2831853071795862 / (double)n;
  nd2 = costab1q->size[0] * costab1q->size[1];
  costab1q->size[0] = 1;
  costab1q->size[1] = nRowsD4 + 1;
  emxEnsureCapacity((emxArray__common *)costab1q, nd2, (int)sizeof(double));
  costab1q->data[0] = 1.0;
  nd2 = (nRowsD4 + (nRowsD4 < 0)) >> 1;
  for (k = 1; k <= nd2; k++) {
    costab1q->data[k] = cos(e * (double)k);
  }

  for (k = nd2 + 1; k < nRowsD4; k++) {
    costab1q->data[k] = sin(e * (double)(nRowsD4 - k));
  }

  emxInit_real_T(&costab, 2);
  emxInit_real_T(&sintab, 2);
  costab1q->data[nRowsD4] = 0.0;
  b_n = costab1q->size[1] - 1;
  n2 = (costab1q->size[1] - 1) << 1;
  nd2 = costab->size[0] * costab->size[1];
  costab->size[0] = 1;
  costab->size[1] = n2 + 1;
  emxEnsureCapacity((emxArray__common *)costab, nd2, (int)sizeof(double));
  nd2 = sintab->size[0] * sintab->size[1];
  sintab->size[0] = 1;
  sintab->size[1] = n2 + 1;
  emxEnsureCapacity((emxArray__common *)sintab, nd2, (int)sizeof(double));
  costab->data[0] = 1.0;
  sintab->data[0] = 0.0;
  for (k = 1; k <= b_n; k++) {
    costab->data[k] = costab1q->data[k];
    sintab->data[k] = -costab1q->data[b_n - k];
  }

  for (k = costab1q->size[1]; k <= n2; k++) {
    costab->data[k] = -costab1q->data[n2 - k];
    sintab->data[k] = -costab1q->data[k - b_n];
  }

  emxFree_real_T(&costab1q);
  ix = 0;
  nd2 = 0;
  while ((n > 0) && (nd2 <= 0)) {
    n2 = 0;
    nd2 = 0;
    for (i = 1; i <= minval; i++) {
      y->data[nd2].re = x_data[ix];
      y->data[nd2].im = 0.0;
      b_n = n;
      tst = true;
      while (tst) {
        b_n >>= 1;
        n2 ^= b_n;
        tst = ((n2 & b_n) == 0);
      }

      nd2 = n2;
      ix++;
    }

    y->data[nd2].re = x_data[ix];
    y->data[nd2].im = 0.0;
    ix = (ix - minval) + 256;
    if (n > 1) {
      for (i = 0; i <= n - 2; i += 2) {
        temp_re = y->data[i + 1].re;
        temp_im = y->data[i + 1].im;
        y->data[i + 1].re = y->data[i].re - y->data[i + 1].re;
        y->data[i + 1].im = y->data[i].im - y->data[i + 1].im;
        y->data[i].re += temp_re;
        y->data[i].im += temp_im;
      }
    }

    nd2 = 2;
    n2 = 4;
    k = nRowsD4;
    b_n = 1 + ((nRowsD4 - 1) << 2);
    while (k > 0) {
      for (i = 0; i < b_n; i += n2) {
        temp_re = y->data[i + nd2].re;
        temp_im = y->data[i + nd2].im;
        y->data[i + nd2].re = y->data[i].re - temp_re;
        y->data[i + nd2].im = y->data[i].im - temp_im;
        y->data[i].re += temp_re;
        y->data[i].im += temp_im;
      }

      istart = 1;
      for (j = k; j < nRowsD2; j += k) {
        e = costab->data[j];
        twid_im = sintab->data[j];
        i = istart;
        ihi = istart + b_n;
        while (i < ihi) {
          temp_re = e * y->data[i + nd2].re - twid_im * y->data[i + nd2].im;
          temp_im = e * y->data[i + nd2].im + twid_im * y->data[i + nd2].re;
          y->data[i + nd2].re = y->data[i].re - temp_re;
          y->data[i + nd2].im = y->data[i].im - temp_im;
          y->data[i].re += temp_re;
          y->data[i].im += temp_im;
          i += n2;
        }

        istart++;
      }

      k >>= 1;
      nd2 = n2;
      n2 <<= 1;
      b_n -= nd2;
    }

    nd2 = n;
  }

  emxFree_real_T(&sintab);
  emxFree_real_T(&costab);
}

/*
 * Funçao que implementa o método do periodograma de welch
 *  Gpires 2015
 *  x: dados de entrada
 *  win: janela que multiplica pelo segmento de dados
 *  overlap: sobreposicao de segmentos (em nr de amostras)
 *  NFFT: número de pontos da FFT
 *  fs: frequencia de amostragem
 * Arguments    : const double x[512]
 *                const double win_data[]
 *                double overlap
 *                double NFFT
 *                double fs
 *                emxArray_real_T *Psd
 *                emxArray_real_T *f
 * Return Type  : void
 */
void gp_pwelch(const double x[512], const double win_data[], double overlap,
               double NFFT, double fs, emxArray_real_T *Psd, emxArray_real_T *f)
{
  double K;
  double d0;
  int i0;
  emxArray_real_T *Sxx;
  int nm1d2;
  double kd;
  int n;
  double apnd;
  double ndbl;
  double cdiff;
  double absb;
  emxArray_real_T *xStart;
  int k;
  emxArray_real_T *xEnd;
  emxArray_creal_T *xFFT;
  emxArray_real_T *r0;
  emxArray_real_T *r1;
  emxArray_creal_T *b_y1;
  emxArray_real_T *r2;
  double x_data[256];
  emxArray_real_T *b_Sxx;
  emxArray_real_T *c_Sxx;
  emxArray_real_T *d_Sxx;

  /* 'gp_pwelch:10' coder.inline('never'); */
  /* tamanho dos dados de entrada */
  /* 'gp_pwelch:13' M = length(x); */
  /* tamanho dos segmentos  */
  /* 'gp_pwelch:15' L = length(win); */
  /*  número de segmentos  */
  /* 'gp_pwelch:18' K = (M-overlap)/(L-overlap); */
  /*  arredonda ao valor inferior */
  /* 'gp_pwelch:20' K = fix(K); */
  K = (512.0 - overlap) / (256.0 - overlap);
  if (K < 0.0) {
    K = ceil(K);
  } else {
    K = floor(K);
  }

  /* normalização com janela usada */
  /* 'gp_pwelch:23' U = win'*win; */
  d0 = 0.0;
  for (i0 = 0; i0 < 256; i0++) {
    d0 += win_data[i0] * win_data[i0];
  }

  emxInit_real_T(&Sxx, 2);

  /* alocação de memoria    */
  /* 'gp_pwelch:26' xs = zeros(1,L); */
  /* 'gp_pwelch:27' xFFT = zeros(1,NFFT); */
  /* 'gp_pwelch:28' Pxk = zeros(1,NFFT); */
  /* 'gp_pwelch:29' Sxx = zeros(1,NFFT); */
  i0 = Sxx->size[0] * Sxx->size[1];
  Sxx->size[0] = 1;
  Sxx->size[1] = (int)NFFT;
  emxEnsureCapacity((emxArray__common *)Sxx, i0, (int)sizeof(double));
  nm1d2 = (int)NFFT;
  for (i0 = 0; i0 < nm1d2; i0++) {
    Sxx->data[i0] = 0.0;
  }

  /* 'gp_pwelch:30' Psd = zeros(1,NFFT/2); */
  /* 'gp_pwelch:31' f= zeros(1,NFFT/2); */
  /* segmento não sobreposto */
  /* 'gp_pwelch:34' LminusOverlap = L-overlap; */
  /* vetor com inicios de segmento */
  /* 'gp_pwelch:36' xStart = 1:LminusOverlap:K*LminusOverlap; */
  kd = K * (256.0 - overlap);
  if ((256.0 - overlap == 0.0) || ((1.0 < kd) && (256.0 - overlap < 0.0)) ||
      ((kd < 1.0) && (256.0 - overlap > 0.0))) {
    n = -1;
    apnd = kd;
  } else {
    ndbl = floor((kd - 1.0) / (256.0 - overlap) + 0.5);
    apnd = 1.0 + ndbl * (256.0 - overlap);
    if (256.0 - overlap > 0.0) {
      cdiff = apnd - kd;
    } else {
      cdiff = kd - apnd;
    }

    absb = fabs(kd);
    if (1.0 >= absb) {
      absb = 1.0;
    }

    if (fabs(cdiff) < 4.4408920985006262E-16 * absb) {
      ndbl++;
      apnd = kd;
    } else if (cdiff > 0.0) {
      apnd = 1.0 + (ndbl - 1.0) * (256.0 - overlap);
    } else {
      ndbl++;
    }

    if (ndbl >= 0.0) {
      n = (int)ndbl - 1;
    } else {
      n = -1;
    }
  }

  emxInit_real_T(&xStart, 2);
  i0 = xStart->size[0] * xStart->size[1];
  xStart->size[0] = 1;
  xStart->size[1] = n + 1;
  emxEnsureCapacity((emxArray__common *)xStart, i0, (int)sizeof(double));
  if (n + 1 > 0) {
    xStart->data[0] = 1.0;
    if (n + 1 > 1) {
      xStart->data[n] = apnd;
      nm1d2 = (n + (n < 0)) >> 1;
      for (k = 1; k < nm1d2; k++) {
        kd = (double)k * (256.0 - overlap);
        xStart->data[k] = 1.0 + kd;
        xStart->data[n - k] = apnd - kd;
      }

      if (nm1d2 << 1 == n) {
        xStart->data[nm1d2] = (1.0 + apnd) / 2.0;
      } else {
        kd = (double)nm1d2 * (256.0 - overlap);
        xStart->data[nm1d2] = 1.0 + kd;
        xStart->data[nm1d2 + 1] = apnd - kd;
      }
    }
  }

  emxInit_real_T(&xEnd, 2);

  /* vetor com fins de segmento   */
  /* 'gp_pwelch:38' xEnd   = xStart+L-1; */
  i0 = xEnd->size[0] * xEnd->size[1];
  xEnd->size[0] = 1;
  xEnd->size[1] = xStart->size[1];
  emxEnsureCapacity((emxArray__common *)xEnd, i0, (int)sizeof(double));
  nm1d2 = xStart->size[0] * xStart->size[1];
  for (i0 = 0; i0 < nm1d2; i0++) {
    xEnd->data[i0] = (xStart->data[i0] + 256.0) - 1.0;
  }

  /* 'gp_pwelch:40' for i=1:K */
  n = 0;
  emxInit_creal_T(&xFFT, 2);
  emxInit_real_T(&r0, 2);
  emxInit_real_T(&r1, 2);
  b_emxInit_creal_T(&b_y1, 1);
  emxInit_real_T(&r2, 2);
  while (n <= (int)K - 1) {
    /* size(x(xStart(i):xEnd(i)))  */
    /* 'gp_pwelch:42' xs=x(xStart(i):xEnd(i)).*win'; */
    if (xStart->data[n] > xEnd->data[n]) {
      i0 = 0;
      k = 0;
    } else {
      i0 = (int)xStart->data[n] - 1;
      k = (int)xEnd->data[n];
    }

    /* 'gp_pwelch:43' xFFT = fft(xs,NFFT); */
    nm1d2 = k - i0;
    for (k = 0; k < nm1d2; k++) {
      x_data[k] = x[i0 + k] * win_data[k];
    }

    eml_fft(x_data, (int)NFFT, b_y1);
    i0 = xFFT->size[0] * xFFT->size[1];
    xFFT->size[0] = 1;
    xFFT->size[1] = b_y1->size[0];
    emxEnsureCapacity((emxArray__common *)xFFT, i0, (int)sizeof(creal_T));
    nm1d2 = b_y1->size[0];
    for (i0 = 0; i0 < nm1d2; i0++) {
      xFFT->data[xFFT->size[0] * i0] = b_y1->data[i0];
    }

    /* potencia */
    /* Pxk= xFFT.*conj(xFFT) / U;      */
    /* 'gp_pwelch:46' Pxk= abs(xFFT).*abs(xFFT) / U; */
    b_abs(xFFT, r0);
    b_abs(xFFT, r1);

    /* 'gp_pwelch:47' Sxx=Sxx+Pxk; */
    i0 = r2->size[0] * r2->size[1];
    r2->size[0] = 1;
    r2->size[1] = r0->size[1];
    emxEnsureCapacity((emxArray__common *)r2, i0, (int)sizeof(double));
    nm1d2 = r0->size[0] * r0->size[1];
    for (i0 = 0; i0 < nm1d2; i0++) {
      r2->data[i0] = r0->data[i0] * r1->data[i0];
    }

    rdivide(r2, d0, r0);
    i0 = Sxx->size[0] * Sxx->size[1];
    Sxx->size[0] = 1;
    emxEnsureCapacity((emxArray__common *)Sxx, i0, (int)sizeof(double));
    nm1d2 = Sxx->size[0];
    k = Sxx->size[1];
    nm1d2 *= k;
    for (i0 = 0; i0 < nm1d2; i0++) {
      Sxx->data[i0] += r0->data[i0];
    }

    n++;
  }

  emxFree_real_T(&r2);
  emxFree_creal_T(&b_y1);
  emxFree_real_T(&r1);
  emxFree_real_T(&r0);
  emxFree_creal_T(&xFFT);
  emxFree_real_T(&xEnd);
  emxFree_real_T(&xStart);
  emxInit_real_T(&b_Sxx, 2);

  /* média das potências */
  /* 'gp_pwelch:50' Sxx = Sxx./K; */
  i0 = b_Sxx->size[0] * b_Sxx->size[1];
  b_Sxx->size[0] = 1;
  b_Sxx->size[1] = Sxx->size[1];
  emxEnsureCapacity((emxArray__common *)b_Sxx, i0, (int)sizeof(double));
  nm1d2 = Sxx->size[0] * Sxx->size[1];
  for (i0 = 0; i0 < nm1d2; i0++) {
    b_Sxx->data[i0] = Sxx->data[i0];
  }

  rdivide(b_Sxx, K, Sxx);

  /* calculo da densidade espectral de potencia unilateral (considerando potencia do lado negativo e positivo e ) */
  /* 'gp_pwelch:52' Psd = [Sxx(1) 2*Sxx(2:NFFT/2)]./fs; */
  d0 = NFFT / 2.0;
  emxFree_real_T(&b_Sxx);
  if (2.0 > d0) {
    i0 = -1;
    k = 0;
  } else {
    i0 = 0;
    k = (int)d0;
  }

  emxInit_real_T(&c_Sxx, 2);
  nm1d2 = c_Sxx->size[0] * c_Sxx->size[1];
  c_Sxx->size[0] = 1;
  c_Sxx->size[1] = k - i0;
  emxEnsureCapacity((emxArray__common *)c_Sxx, nm1d2, (int)sizeof(double));
  c_Sxx->data[0] = Sxx->data[0];
  nm1d2 = k - i0;
  for (k = 0; k <= nm1d2 - 2; k++) {
    c_Sxx->data[c_Sxx->size[0] * (k + 1)] = 2.0 * Sxx->data[(i0 + k) + 1];
  }

  rdivide(c_Sxx, fs, Psd);

  /* OneSidePsd=[Psd(1) 2*Psd(1:NFF-1)];   %densidade espectral de potencia unilateral  */
  /* vetor de frequencias */
  /* 'gp_pwelch:56' f=[0:NFFT/2-1]*fs/NFFT; */
  kd = NFFT / 2.0;
  emxFree_real_T(&c_Sxx);
  if (kd - 1.0 < 0.0) {
    n = -1;
    apnd = kd - 1.0;
  } else {
    ndbl = floor((NFFT / 2.0 - 1.0) + 0.5);
    apnd = ndbl;
    cdiff = ndbl - (kd - 1.0);
    if (fabs(cdiff) < 4.4408920985006262E-16 * fabs(kd - 1.0)) {
      ndbl++;
      apnd = kd - 1.0;
    } else if (cdiff > 0.0) {
      apnd = ndbl - 1.0;
    } else {
      ndbl++;
    }

    n = (int)ndbl - 1;
  }

  i0 = Sxx->size[0] * Sxx->size[1];
  Sxx->size[0] = 1;
  Sxx->size[1] = n + 1;
  emxEnsureCapacity((emxArray__common *)Sxx, i0, (int)sizeof(double));
  if (n + 1 > 0) {
    Sxx->data[0] = 0.0;
    if (n + 1 > 1) {
      Sxx->data[n] = apnd;
      nm1d2 = (n + (n < 0)) >> 1;
      for (k = 1; k < nm1d2; k++) {
        Sxx->data[k] = k;
        Sxx->data[n - k] = apnd - (double)k;
      }

      if (nm1d2 << 1 == n) {
        Sxx->data[nm1d2] = apnd / 2.0;
      } else {
        Sxx->data[nm1d2] = nm1d2;
        Sxx->data[nm1d2 + 1] = apnd - (double)nm1d2;
      }
    }
  }

  emxInit_real_T(&d_Sxx, 2);
  i0 = d_Sxx->size[0] * d_Sxx->size[1];
  d_Sxx->size[0] = 1;
  d_Sxx->size[1] = Sxx->size[1];
  emxEnsureCapacity((emxArray__common *)d_Sxx, i0, (int)sizeof(double));
  nm1d2 = Sxx->size[0] * Sxx->size[1];
  for (i0 = 0; i0 < nm1d2; i0++) {
    d_Sxx->data[i0] = Sxx->data[i0] * fs;
  }

  emxFree_real_T(&Sxx);
  rdivide(d_Sxx, NFFT, f);
  emxFree_real_T(&d_Sxx);
}

/*
 * File trailer for gp_pwelch.c
 *
 * [EOF]
 */
