using UnityEngine;
using System.Collections;

public class ExitToMenuScript : MonoBehaviour {

	// Use this for initialization
	void Start () {
	
	}
	
	// Update is called once per frame
	void Update () {
		if (Input.GetKeyDown ("escape")) {
			Estimulo.abortThreads ();
			Application.LoadLevel ("DefinirFrequencias_Menuopcoes");
		}
	}
}
