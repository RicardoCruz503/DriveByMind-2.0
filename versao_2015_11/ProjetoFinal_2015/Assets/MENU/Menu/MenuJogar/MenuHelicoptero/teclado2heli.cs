using UnityEngine;
using System.Collections;

public class teclado2heli : MonoBehaviour {
	
	// Use this for initialization
	void Start () {
		if (Application.platform == RuntimePlatform.Android) {
			this.GetComponent<GUIText>().text = "Touch II";
		} else if (Application.platform == RuntimePlatform.IPhonePlayer) {
			this.GetComponent<GUIText>().text = "Touch II";
		} else {
			this.GetComponent<GUIText>().text = "Teclado II";
		}
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
		this.GetComponent<GUIText>().color = Color.white;
	}
	
	void OnMouseDown ()
	{
		Application.LoadLevel ("HelicopteroLivreTeclado");
	}
}
