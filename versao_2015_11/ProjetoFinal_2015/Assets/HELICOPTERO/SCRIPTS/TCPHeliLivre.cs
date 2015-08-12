﻿using UnityEngine;
using System.Collections;

public class TCPHeliLivre : MonoBehaviour
{		
	
		public Transform[] destino;
		public int Pontuacao = 0;
		public string comando;				 // contem a tecla que o utilizador escolheu
		public static float distancia;
		public GameObject Estimulos;
		bool EnviarMatLabModoJogo = true;
		bool Colisao = false;
		bool JogoAcabou = false;
		public Texture pauseGUI;
		bool clickMenuReiniciar = false;
		public GUIStyle TipoLetraFinal;
		int numeroColisao = 0; // Apenas faz a explosao quando bate a primeira vez em algo
		public GameObject explosao;
		float tempo = 0; // enviar comando ao MatLab 2s depois da conexao
		
		// Use this for initialization
		void Start ()
		{
		Pontuacao = 0;
		clickMenuReiniciar = false;
		EnviarMatLabModoJogo = true;
		Colisao = false;
		JogoAcabou = false;
		numeroColisao = 0;
		}
		
		// Update is called once per frame
		void Update ()
		{
		if (Tcpheli.conectado == true && Colisao == false) {
			tempo += Time.deltaTime;
						if (EnviarMatLabModoJogo == true && tempo>12) { // Cada vez que se perde ligaçao e retoma, envia um pedido de jogo modo2 (1 em 1s)
								EnviarMatLabModoJogo = false;
								//Tcpheli.mensagemMatLab = MatLab_Env_Comando.modo2Valor.ToString() + "1";
								Tcpheli.mensagemMatLab = "Q1";
								Tcpheli.EnviarComandoMatLabHeli = true;
								Estimulos.SetActive (true);
						}
						

					//	if (!Input.GetKey (KeyCode.UpArrow) && !Tcpheli.comand.Equals ("C")) {
								if (this.gameObject.transform.position.y > EscolherAneis.PosicaoYAneis) {
										this.transform.Translate (new Vector3 (0, -2 * Time.deltaTime, 0));
								}
					//	}
						this.transform.Translate (new Vector3 (0, 0, 5 * Time.deltaTime));
				
						// Se houver ligacao TCP com o jogo ele anda, senao fica parado

				//if (Tcpheli.comand.Equals (MatLab_Det_Setas.DirValor.ToString())) {
							if (Tcpheli.comand.Equals ("B")) {
										this.transform.Rotate (new Vector3 (0, 10 * Time.deltaTime, 0));	
								}
				//if (Tcpheli.comand.Equals (MatLab_Det_Setas.EsqValor.ToString())) {
							if (Tcpheli.comand.Equals ("A")) {
										this.transform.Rotate (new Vector3 (0, -10 * Time.deltaTime, 0));
								}
						
			//	if (Tcpheli.comand.Equals (MatLab_Det_Setas.FrenteValor.ToString())) {
							if (Tcpheli.comand.Equals ("C")) {
										this.transform.Translate (new Vector3 (0, 0, 0));
								}
						
				


						if (Input.GetKey (KeyCode.RightArrow)) {
								this.transform.Rotate (new Vector3 (0, 10 * Time.deltaTime, 0));	
						}
						if (Input.GetKey (KeyCode.LeftArrow)) {
								this.transform.Rotate (new Vector3 (0, -10 * Time.deltaTime, 0));
						}
		
						if (Input.GetKey (KeyCode.UpArrow)) {
								this.transform.Translate (new Vector3 (0, 2 * Time.deltaTime, 0));
						}

				} else {
						EnviarMatLabModoJogo = true;
						tempo = 0;
				}
				if (Colisao == true && JogoAcabou != true) {
						this.transform.Translate (new Vector3 (0, -10 * Time.deltaTime, 0));	
				} 
				if (clickMenuReiniciar == true) {
						Application.LoadLevel ("HelicopteroLivreTeclado");
				}
		}
		
		void  OnCollisionEnter (Collision collision)
		{
				Colisao = true;
				explosao.SetActive (true);
				if (numeroColisao == 0) {
						explosao.GetComponent<AudioSource>().Play ();
				}
				this.GetComponent<Rigidbody>().useGravity = true;
				this.GetComponent<AudioSource>().Stop ();
				if (collision.gameObject.name.Contains ("floor") || collision.gameObject.name.Contains ("Terrain") || collision.gameObject.name.Contains ("Street") || numeroColisao == 4) {
						JogoAcabou = true;
						this.GetComponent<RodarHelices> ().enabled = false;
			
						Time.timeScale = 0.0f;
				}
				numeroColisao++;
		}

		void OnGUI ()
		{
		
				if (JogoAcabou == true) {
						GUI.DrawTexture (new Rect (0, 0, Screen.width, Screen.height), pauseGUI);
						GUI.Label (new Rect ((Screen.width / 4), (Screen.height / 10), 2 * Screen.width / 4, Screen.height / 8), "FIM DO JOGO", TipoLetraFinal);
						GUI.Label (new Rect ((Screen.width / 8), (3 * Screen.height / 10), 2 * Screen.width / 4, Screen.height / 8), "Tempo : " + EscolherAneisTeclado.niceTime, TipoLetraFinal);
						GUI.Label (new Rect ((5 * Screen.width / 8), (3 * Screen.height / 10), 2 * Screen.width / 4, Screen.height / 8), "Pontos : " + PassarAneis.Pontos, TipoLetraFinal);
						if (GUI.Button (new Rect ((Screen.width / 4), (4 * Screen.height / 8), 2 * Screen.width / 4, Screen.height / 8), "Reiniciar")) {
								clickMenuReiniciar = true;
						}
						if (GUI.Button (new Rect ((Screen.width / 4), (6 * Screen.height / 8), 2 * Screen.width / 4, Screen.height / 8), "Sair")) {
								Application.LoadLevel ("MainMenu");
						}
				} 	
		}
}
