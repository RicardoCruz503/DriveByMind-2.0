﻿using UnityEngine;
using System.Collections;

public class DestinosHelicoptero : MonoBehaviour
{
	
		string DestinoAnterior;
		int NumeroDestino = 1;
		int Aceleracao = 10;
		int Velocidade = 10;
		string DestinoActual;
		bool EscolhaDestino = false;
		public Transform[] destino;
		private NavMeshAgent agente;
		public string comando;				 // contem a tecla que o utilizador escolheu
		public bool chegouDestino = false; 	 // verifica se passou num trigger Destino
		public bool ParouCruzamento = false; // Serve para verificar se chegou a um cruzamento
		public static int distancia;
		public GUIText DistanciaGUI;
		public GameObject Teclado;
	
	
		// Use this for initialization
		void Start ()
		{
				Time.timeScale = 1.0f;
				DestinoActual = "Destino20";
				DestinoAnterior = "Destino20";
				comando = "";
				NumeroDestino = 1;
				EscolhaDestino = false;
				chegouDestino = false;
				ParouCruzamento = false;
		}
	
		// Update is called once per frame
		void Update ()
		{
				
				if (EscolhaDestino == true) {
						Teclado.SetActive (true);
				} else {
						Teclado.SetActive (false);
				}
				distancia = (int)Vector3.Distance (this.transform.position, destino [NumeroDestino].transform.position);
				DistanciaGUI.GetComponent<GUIText>().text = distancia + "m";
				// Escolha da tecla correspondente ao destino do Helicoptero
				if (EscolhaDestino == true || ParouCruzamento == true) {
						if (Input.GetKey (KeyCode.M) || Direita.DireitaTeclado == true) {
								comando = "M";
								EscolhaDestino = false;
								Direita.DireitaTeclado = false;
								this.gameObject.GetComponent<NavMeshAgent> ().speed = Velocidade;
								if (this.gameObject.GetComponent<NavMeshAgent> ().enabled == false) {
										ParouCruzamento = false;
										ActivarNavMesh (Velocidade, Aceleracao);
								}
				
						}
						if (Input.GetKey (KeyCode.Z) || Esquerda.EsquerdaTeclado == true) {
								
								comando = "Z";
								EscolhaDestino = false;
								Esquerda.EsquerdaTeclado = false;
								this.gameObject.GetComponent<NavMeshAgent> ().speed = Velocidade;
								if (this.gameObject.GetComponent<NavMeshAgent> ().enabled == false) {
										ParouCruzamento = false;
										ActivarNavMesh (Velocidade, Aceleracao);
								}
						}
						if (Input.GetKey (KeyCode.Y)) {
								comando = "Y";
								EscolhaDestino = false;
								this.gameObject.GetComponent<NavMeshAgent> ().speed = Velocidade;
								if (this.gameObject.GetComponent<NavMeshAgent> ().enabled == false) {
										ParouCruzamento = false;
										ActivarNavMesh (Velocidade, Aceleracao);
								}
						}
				}
		
				// Dar destino ao Taxi segundo a tecla em cima pressionada
				if (comando != "" && chegouDestino == true) {
						if (comando == "M") {
								VirarDireita ();
								DestinoAnterior = DestinoActual;
								chegouDestino = false;
				
						}
						if (comando == "Z") {
								VirarEsquerda ();
								DestinoAnterior = DestinoActual;
								chegouDestino = false;
						}
						comando = "";
				} 

				// Verificacao de colidiu com os Triggers colocados ao meio das ruas, para visualizar as direcoes
				if (distancia <= 150 && comando == "") {
						EscolhaDestino = true;
						this.gameObject.GetComponent<NavMeshAgent> ().speed = Mathf.Floor (Velocidade / 2);
				} else if (distancia > 150) {
						EscolhaDestino = false;
						this.gameObject.GetComponent<NavMeshAgent> ().speed = Mathf.Floor (Velocidade);
				}

				// Colocar o Taxi na rota de destino definida pela tecla pressionada, se o NavMesh estiver ativo
				if (this.gameObject.GetComponent<NavMeshAgent> ().enabled == true) {
						agente = gameObject.GetComponent<NavMeshAgent> ();
						agente.SetDestination (destino [NumeroDestino].position);
				}
		}
	
