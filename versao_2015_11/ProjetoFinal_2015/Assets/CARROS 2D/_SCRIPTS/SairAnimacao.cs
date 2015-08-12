﻿using UnityEngine;
using System.Collections;

public class SairAnimacao : MonoBehaviour {

	// Use this for initialization
	void Start () {
	}
	
	// Update is called once per frame
	void Update () {
		
	}
	
	void OnMouseEnter(){
		GetComponent<Renderer>().material.color = Color.grey;
		GetComponent<TextMesh>().fontSize = GetComponent<TextMesh> ().fontSize + 40;
		
		//GameObject.Find ("Seta1").renderer.enabled = false;
		//animation.Play ("ResizeUp");
	}
	
	void OnMouseExit(){
		GetComponent<Renderer>().material.color = Color.white;
		GetComponent<TextMesh>().fontSize = GetComponent<TextMesh> ().fontSize - 40;
		//animation.Play ("ResizeDown");
	}

	void OnMouseDown(){
		Application.Quit();
	}
}
