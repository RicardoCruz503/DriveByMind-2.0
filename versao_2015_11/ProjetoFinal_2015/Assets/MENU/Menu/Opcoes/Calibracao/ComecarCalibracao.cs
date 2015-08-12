﻿using UnityEngine;
using System.Collections;

public class ComecarCalibracao : MonoBehaviour
{

		// Use this for initialization
		public GameObject Botao;
		public Texture[] GUIBegin_End;
		public bool Comecou = false;

		void Start ()
		{
	
		}
	
		// Update is called once per frame
		void Update ()
		{
	
		}

		void OnMouseDown ()
		{
				if (CalibracaoTCP.conectado == true) {
						Comecou = !Comecou;
						if (Comecou == true) {
								GameObject.Find ("Main Camera").GetComponent<Calibracao> ().enabled = true;
								GameObject.Find ("MenuPref").GetComponent<PiscarGUI> ().enabled = true;
								Botao.GetComponent<GUITexture>().texture = GUIBegin_End [0];
								this.GetComponent<GUIText>().text = "Parar";
						}
						if (Comecou == false) {
								GameObject.Find ("Main Camera").GetComponent<Calibracao> ().enabled = false;
								GameObject.Find ("MenuPref").GetComponent<PiscarGUI> ().enabled = false;
								PiscarGUI.progressoGUI = 0;
								Calibracao.progressoCalib = 0;
								Botao.GetComponent<GUITexture>().texture = GUIBegin_End [1];
								this.GetComponent<GUIText>().text = "Começar";
						}
				}
		}
}
