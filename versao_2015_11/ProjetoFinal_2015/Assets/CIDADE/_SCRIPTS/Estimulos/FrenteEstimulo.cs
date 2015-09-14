using UnityEngine;
using System.Collections;
using System.Threading;
using System.Timers;
using System;

public class FrenteEstimulo : Estimulo{
	public GUITexture Seta1Frt;
	public GUITexture Seta2Frt;
	public Texture[] GUIEstFrt;
	
	void Start(){
		Seta1Frt.GetComponent<GUITexture>().texture = GUIEstFrt [EstimulosEscolha.Seta1Usada];
		Seta2Frt.GetComponent<GUITexture>().texture = GUIEstFrt [EstimulosEscolha.Seta2Usada];
		Seta1Frt.gameObject.SetActive (true);
		Seta2Frt.gameObject.SetActive (true);
		base.setTextures (Seta1Frt, Seta2Frt);
		if (FreqEscolhe.DefFreqManual == true) {
			base.frequencia = FreqEscolhe.FreqFrenteVal;
		} else {
			frequencia = 9;
		}
		base.Run();
	}

}
