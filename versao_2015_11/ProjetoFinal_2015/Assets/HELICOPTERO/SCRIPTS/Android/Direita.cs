﻿using UnityEngine;
using System.Collections;

public class Direita : MonoBehaviour {

	// Use this for initialization
	public static bool DireitaTeclado=false;
	public Texture Dir;
	
	void Start () {
		if (Application.platform == RuntimePlatform.Android) {
			this.GetComponent<GUITexture>().texture = Dir;
		} else if (Application.platform == RuntimePlatform.IPhonePlayer) {
			this.GetComponent<GUITexture>().texture = Dir;
		}
	}
	
	// Update is called once per frame
	void Update () {
		
	}
	
	void OnMouseDown(){
		Debug.Log("DireitaTeclado");
		DireitaTeclado = true;
	}
}