		void OnTriggerEnter (Collider collision)
		{
		
				// Verificacao de colidiu com algum Trigger Destino. Esta condicao serve para verificar onde estao os viajantes,
				// bem como verificar se o jogador definiu algum caminho antes de chegar ao cruzamento
				if (collision.gameObject.name.Contains ("Destino") && DestinoActual != collision.gameObject.name) {
						
						
						// Verificacao se chegou ao cruzamento ja com rota definida ou nao. Se nao para o helicoptero e espera direcao
						if (comando == "") {
								this.gameObject.GetComponent<NavMeshAgent> ().speed = 0;
								this.gameObject.GetComponent<NavMeshAgent> ().acceleration = 0;
								this.gameObject.GetComponent<NavMeshAgent> ().enabled = false;
								ParouCruzamento = true;
						}
						// Actualiza o Destino Actual, para saber em que sentido o Taxi segue
						//comando = "";
						DestinoActual = collision.gameObject.name;
						chegouDestino = true;
						EscolhaDestino = false;
						distancia = 0;
				}
		}

		// Verificacao se o Taxi parou num cruzamento, se parou ativa de novo o NavMesh e respectivas Velocidades
		void ActivarNavMesh (int Velocidade, int Aceleracao)
		{
				this.gameObject.GetComponent<NavMeshAgent> ().enabled = true;
				this.gameObject.GetComponent<NavMeshAgent> ().speed = Velocidade;
				this.gameObject.GetComponent<NavMeshAgent> ().acceleration = Aceleracao;
		}
	

