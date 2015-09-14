using UnityEngine;
using System.Collections;
using System.Threading;
using System.Timers;
using System;

public class DireitaEstimulo : Estimulo{
	public GUITexture Seta1Dir;
	public GUITexture Seta2Dir;
	public Texture[] GUIEstDir;
	
	void Start(){
		Seta1Dir.GetComponent<GUITexture>().texture = GUIEstDir [EstimulosEscolha.Seta1Usada];
		Seta2Dir.GetComponent<GUITexture>().texture = GUIEstDir [EstimulosEscolha.Seta2Usada];
		Seta1Dir.gameObject.SetActive (true);
		Seta2Dir.gameObject.SetActive (true);
		base.setTextures (Seta1Dir, Seta2Dir);
		
		if (FreqEscolhe.DefFreqManual == true) {
			base.frequencia = FreqEscolhe.FreqDirVal;
		} else {
			frequencia = 9;
		}
		base.Run();
	}
}
