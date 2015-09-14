/*
 * File: gp_RPSSVEP.c
 *
 * MATLAB Coder version            : 2.8
 * C/C++ source code generated on  : 11-Sep-2015 21:01:56
 */

/* Include Files */
#include "gp_RPSSVEP.h"
#include "gp_pwelch.h"
#include "gp_RPSSVEP_emxutil.h"

/* Function Definitions */

/*
 * Algoritmo que deteta o alvo (seta ou checkerboard) com base na potência relativa evocada do sinal SSVEP
 *  Gpires 2015
 *  yr: sinal de referencia sem estimulo (dimensao: NcanaisxTsamples)
 *  ys: sinal SSVEP de resposta a estimululação (dimensao: NcanaisxTsamples)
 *  f_interest: frequencias de interesse (frequencia do estimulo e respetivo harmónico)
 *  canais: canais usados no algoritmo
 *  win_type: tipo de janela usada no pwelch
 *  overlap: sobreposição do segmento usado no pwelch
 *  NFFT: nr de pontos a usar na FFT do pwelch
 *  fs: frequencia de amostragem
 *  class_type: 1 (Ps - apenas potencia de ys); 2 (Ganho SNR: relação entre Ps e Pr (potencia do sinal e Potencia de baseline))
 * Arguments    : double yr
 *                const double ys[4096]
 *                const double f_interest[6]
 *                const double canais[4]
 *                double win_type
 *                double overlap
 *                double NFFT
 *                double fs
 *                double class_type
 *                double GTargets[3]
 *                double *idx
 * Return Type  : void
 */
