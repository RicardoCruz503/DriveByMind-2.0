using UnityEngine;
using System.Collections;

public class AnimacaoWindows : MonoBehaviour {

	// Use this for initialization
	void Start () {
		GameObject.Find ("SetaWindows").GetComponent<Renderer>().enabled = false;
	}
	
	// Update is called once per frame
	void Update () {
	
	}

	void OnMouseEnter(){
		GameObject.Find ("SetaWindows").GetComponent<Renderer>().enabled = true;
	}
	
	void OnMouseExit(){
		GameObject.Find ("SetaWindows").GetComponent<Renderer>().enabled = false;
	}

	void OnMouseDown(){
		Application.LoadLevel("Carros2D");
	}

}