		//  ----------------------  FUNCOES RELATIVAS A DIRECAO ESCOLHIDA ------------------------------------------------------
		void VirarDireita ()
		{
			
				if (DestinoActual == "Destino1" && DestinoAnterior == "Destino20") {
						NumeroDestino = 3;
				}
				if (DestinoActual == "Destino1" && DestinoAnterior == "Destino2") {
						NumeroDestino = 20;
				}
				if (DestinoActual == "Destino1" && DestinoAnterior == "Destino3") {
						NumeroDestino = 2;
				}
		
				if (DestinoActual == "Destino2" && DestinoAnterior == "Destino1") {
						NumeroDestino = 5;
				}
				if (DestinoActual == "Destino2" && DestinoAnterior == "Destino3") {
						NumeroDestino = 4;
				}
				if (DestinoActual == "Destino2" && DestinoAnterior == "Destino4") {
						NumeroDestino = 1;
				}
				if (DestinoActual == "Destino2" && DestinoAnterior == "Destino5") {
						NumeroDestino = 4;
				}
		
				if (DestinoActual == "Destino3" && DestinoAnterior == "Destino1") {
						NumeroDestino = 7;
				}
				if (DestinoActual == "Destino3" && DestinoAnterior == "Destino5") {
						NumeroDestino = 1;
				}
				if (DestinoActual == "Destino3" && DestinoAnterior == "Destino6") {
						NumeroDestino = 5;
				}
				if (DestinoActual == "Destino3" && DestinoAnterior == "Destino7") {
						NumeroDestino = 6;
				}
		
				if (DestinoActual == "Destino4" && DestinoAnterior == "Destino2") {
						NumeroDestino = 5;
				}
				if (DestinoActual == "Destino4" && DestinoAnterior == "Destino5") {
						NumeroDestino = 9;
				}
				if (DestinoActual == "Destino4" && DestinoAnterior == "Destino9") {
						NumeroDestino = 2;
				}
		
				if (DestinoActual == "Destino5" && DestinoAnterior == "Destino2") {
						NumeroDestino = 10;
				}
				if (DestinoActual == "Destino5" && DestinoAnterior == "Destino3") {
						NumeroDestino = 10;
				}
				if (DestinoActual == "Destino5" && DestinoAnterior == "Destino4") {
						NumeroDestino = 3;
				}
				if (DestinoActual == "Destino5" && DestinoAnterior == "Destino6") {
						NumeroDestino = 10;
				}
				if (DestinoActual == "Destino5" && DestinoAnterior == "Destino10") {
						NumeroDestino = 4;
				}
		
				if (DestinoActual == "Destino6" && DestinoAnterior == "Destino3") {
						NumeroDestino = 12;
				}
				if (DestinoActual == "Destino6" && DestinoAnterior == "Destino11") {
						NumeroDestino = 5;
				}
				if (DestinoActual == "Destino6" && DestinoAnterior == "Destino12") {
						NumeroDestino = 11;
				}
				if (DestinoActual == "Destino6" && DestinoAnterior == "Destino7") {
						NumeroDestino = 12;
				}
		
				if (DestinoActual == "Destino7" && DestinoAnterior == "Destino3") {
						NumeroDestino = 8;
				}
				if (DestinoActual == "Destino7" && DestinoAnterior == "Destino8") {
						NumeroDestino = 6;
				}
				if (DestinoActual == "Destino7" && DestinoAnterior == "Destino6") {
						NumeroDestino = 3;
				}
		
				if (DestinoActual == "Destino8" && DestinoAnterior == "Destino7") {
						NumeroDestino = 20;
				}
				if (DestinoActual == "Destino8" && DestinoAnterior == "Destino12") {
						NumeroDestino = 7;
				}
				if (DestinoActual == "Destino8" && DestinoAnterior == "Destino13") {
						NumeroDestino = 7;
				}
				if (DestinoActual == "Destino8" && DestinoAnterior == "Destino20") {
						NumeroDestino = 13;
				}
		
				if (DestinoActual == "Destino9" && DestinoAnterior == "Destino4") {
						NumeroDestino = 10;
				}
				if (DestinoActual == "Destino9" && DestinoAnterior == "Destino10") {
						NumeroDestino = 14;
				}
				if (DestinoActual == "Destino9" && DestinoAnterior == "Destino14") {
						NumeroDestino = 4;
				}
		
				if (DestinoActual == "Destino10" && DestinoAnterior == "Destino5") {
						NumeroDestino = 11;
				}
				if (DestinoActual == "Destino10" && DestinoAnterior == "Destino11") {
						NumeroDestino = 15;
				}
				if (DestinoActual == "Destino10" && DestinoAnterior == "Destino14") {
						NumeroDestino = 5;
				}
				if (DestinoActual == "Destino10" && DestinoAnterior == "Destino15") {
						NumeroDestino = 9;
				}
		
				if (DestinoActual == "Destino11" && DestinoAnterior == "Destino6") {
						NumeroDestino = 12;
				}
				if (DestinoActual == "Destino11" && DestinoAnterior == "Destino10") {
						NumeroDestino = 6;
				}
				if (DestinoActual == "Destino11" && DestinoAnterior == "Destino12") {
						NumeroDestino = 15;
				}
				if (DestinoActual == "Destino11" && DestinoAnterior == "Destino15") {
						NumeroDestino = 10;
				}
				if (DestinoActual == "Destino11" && DestinoAnterior == "Destino16") {
						NumeroDestino = 15;
				}
		
				if (DestinoActual == "Destino12" && DestinoAnterior == "Destino6") {
						NumeroDestino = 13;
				}
				if (DestinoActual == "Destino12" && DestinoAnterior == "Destino8") {
						NumeroDestino = 16;
				}
				if (DestinoActual == "Destino12" && DestinoAnterior == "Destino11") {
						NumeroDestino = 8;
				}
				if (DestinoActual == "Destino12" && DestinoAnterior == "Destino13") {
						NumeroDestino = 16;
				}
				if (DestinoActual == "Destino12" && DestinoAnterior == "Destino16") {
						NumeroDestino = 11;
				}
		
				if (DestinoActual == "Destino13" && DestinoAnterior == "Destino8") {
						NumeroDestino = 17;
				}
				if (DestinoActual == "Destino13" && DestinoAnterior == "Destino12") {
						NumeroDestino = 8;
				}
				if (DestinoActual == "Destino13" && DestinoAnterior == "Destino17") {
						NumeroDestino = 12;
				}
		
				if (DestinoActual == "Destino14" && DestinoAnterior == "Destino9") {
						NumeroDestino = 10;
				}
				if (DestinoActual == "Destino14" && DestinoAnterior == "Destino19") {
						NumeroDestino = 9;
				}
		
				if (DestinoActual == "Destino15" && DestinoAnterior == "Destino10") {
						NumeroDestino = 18;
				}
				if (DestinoActual == "Destino15" && DestinoAnterior == "Destino11") {
						NumeroDestino = 18;
				}
				if (DestinoActual == "Destino15" && DestinoAnterior == "Destino18") {
						NumeroDestino = 19;
				}
				if (DestinoActual == "Destino15" && DestinoAnterior == "Destino19") {
						NumeroDestino = 10;
				}
		
				if (DestinoActual == "Destino16" && DestinoAnterior == "Destino11") {
						NumeroDestino = 12;
				}
				if (DestinoActual == "Destino16" && DestinoAnterior == "Destino12") {
						NumeroDestino = 17;
				}
				if (DestinoActual == "Destino16" && DestinoAnterior == "Destino17") {
						NumeroDestino = 18;
				}
				if (DestinoActual == "Destino16" && DestinoAnterior == "Destino18") {
						NumeroDestino = 11;
				}
		
				if (DestinoActual == "Destino17" && DestinoAnterior == "Destino13") {
						NumeroDestino = 18;
				}
				if (DestinoActual == "Destino17" && DestinoAnterior == "Destino16") {
						NumeroDestino = 13;
				}
				if (DestinoActual == "Destino17" && DestinoAnterior == "Destino18") {
						NumeroDestino = 16;
				}
		
				if (DestinoActual == "Destino18" && DestinoAnterior == "Destino15") {
						NumeroDestino = 16;
				}
				if (DestinoActual == "Destino18" && DestinoAnterior == "Destino16") {
						NumeroDestino = 17;
				}
				if (DestinoActual == "Destino18" && DestinoAnterior == "Destino17") {
						NumeroDestino = 19;
				}
				if (DestinoActual == "Destino18" && DestinoAnterior == "Destino19") {
						NumeroDestino = 16;
				}
		
				if (DestinoActual == "Destino19" && DestinoAnterior == "Destino14") {
						NumeroDestino = 15;
				}
				if (DestinoActual == "Destino19" && DestinoAnterior == "Destino15") {
						NumeroDestino = 18;
				}
				if (DestinoActual == "Destino19" && DestinoAnterior == "Destino18") {
						NumeroDestino = 14;
				}
		
				if (DestinoActual == "Destino20" && DestinoAnterior == "Destino1") {
						NumeroDestino = 1;
				}
				if (DestinoActual == "Destino20" && DestinoAnterior == "Destino8") {
						NumeroDestino = 1;
				}

		}

