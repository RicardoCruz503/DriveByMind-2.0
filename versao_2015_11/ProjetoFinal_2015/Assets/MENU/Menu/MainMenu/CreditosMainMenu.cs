﻿using UnityEngine;
using System.Collections;

public class CreditosMainMenu : MonoBehaviour {

	// Use this for initialization
	void Start () {
	
	}
	
	// Update is called once per frame
	void Update () {
	
	}

	void OnMouseEnter ()
	{
		this.GetComponent<GUIText>().color = Color.yellow;
	}
	
	void OnMouseExit ()
	{
		this.GetComponent<GUIText>().color = Color.grey;
	}
	
	void OnMouseDown ()
	{
		Application.LoadLevel ("Creditos");
	}
}
