using UnityEngine;
using System.Collections;
using System.Threading;
using System.Timers;
using System;

public class EsquerdaEstimulo : Estimulo{
	public GUITexture Seta2Esq;
	public GUITexture Seta1Esq;


	void Start(){
		base.setTextures (Seta1Esq, Seta2Esq);
		if (FreqEscolhe.DefFreqManual == true) {
			base.frequencia = FreqEscolhe.FreqEsqVal;
		} else {
			frequencia = 9;
		}
		base.Run();
	}
}