void gp_RPSSVEP(double yr, const double ys[4096], const double f_interest[6],
                const double canais[4], double win_type, double overlap, double
                NFFT, double fs, double class_type, double GTargets[3], double
                *idx)
{
  emxArray_real_T *Pc;
  double s;
  int ixstart;
  int iy;
  emxArray_real_T *Ps;
  double Ps_harm[12];
  emxArray_real_T *win;
  static const double dv0[256] = { 0.080000000000000016, 0.0801396320901791,
    0.080558443590626638, 0.0812561802425375, 0.082232418452750367,
    0.083486565550909553, 0.085017860149272872, 0.086825372604947,
    0.088908005584270255, 0.091264494728999168, 0.093893409423895624,
    0.096793153665247822, 0.0999619670297972, 0.10339792574348544,
    0.10709894384936913, 0.11106277447399632, 0.11528701119147489,
    0.11976908948440362, 0.12450628830078075, 0.12949573170594403,
    0.13473439062853915, 0.140219084699457, 0.14594648418262318,
    0.15191311199646745, 0.15811534582484671, 0.164549420316138,
    0.17121142936916933, 0.17809732850459775, 0.18520293732029686,
    0.19252394202926287, 0.20005589807849683, 0.20779423284727572,
    0.21573424842317235, 0.2238711244541392, 0.23219992107492521,
    0.24071558190604869, 0.24941293712350454, 0.25828670659734504,
    0.26733150309722475, 0.27654183556296763, 0.28591211243816733,
    0.29543664506479939, 0.30510965113678279, 0.31492525821039519,
    0.32487750726941012, 0.33496035634279242, 0.34516768417275479,
    0.35549329393094942, 0.36593091698053726, 0.37647421668185377,
    0.38711679223935658, 0.39785218258752419, 0.40867387031334262,
    0.4195752856130015, 0.43054981028039535, 0.44159078172501126,
    0.45269149701676009, 0.46384521695530023, 0.47504517016137793,
    0.48628455718770613, 0.49755655464688109, 0.50885431935383452,
    0.520170992480304, 0.53149970371880029, 0.54283357545354383,
    0.55416572693583832, 0.56548927846134622, 0.57679735554672917,
    0.58808309310312057, 0.59933963960389347, 0.61056016124419532,
    0.6217378460897236, 0.63286590821222466, 0.64393759180920385,
    0.65494617530534627, 0.66588497543315817, 0.67674735129035324,
    0.6875267083715173, 0.698216502571607, 0.70881024415884919,
    0.71930150171463247, 0.7296839060379956, 0.7399511540123439,
    0.75009701243204674, 0.7601153217865918, 0.76999999999999991,
    0.77974504612322926, 0.78934454397732678, 0.79879266574511876,
    0.808083675509254, 0.817211932734458, 0.826171895691878, 0.834958124823446,
    0.84356528604421233, 0.85198815398064776, 0.86022161514294626,
    0.86826067102940607, 0.87610044116099883, 0.88373616604429,
    0.89116321006090748, 0.89837706428180852, 0.90537334920463164,
    0.91214781741247575, 0.91869635615249012, 0.9250149898327098,
    0.93109988243562247, 0.93694733984699929, 0.94255381209857669,
    0.94791589552322852, 0.95303033482131871, 0.957894025036981,
    0.96250401344312464, 0.96685750133402437, 0.97095184572440285,
    0.974784560953977, 0.978353320196492, 0.981655956872329, 0.98469046596382626,
    0.98745500523251828, 0.98994789633755065, 0.99216762585459484,
    0.9941128461946418, 0.99578237642211787, 0.99717520297182594,
    0.99829048026427758, 0.99912753121904041, 0.99968584766579061,
    0.99996509065282213, 0.99996509065282213, 0.99968584766579061,
    0.99912753121904041, 0.99829048026427758, 0.99717520297182594,
    0.99578237642211787, 0.9941128461946418, 0.99216762585459484,
    0.98994789633755065, 0.98745500523251828, 0.98469046596382626,
    0.981655956872329, 0.978353320196492, 0.974784560953977, 0.97095184572440285,
    0.96685750133402437, 0.96250401344312464, 0.957894025036981,
    0.95303033482131871, 0.94791589552322852, 0.94255381209857669,
    0.93694733984699929, 0.93109988243562247, 0.9250149898327098,
    0.91869635615249012, 0.91214781741247575, 0.90537334920463164,
    0.89837706428180852, 0.89116321006090748, 0.88373616604429,
    0.87610044116099883, 0.86826067102940607, 0.86022161514294626,
    0.85198815398064776, 0.84356528604421233, 0.834958124823446,
    0.826171895691878, 0.817211932734458, 0.808083675509254, 0.79879266574511876,
    0.78934454397732678, 0.77974504612322926, 0.76999999999999991,
    0.7601153217865918, 0.75009701243204674, 0.7399511540123439,
    0.7296839060379956, 0.71930150171463247, 0.70881024415884919,
    0.698216502571607, 0.6875267083715173, 0.67674735129035324,
    0.66588497543315817, 0.65494617530534627, 0.64393759180920385,
    0.63286590821222466, 0.6217378460897236, 0.61056016124419532,
    0.59933963960389347, 0.58808309310312057, 0.57679735554672917,
    0.56548927846134622, 0.55416572693583832, 0.54283357545354383,
    0.53149970371880029, 0.520170992480304, 0.50885431935383452,
    0.49755655464688109, 0.48628455718770613, 0.47504517016137793,
    0.46384521695530023, 0.45269149701676009, 0.44159078172501126,
    0.43054981028039535, 0.4195752856130015, 0.40867387031334262,
    0.39785218258752419, 0.38711679223935658, 0.37647421668185377,
    0.36593091698053726, 0.35549329393094942, 0.34516768417275479,
    0.33496035634279242, 0.32487750726941012, 0.31492525821039519,
    0.30510965113678279, 0.29543664506479939, 0.28591211243816733,
    0.27654183556296763, 0.26733150309722475, 0.25828670659734504,
    0.24941293712350454, 0.24071558190604869, 0.23219992107492521,
    0.2238711244541392, 0.21573424842317235, 0.20779423284727572,
    0.20005589807849683, 0.19252394202926287, 0.18520293732029686,
    0.17809732850459775, 0.17121142936916933, 0.164549420316138,
    0.15811534582484671, 0.15191311199646745, 0.14594648418262318,
    0.140219084699457, 0.13473439062853915, 0.12949573170594403,
    0.12450628830078075, 0.11976908948440362, 0.11528701119147489,
    0.11106277447399632, 0.10709894384936913, 0.10339792574348544,
    0.0999619670297972, 0.096793153665247822, 0.093893409423895624,
    0.091264494728999168, 0.088908005584270255, 0.086825372604947,
    0.085017860149272872, 0.083486565550909553, 0.082232418452750367,
    0.0812561802425375, 0.080558443590626638, 0.0801396320901791,
    0.080000000000000016 };

  int k;
  emxArray_real_T *P;
  emxArray_real_T *f;
  int i;
  double b_ys[512];
  int ix;
  (void)yr;
  (void)win_type;
  emxInit_real_T(&Pc, 2);

  /* alocação de memoria para os parâmetros de entrada */
  /* inputs */
  /* 'gp_RPSSVEP:16' N=length(canais); */
  /* Nr de targets (assume sempre que é colocado o harmonico) */
  /* 'gp_RPSSVEP:18' Ntargets=length(f_interest)/2; */
  /* 'gp_RPSSVEP:19' Pc=zeros(N,NFFT/2); */
  s = NFFT / 2.0;
  ixstart = Pc->size[0] * Pc->size[1];
  Pc->size[0] = 4;
  Pc->size[1] = (int)s;
  emxEnsureCapacity((emxArray__common *)Pc, ixstart, (int)sizeof(double));
  iy = (int)s << 2;
  for (ixstart = 0; ixstart < iy; ixstart++) {
    Pc->data[ixstart] = 0.0;
  }

  emxInit_real_T(&Ps, 2);

  /* 'gp_RPSSVEP:20' Ps=zeros(N,NFFT/2); */
  s = NFFT / 2.0;
  ixstart = Ps->size[0] * Ps->size[1];
  Ps->size[0] = 4;
  Ps->size[1] = (int)s;
  emxEnsureCapacity((emxArray__common *)Ps, ixstart, (int)sizeof(double));
  iy = (int)s << 2;
  for (ixstart = 0; ixstart < iy; ixstart++) {
    Ps->data[ixstart] = 0.0;
  }

  /* length(f_interest)); */
  /* 'gp_RPSSVEP:21' Ps_harm=zeros(N,Ntargets); */
  memset(&Ps_harm[0], 0, 12U * sizeof(double));

  /* 'gp_RPSSVEP:22' win=zeros(fs,1); */
  /* outputs */
  /* 'gp_RPSSVEP:25' GTargets=zeros(1,Ntargets); */
  for (ixstart = 0; ixstart < 3; ixstart++) {
    GTargets[ixstart] = 0.0;
  }

  b_emxInit_real_T(&win, 1);

  /* 'gp_RPSSVEP:26' idx=0; */
  *idx = 0.0;

  /* escolha da janela para pwelch (terá sempre o tamanho de 1 seg, ou seja, fs amostras) */
  /* default: janela de hamming */
  /* 'gp_RPSSVEP:30' win_type=2; */
  /* 'gp_RPSSVEP:31' switch win_type */
  /* 'gp_RPSSVEP:34' case 2 */
  /* 'gp_RPSSVEP:35' win=hamming(256); */
  ixstart = win->size[0];
  win->size[0] = 256;
  emxEnsureCapacity((emxArray__common *)win, ixstart, (int)sizeof(double));
  for (ixstart = 0; ixstart < 256; ixstart++) {
    win->data[ixstart] = dv0[ixstart];
  }

  /* janela de hamming */
  /* 'gp_RPSSVEP:40' k=0; */
  k = -1;

  /* 'gp_RPSSVEP:41' if class_type==1 */
  if (class_type == 1.0) {
    /* lista de canais selecionados */
    /* 'gp_RPSSVEP:43' for i=canais */
    emxInit_real_T(&P, 2);
    emxInit_real_T(&f, 2);
    for (i = 0; i < 4; i++) {
      /* i */
      /* indice continuo para as matrizes associadas aos canais */
      /* 'gp_RPSSVEP:46' k=k+1; */
      k++;

      /* 'gp_RPSSVEP:47' [P f] = gp_pwelch(ys(i,:),win,overlap,NFFT,fs); */
      for (ixstart = 0; ixstart < 512; ixstart++) {
        b_ys[ixstart] = ys[((int)canais[i] + (ixstart << 3)) - 1];
      }

      gp_pwelch(b_ys, win->data, overlap, NFFT, fs, P, f);

      /* 'gp_RPSSVEP:48' Pc(k,:) = P; */
      iy = P->size[1];
      for (ixstart = 0; ixstart < iy; ixstart++) {
        Pc->data[k + Pc->size[0] * ixstart] = P->data[P->size[0] * ixstart];
      }

      /* 'gp_RPSSVEP:49' for  fi=f_interest */
      for (ixstart = 0; ixstart < 6; ixstart++) {
        /* 'gp_RPSSVEP:50' Ps(k,fi)=Pc(k,fi)/(Pc(k,fi-1)+Pc(k,fi)+Pc(k,fi+1)); */
        Ps->data[k + Ps->size[0] * ((int)f_interest[ixstart] - 1)] = Pc->data[k
          + Pc->size[0] * ((int)f_interest[ixstart] - 1)] / ((Pc->data[k +
          Pc->size[0] * ((int)(f_interest[ixstart] - 1.0) - 1)] + Pc->data[k +
          Pc->size[0] * ((int)f_interest[ixstart] - 1)]) + Pc->data[k + Pc->
          size[0] * ((int)(f_interest[ixstart] + 1.0) - 1)]);
      }

      /* soma os harmonicos */
      /* 'gp_RPSSVEP:54' for j=1:Ntargets */
      for (ixstart = 0; ixstart < 3; ixstart++) {
        /* 'gp_RPSSVEP:55' Ps_harm(k,j)=sum( Ps(k,f_interest([2*j-1:2*j])), 2); */
        Ps_harm[k + (ixstart << 2)] = Ps->data[k + Ps->size[0] * ((int)
          f_interest[((1 + ixstart) << 1) - 2] - 1)] + Ps->data[k + Ps->size[0] *
          ((int)f_interest[-1 + ((1 + ixstart) << 1)] - 1)];
      }
    }

    emxFree_real_T(&f);
    emxFree_real_T(&P);

    /* soma potencia dos vários canais e devolve potencia para os targets */
    /* 'gp_RPSSVEP:59' GTargets=sum(Ps_harm,1); */
    ix = 0;
    iy = -1;
    for (i = 0; i < 3; i++) {
      ixstart = ix;
      ix++;
      s = Ps_harm[ixstart];
      for (k = 0; k < 3; k++) {
        ix++;
        s += Ps_harm[ix - 1];
      }

      iy++;
      GTargets[iy] = s;
    }

    /* idx devolve o indice do target com maior potência */
    /* 'gp_RPSSVEP:61' [v idx]=max(GTargets); */
    s = GTargets[0];
    ixstart = -1;
    for (ix = 0; ix < 2; ix++) {
      if (GTargets[ix + 1] > s) {
        s = GTargets[ix + 1];
        ixstart = ix;
      }
    }

    *idx = (double)ixstart + 2.0;
  }

  emxFree_real_T(&win);
  emxFree_real_T(&Ps);
  emxFree_real_T(&Pc);

  /* toc */
  /*   */
  /*  if mode==2 */
  /*  Pr=zeros(N,length(f_interest));     */
  /*  %potencia com estimulaçao */
  /*  for i=1:length(canais)    %channels */
  /*      [P f] = pwelch(ys(i,:),256,128,NFFT,fs); */
  /*      Pc(i,:)=P; */
  /*      for  fi=f_interest */
  /*          Ps(i,fi)=Pc(i,fi)/(Pc(i,fi-1)+Pc(i,fi)+Pc(i,fi+1)); */
  /*      end */
  /*      for j=1:Ntargets */
  /*           auxT(i,j)=sum( Ps(i,f_interest([2*j-1:2*j])), 2); %soma com harmonico  */
  /*      end */
  /*  end */
  /*  PsTargets=sum(auxT,1); */
  /*   */
  /*  %potencia SEM estimulaçao (rest) */
  /*  for i=1:length(canais)    %channels */
  /*      [P f] = pwelch(yr(i,:),256,128,NFFT,fs); */
  /*      Pc(i,:)=P; */
  /*      for  fi=f_interest */
  /*          Pr(i,fi)=Pc(i,fi)/(Pc(i,fi-1)+Pc(i,fi)+Pc(i,fi+1)); */
  /*      end */
  /*      for j=1:Ntargets */
  /*           auxT(i,j)=sum( Pr(i,f_interest([2*j-1:2*j])), 2); %soma com harmonico  */
  /*        end */
  /*  end */
  /*  PrTargets=sum(auxT,1); */
  /*   */
  /*  GTargets=PsTargets./PrTargets; */
  /*  end     */
}

/*
 * File trailer for gp_RPSSVEP.c
 *
 * [EOF]
 */
