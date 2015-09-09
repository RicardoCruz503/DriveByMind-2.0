﻿using UnityEngine;
using System.Collections;

public class HeliLivre : MonoBehaviour
{		
		
		public int Pontuacao = 0;
		public string comando;				 // contem a tecla que o utilizador escolheu
		public static float distancia;
		public GameObject Teclado;
		public static bool Colisao = false;
		public static bool JogoAcabou = false;
		public Texture pauseGUI;
		bool clickMenuReiniciar = false;
		public GUIStyle TipoLetraFinal;
		public GameObject ChopperBody;
		private float tilt = 3;
		private float rotation = 0;
		private int turnSpeed = 1;

		
		// Use this for initialization
		void Start ()
		{
				Time.timeScale = 1.0f;
				if (Application.platform == RuntimePlatform.Android) {
						Teclado.SetActive (true);
				} else if (Application.platform == RuntimePlatform.IPhonePlayer) {
						Teclado.SetActive (true);
				} else {
						Teclado.SetActive (false);
				}
		clickMenuReiniciar = false;
		JogoAcabou = false;
		Colisao = false;
		Pontuacao = 0;
		}
		
		// Update is called once per frame
		void FixedUpdate ()
		{
            //this.transform.rotation = Quaternion.Euler(new Vector3(0.0f, transform.rotation.y, transform.rotation.y));
				if (Colisao == false) {
						if (!Input.GetKey (KeyCode.UpArrow) && !FrenteAndroid.FrnAndroid == true) {
								if (this.gameObject.transform.position.y > EscolherAneisTeclado.PosicaoYAneis) {
										this.transform.Translate (new Vector3 (0, -7 * turnSpeed * Time.deltaTime, 0));
								}
						}
						this.transform.Translate (new Vector3 (0, 0, 20 * Time.deltaTime));
						if (Input.GetKey (KeyCode.RightArrow) || DireitaAndroid.DirAndroid == true) {
								this.transform.Rotate (new Vector3 (0, 15 * turnSpeed * Time.deltaTime, 0));	
								if(rotation*tilt < 50){
									rotation += 0.5f;
								}
								
						}
						if (Input.GetKey (KeyCode.LeftArrow) || EsquerdaAndroid.EsqAndroid == true) {
								this.transform.Rotate (new Vector3 (0, -15 * turnSpeed * Time.deltaTime, 0));
								if(rotation*tilt > -50){
									rotation -= 0.5f;
								}
						}
						
						if(!Input.GetKey(KeyCode.LeftArrow) && !Input.GetKey(KeyCode.RightArrow)){
								if(rotation > -1.5 && 0-rotation < -0.5){
									rotation -= 0.4f;
								}
								else if(rotation < 1.5 && 0-rotation > 0.5 ){
									rotation +=0.4f;	
								}
						}
		
						if (Input.GetKey (KeyCode.UpArrow) || FrenteAndroid.FrnAndroid == true) {
								this.transform.Translate (new Vector3 (0, 10 * turnSpeed * Time.deltaTime, 0));
						}
						if (Input.GetKey (KeyCode.DownArrow) || FrenteAndroid.FrnAndroid == true) {
							this.transform.Translate (new Vector3 (0, -5 * turnSpeed , 0));
						}
						if (Input.GetKey (KeyCode.Space)) {
							this.transform.Translate (new Vector3 (0, 0 * Time.deltaTime, 10 ));
							turnSpeed = 10;
						} 
						if(!Input.GetKey (KeyCode.Space)){
							turnSpeed = 1;
			}
				} else if (Colisao == true && JogoAcabou != true) {
						this.transform.Translate (new Vector3 (0, -10 * Time.deltaTime, 0));	
				} 
			
				if (clickMenuReiniciar == true) {
						Application.LoadLevel ("HelicopteroLivreTeclado");
				}

				Transform t = ChopperBody.transform;
				ChopperBody.transform.localEulerAngles = new Vector3 (t.localEulerAngles.x, t.localEulerAngles.y, rotation * -tilt);
				//ChopperBody.transform.rotation = Quaternion.Euler (t.rotation.x, t.rotation.y, rotation * -tilt)
		}

		void OnGUI ()
		{
		
				if (JogoAcabou == true) {
						GUI.DrawTexture (new Rect (0, 0, Screen.width, Screen.height), pauseGUI);
						GUI.Label (new Rect ((Screen.width / 4), (Screen.height / 10), 2 * Screen.width / 4, Screen.height / 8), "FIM DO JOGO", TipoLetraFinal);
						GUI.Label (new Rect ((Screen.width / 8), (3 * Screen.height / 10), 2 * Screen.width / 4, Screen.height / 8), "Tempo : " + EscolherAneisTeclado.niceTime, TipoLetraFinal);
						GUI.Label (new Rect ((5 * Screen.width / 8), (3 * Screen.height / 10), 2 * Screen.width / 4, Screen.height / 8), "Pontos : " + PassarAneis.Pontos, TipoLetraFinal);
						if (GUI.Button (new Rect ((Screen.width / 4), (4 * Screen.height / 8), 2 * Screen.width / 4, Screen.height / 8), "Reiniciar")) {
				Application.LoadLevel ("HelicopteroLivreTeclado");
						}
						if (GUI.Button (new Rect ((Screen.width / 4), (6 * Screen.height / 8), 2 * Screen.width / 4, Screen.height / 8), "Sair")) {
								Application.LoadLevel ("MainMenu");
						}
				} 	
		}
}
