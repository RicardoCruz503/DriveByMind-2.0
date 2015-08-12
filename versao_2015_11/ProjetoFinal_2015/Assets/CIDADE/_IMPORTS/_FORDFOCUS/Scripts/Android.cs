﻿using UnityEngine;
using System.Collections;
	
public class Android : MonoBehaviour {
		
		public WheelCollider WheelFL;
		public WheelCollider WheelFR;
		public WheelCollider WheelBL;
		public WheelCollider WheelBR;
		public float maxTorque = 50f;
		public Transform WheelFLTrans;
		public Transform WheelFRTrans;
		public Transform WheelBLTrans;
		public Transform WheelBRTrans;
		public Transform VolanteTrans;
		public float VelocidadeActual;
		public float VelocidadeMaxima = 150f;
		private float desacelerar = 40f; 
		public GameObject LuzTraseiraDir;
		public GameObject LuzTraseiraEsq;
		public Material LuzTravar;
		public Material LuzMarchaTras;
		public Material LuzNormal;
		bool TravaoDeMao = false;
		public float MaximaTravagemMao = 100f;
		public GUITexture Acelerar;
		public GUITexture Travar;
		public GUITexture VirarEsq;
		public GUITexture VirarDir;
		// Use this for initialization
		void Start () {
			// Para o carro nao caputar
				GetComponent<Rigidbody>().centerOfMass = new Vector3 (0f, -0.9f, 0f);
		}
		
		void FixedUpdate () {
			ControloCarro ();
		//	TravagemTravaoDeMao ();
		}
		void Update(){
			// Rotaçao das Rodas
			WheelFLTrans.Rotate (WheelFL.rpm / 60 * 360 * Time.deltaTime, 0, 0);
			WheelFRTrans.Rotate (WheelFR.rpm / 60 * 360 * Time.deltaTime, 0, 0);
			WheelBLTrans.Rotate (WheelBL.rpm / 60 * 360 * Time.deltaTime, 0, 0);
			WheelBRTrans.Rotate (WheelBR.rpm / 60 * 360 * Time.deltaTime, 0, 0);
			LuzesTraseiras ();
			EngineSound ();
			// Virar o Volante
			VolanteTrans.Rotate(WheelFL.steerAngle - WheelFLTrans.localEulerAngles.z, 0, 0);
			
		}
		
		// Funçao de controlo do carro
		void ControloCarro(){
			VelocidadeActual = 2 * 22 / 7 * WheelBL.radius*WheelBL.rpm * 60 / 1000; // Formula matematica para calcular a velocidade
			VelocidadeActual = Mathf.Round (VelocidadeActual); // Tirar as casas decimais
			if (VelocidadeActual < VelocidadeMaxima && !TravaoDeMao) {
//  --------------------------------  MOVIMENTOS PARA O ANDROID -------------------------------------------------
// --------------------------------------------------------------------------------------------------------------
			if (Input.touches.Length <= 0) {

			} else {
				WheelBR.brakeTorque = 0;
				WheelBL.brakeTorque = 0;
				for(int i=0; i<Input.touchCount; i++){
					if (Acelerar.GetComponent<GUITexture>().HitTest(Input.GetTouch(i).position) || Travar.GetComponent<GUITexture>().HitTest(Input.GetTouch(i).position)){
						if(Acelerar.GetComponent<GUITexture>().HitTest(Input.GetTouch(i).position)){
							if (Input.GetTouch (i).phase == TouchPhase.Stationary) {
								WheelBR.motorTorque = maxTorque * 200*Time.deltaTime;// Acelerar Carro
								WheelBL.motorTorque = maxTorque * 200*Time.deltaTime;// Acelerar Carro
							}
						} else {
							WheelBR.brakeTorque = desacelerar;  // Travar Carro
							WheelBL.brakeTorque = desacelerar;	// Travar Carro
						}
						if(Travar.GetComponent<GUITexture>().HitTest(Input.GetTouch(i).position)){
							if (Input.GetTouch (i).phase == TouchPhase.Stationary) {
								WheelBR.brakeTorque = 100*Time.deltaTime;// Acelerar Carro
								WheelBL.brakeTorque = 100*Time.deltaTime; // Acelerar Carro
							}
						}
					} else{
						WheelBR.motorTorque = 0; 
						WheelBL.motorTorque = 0; 
					} if(VirarDir.GetComponent<GUITexture>().HitTest(Input.GetTouch(i).position) || VirarEsq.GetComponent<GUITexture>().HitTest(Input.GetTouch(i).position)){
						if(VirarDir.GetComponent<GUITexture>().HitTest(Input.GetTouch(i).position)){
							if (Input.GetTouch (i).phase == TouchPhase.Stationary) {
								WheelFL.steerAngle = 200*Time.deltaTime;
								WheelFR.steerAngle = 200*Time.deltaTime;
							}
						}
						if(VirarEsq.GetComponent<GUITexture>().HitTest(Input.GetTouch(i).position)){
							if (Input.GetTouch (i).phase == TouchPhase.Stationary) {
								WheelFL.steerAngle = -200*Time.deltaTime;
								WheelFR.steerAngle = -200*Time.deltaTime;
							}
						}
					} else {
						WheelFL.steerAngle = 0;
						WheelFR.steerAngle = 0;
					}
				}

			}
// --------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------
			} else{
				WheelBR.motorTorque = 0; 
				WheelBL.motorTorque = 0; 
			}
			
		}
		
		void LuzesTraseiras() {
		if (VelocidadeActual > 0 && !TravaoDeMao){
				LuzTraseiraDir.GetComponent<Renderer>().material = LuzTravar;
				LuzTraseiraEsq.GetComponent<Renderer>().material = LuzTravar;
			} else if (VelocidadeActual < 0 && !TravaoDeMao){
				LuzTraseiraDir.GetComponent<Renderer>().material = LuzTravar;
				LuzTraseiraEsq.GetComponent<Renderer>().material = LuzTravar;
			} else if (VelocidadeActual < 0 && !TravaoDeMao){
				LuzTraseiraDir.GetComponent<Renderer>().material = LuzMarchaTras;
				LuzTraseiraEsq.GetComponent<Renderer>().material = LuzMarchaTras;
			}else if (!TravaoDeMao){
				LuzTraseiraDir.GetComponent<Renderer>().material = LuzNormal;
				LuzTraseiraEsq.GetComponent<Renderer>().material = LuzNormal;
			}
		}
		
	/*	void TravagemTravaoDeMao () {
			if (Input.GetKey (KeyCode.Space)) {
				TravaoDeMao = true;				
			} else {
				TravaoDeMao = false;
			}
			if (TravaoDeMao) {
				WheelBR.brakeTorque = MaximaTravagemMao; 
				WheelBL.brakeTorque = MaximaTravagemMao;
				WheelBR.motorTorque = 0; 
				WheelBL.motorTorque = 0;
				if (VelocidadeActual <1 && VelocidadeActual > -1){
					LuzTraseiraDir.renderer.material = LuzNormal;
					LuzTraseiraEsq.renderer.material = LuzNormal;
				} else {
					LuzTraseiraDir.renderer.material = LuzTravar;
					LuzTraseiraEsq.renderer.material = LuzTravar;
				}
			}
		} */
		
		void EngineSound (){
			GetComponent<AudioSource>().pitch = VelocidadeActual / VelocidadeMaxima +1;
		}
	}

