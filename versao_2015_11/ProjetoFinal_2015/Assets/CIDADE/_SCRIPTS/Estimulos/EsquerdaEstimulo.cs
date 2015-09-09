using UnityEngine;
using System.Collections;
using System.Threading;
using System.Timers;
using System;

public class EsquerdaEstimulo : Estimulo{
	public GUITexture Seta2Esq;
	public GUITexture Seta1Esq;
	public Texture[] GUIEstEsq;

	void Start(){
		Seta1Esq.GetComponent<GUITexture>().texture = GUIEstEsq [EstimulosEscolha.Seta1Usada];
		Seta2Esq.GetComponent<GUITexture>().texture = GUIEstEsq [EstimulosEscolha.Seta2Usada];
		Seta1Esq.gameObject.SetActive (true);
		Seta2Esq.gameObject.SetActive (true);
		base.setTextures (Seta1Esq, Seta2Esq);
		if (FreqEscolhe.DefFreqManual == true) {
			base.frequencia = FreqEscolhe.FreqEsqVal;
		} else {
			frequencia = 9;
		}
		base.Run();
	}
}
