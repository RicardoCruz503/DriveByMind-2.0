﻿using UnityEngine;
using System.Collections;
using System.Collections.Generic;
public class NavigationTest : MonoBehaviour {

	
	
	public Transform[] destino;
	private NavMeshAgent agente;
	public GameObject EscolhaDestino;
	int numerodestino=0;
	string DestinoAnterior;
	int Aceleracao = 8;
	public Material TaxiOcupado;
	public Material TaxiLivre;
	// Use this for initialization
	void Start () {
		EscolhaDestino.SetActive (false);
		
	}
	
	// Update is called once per frame
	void Update () {
		
		if (EscolhaDestino.activeSelf == true) {
			if (Input.GetKey(KeyCode.M)){
				EscolhaDestino.SetActive(false);
				numerodestino = 1;
				this.gameObject.GetComponent<NavMeshAgent>().acceleration = Aceleracao;
			}
			if (Input.GetKey(KeyCode.Z)){
				EscolhaDestino.SetActive(false);
				numerodestino = 2;
				this.gameObject.GetComponent<NavMeshAgent>().acceleration = Aceleracao;
			}
		}
		
		agente = gameObject.GetComponent<NavMeshAgent>();
		agente.SetDestination (destino[numerodestino].position);
	}
	
	void OnTriggerEnter(Collider collision) {
		
		if (collision.gameObject.name == "Destino") 
		{
			this.gameObject.GetComponent<NavMeshAgent>().acceleration = 0;
			this.gameObject.GetComponent<NavMeshAgent>().acceleration = 0;
			this.gameObject.GetComponent<NavMeshAgent>().Stop();
			EscolhaDestino.SetActive (true);
			this.gameObject.GetComponent<NavMeshAgent>().acceleration =0;
			this.gameObject.GetComponent<NavMeshAgent>().Stop();
			DestinoAnterior = collision.gameObject.name;
			GameObject.Find(DestinoAnterior).SetActive(false);
			GameObject.Find("EstadoTaxi1").GetComponent<Renderer>().material = TaxiOcupado;
			GameObject.Find("EstadoTaxi2").GetComponent<Renderer>().material = TaxiOcupado;
			GameObject.Find("EstadoTaxi3").GetComponent<Renderer>().material = TaxiOcupado;
			GameObject.Find("EstadoTaxi4").GetComponent<Renderer>().material = TaxiOcupado;
		//	this.gameObject.transform.position = new Vector3(posicaoAtualX,posicaoAtualY,posicaoAtualZ);
		}

		if (collision.gameObject.name == "Destino1") 
		{
			GameObject.Find("EstadoTaxi1").GetComponent<Renderer>().material = TaxiLivre;
			GameObject.Find("EstadoTaxi2").GetComponent<Renderer>().material = TaxiLivre;
			GameObject.Find("EstadoTaxi3").GetComponent<Renderer>().material = TaxiLivre;
			GameObject.Find("EstadoTaxi4").GetComponent<Renderer>().material = TaxiLivre;
			EscolhaDestino.SetActive (true);
			agente.acceleration=0;
			this.gameObject.GetComponent<NavMeshAgent>().acceleration =0;
			DestinoAnterior = collision.gameObject.name;
			GameObject.Find(DestinoAnterior).SetActive(false);
		}

		if (collision.gameObject.name == "Destino2") 
		{

			EscolhaDestino.SetActive (true);
			agente.acceleration=0;
			this.gameObject.GetComponent<NavMeshAgent>().acceleration =0;
			DestinoAnterior = collision.gameObject.name;
			GameObject.Find(DestinoAnterior).SetActive(false);
		}
	}
}
