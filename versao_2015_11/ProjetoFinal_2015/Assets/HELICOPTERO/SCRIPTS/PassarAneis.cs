using UnityEngine;
using System.Collections;

public class PassarAneis : MonoBehaviour {

	public static int Pontos= 0;
	public GameObject anel;
	
	void Start(){
		//Pontos= 0;
	}

	void OnTriggerEnter (Collider collision)
	{
		GameObject.Find ("Main Camera").GetComponent<AudioSource>().Play ();
		this.gameObject.SetActive (false);
		anel.gameObject.SetActive (false);
		Pontos += 1;
	}
	
	
}
