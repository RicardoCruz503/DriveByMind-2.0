using UnityEngine;
using System.Collections;

public class AnimacaoMindwave : MonoBehaviour {

	// Use this for initialization
	void Start () {
		GameObject.Find ("SetaMindwave").GetComponent<Renderer>().enabled = false;
	}
	
	// Update is called once per frame
	void Update () {
	
	}

	void OnMouseEnter(){
		GameObject.Find ("SetaMindwave").GetComponent<Renderer>().enabled = true;
	}
	
	void OnMouseExit(){
		GameObject.Find ("SetaMindwave").GetComponent<Renderer>().enabled = false;
	}

	void OnMouseDown(){
		Application.LoadLevel("Carros2D_Mindwave");
	}
}
