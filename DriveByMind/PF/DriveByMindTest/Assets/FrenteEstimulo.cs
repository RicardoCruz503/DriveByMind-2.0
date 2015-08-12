using UnityEngine;
using System.Collections;
using System.Threading;
using System.Timers;
using System;

public class FrenteEstimulo : Estimulo{
	public GUITexture Seta1Frt;
	public GUITexture Seta2Frt;
	
	
	void Start(){
		base.setTextures (Seta1Frt, Seta2Frt);
		if (FreqEscolhe.DefFreqManual == true) {
			base.frequencia = FreqEscolhe.FreqFrenteVal;
		} else {
			frequencia = 9;
		}
		base.Run();
	}

}