		void VirarEsquerda ()
		{

				if (DestinoActual == "Destino1" && DestinoAnterior == "Destino20") {
						NumeroDestino = 2;
				}
				if (DestinoActual == "Destino1" && DestinoAnterior == "Destino2") {
						NumeroDestino = 3;
				}
				if (DestinoActual == "Destino1" && DestinoAnterior == "Destino3") {
						NumeroDestino = 20;
				}
		
				if (DestinoActual == "Destino2" && DestinoAnterior == "Destino1") {
						NumeroDestino = 4;
				}
				if (DestinoActual == "Destino2" && DestinoAnterior == "Destino3") {
						NumeroDestino = 1;
				}
				if (DestinoActual == "Destino2" && DestinoAnterior == "Destino4") {
						NumeroDestino = 5;
				}
				if (DestinoActual == "Destino2" && DestinoAnterior == "Destino5") {
						NumeroDestino = 1;
				}
		
				if (DestinoActual == "Destino3" && DestinoAnterior == "Destino1") {
						NumeroDestino = 5;
				}
				if (DestinoActual == "Destino3" && DestinoAnterior == "Destino5") {
						NumeroDestino = 6;
				}
				if (DestinoActual == "Destino3" && DestinoAnterior == "Destino6") {
						NumeroDestino = 2;
				}
				if (DestinoActual == "Destino3" && DestinoAnterior == "Destino7") {
						NumeroDestino = 1;
				}
		
				if (DestinoActual == "Destino4" && DestinoAnterior == "Destino2") {
						NumeroDestino = 9;
				}
				if (DestinoActual == "Destino4" && DestinoAnterior == "Destino5") {
						NumeroDestino = 2;
				}
				if (DestinoActual == "Destino4" && DestinoAnterior == "Destino9") {
						NumeroDestino = 5;
				}
		
				if (DestinoActual == "Destino5" && DestinoAnterior == "Destino2") {
						NumeroDestino = 4;
				}
				if (DestinoActual == "Destino5" && DestinoAnterior == "Destino3") {
						NumeroDestino = 4;
				}
				if (DestinoActual == "Destino5" && DestinoAnterior == "Destino4") {
						NumeroDestino = 10;
				}
				if (DestinoActual == "Destino5" && DestinoAnterior == "Destino6") {
						NumeroDestino = 2;
				}
				if (DestinoActual == "Destino5" && DestinoAnterior == "Destino10") {
						NumeroDestino = 2;
				}
		
				if (DestinoActual == "Destino6" && DestinoAnterior == "Destino3") {
						NumeroDestino = 11;
				}
				if (DestinoActual == "Destino6" && DestinoAnterior == "Destino11") {
						NumeroDestino = 7;
				}
				if (DestinoActual == "Destino6" && DestinoAnterior == "Destino12") {
						NumeroDestino = 3;
				}
				if (DestinoActual == "Destino6" && DestinoAnterior == "Destino7") {
						NumeroDestino = 5;
				}
		
				if (DestinoActual == "Destino7" && DestinoAnterior == "Destino3") {
						NumeroDestino = 6;
				}
				if (DestinoActual == "Destino7" && DestinoAnterior == "Destino8") {
						NumeroDestino = 1;
				}
				if (DestinoActual == "Destino7" && DestinoAnterior == "Destino6") {
						NumeroDestino = 8;
				}
		
				if (DestinoActual == "Destino8" && DestinoAnterior == "Destino7") {
						NumeroDestino = 12;
				}
				if (DestinoActual == "Destino8" && DestinoAnterior == "Destino12") {
						NumeroDestino = 13;
				}
				if (DestinoActual == "Destino8" && DestinoAnterior == "Destino13") {
						NumeroDestino = 20;
				}
				if (DestinoActual == "Destino8" && DestinoAnterior == "Destino20") {
						NumeroDestino = 7;
				}
		
				if (DestinoActual == "Destino9" && DestinoAnterior == "Destino4") {
						NumeroDestino = 14;
				}
				if (DestinoActual == "Destino9" && DestinoAnterior == "Destino10") {
						NumeroDestino = 4;
				}
				if (DestinoActual == "Destino9" && DestinoAnterior == "Destino14") {
						NumeroDestino = 10;
				}
		
				if (DestinoActual == "Destino10" && DestinoAnterior == "Destino5") {
						NumeroDestino = 9;
				}
				if (DestinoActual == "Destino10" && DestinoAnterior == "Destino11") {
						NumeroDestino = 5;
				}
				if (DestinoActual == "Destino10" && DestinoAnterior == "Destino14") {
						NumeroDestino = 15;
				}
				if (DestinoActual == "Destino10" && DestinoAnterior == "Destino15") {
						NumeroDestino = 11;
				}
				if (DestinoActual == "Destino10" && DestinoAnterior == "Destino9") {
						NumeroDestino = 15;
				}
		
				if (DestinoActual == "Destino11" && DestinoAnterior == "Destino6") {
						NumeroDestino = 10;
				}
				if (DestinoActual == "Destino11" && DestinoAnterior == "Destino10") {
						NumeroDestino = 16;
				}
				if (DestinoActual == "Destino11" && DestinoAnterior == "Destino12") {
						NumeroDestino = 6;
				}
				if (DestinoActual == "Destino11" && DestinoAnterior == "Destino15") {
						NumeroDestino = 16;
				}
				if (DestinoActual == "Destino11" && DestinoAnterior == "Destino16") {
						NumeroDestino = 6;
				}
		
				if (DestinoActual == "Destino12" && DestinoAnterior == "Destino6") {
						NumeroDestino = 16;
				}
				if (DestinoActual == "Destino12" && DestinoAnterior == "Destino8") {
						NumeroDestino = 6;
				}
				if (DestinoActual == "Destino12" && DestinoAnterior == "Destino11") {
						NumeroDestino = 16;
				}
				if (DestinoActual == "Destino12" && DestinoAnterior == "Destino13") {
						NumeroDestino = 11;
				}
				if (DestinoActual == "Destino12" && DestinoAnterior == "Destino16") {
						NumeroDestino = 13;
				}
		
				if (DestinoActual == "Destino13" && DestinoAnterior == "Destino8") {
						NumeroDestino = 12;
				}
				if (DestinoActual == "Destino13" && DestinoAnterior == "Destino12") {
						NumeroDestino = 17;
				}
				if (DestinoActual == "Destino13" && DestinoAnterior == "Destino17") {
						NumeroDestino = 8;
				}
		
				if (DestinoActual == "Destino14" && DestinoAnterior == "Destino9") {
						NumeroDestino = 9;
				}
				if (DestinoActual == "Destino14" && DestinoAnterior == "Destino19") {
						NumeroDestino = 19;
				}
		
				if (DestinoActual == "Destino15" && DestinoAnterior == "Destino10") {
						NumeroDestino = 19;
				}
				if (DestinoActual == "Destino15" && DestinoAnterior == "Destino11") {
						NumeroDestino = 19;
				}
				if (DestinoActual == "Destino15" && DestinoAnterior == "Destino18") {
						NumeroDestino = 11;
				}
				if (DestinoActual == "Destino15" && DestinoAnterior == "Destino19") {
						NumeroDestino = 18;
				}
		
				if (DestinoActual == "Destino16" && DestinoAnterior == "Destino11") {
						NumeroDestino = 17;
				}
				if (DestinoActual == "Destino16" && DestinoAnterior == "Destino12") {
						NumeroDestino = 12;
				}
				if (DestinoActual == "Destino16" && DestinoAnterior == "Destino17") {
						NumeroDestino = 12;
				}
				if (DestinoActual == "Destino16" && DestinoAnterior == "Destino18") {
						NumeroDestino = 17;
				}
		
				if (DestinoActual == "Destino17" && DestinoAnterior == "Destino13") {
						NumeroDestino = 16;
				}
				if (DestinoActual == "Destino17" && DestinoAnterior == "Destino16") {
						NumeroDestino = 18;
				}
				if (DestinoActual == "Destino17" && DestinoAnterior == "Destino18") {
						NumeroDestino = 18;
				}
		
				if (DestinoActual == "Destino18" && DestinoAnterior == "Destino15") {
						NumeroDestino = 19;
				}
				if (DestinoActual == "Destino18" && DestinoAnterior == "Destino16") {
						NumeroDestino = 15;
				}
				if (DestinoActual == "Destino18" && DestinoAnterior == "Destino17") {
						NumeroDestino = 15;
				}
				if (DestinoActual == "Destino18" && DestinoAnterior == "Destino19") {
						NumeroDestino = 17;
				}
		
				if (DestinoActual == "Destino19" && DestinoAnterior == "Destino14") {
						NumeroDestino = 18;
				}
				if (DestinoActual == "Destino19" && DestinoAnterior == "Destino15") {
						NumeroDestino = 14;
				}
				if (DestinoActual == "Destino19" && DestinoAnterior == "Destino18") {
						NumeroDestino = 15;
				}
		
				if (DestinoActual == "Destino20" && DestinoAnterior == "Destino1") {
						NumeroDestino = 8;
				}
				if (DestinoActual == "Destino20" && DestinoAnterior == "Destino8") {
						NumeroDestino = 13;
				}

		}

}
