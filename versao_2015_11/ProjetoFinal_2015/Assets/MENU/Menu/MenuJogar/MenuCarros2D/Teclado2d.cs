using UnityEngine;
using System.Collections;

public class Teclado2d : MonoBehaviour {

	public GUIText Teclado;
	// Use this for initialization
	void Start () {
		if (Application.platform == RuntimePlatform.Android) {
			this.GetComponent<GUIText>().text = "Touch";
		} else if (Application.platform == RuntimePlatform.IPhonePlayer) {
			this.GetComponent<GUIText>().text = "Touch";
		} else {
			this.GetComponent<GUIText>().text = "Teclado";
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
		Application.LoadLevel ("Carros2D");
	}
}
