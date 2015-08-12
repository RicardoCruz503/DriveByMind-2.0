using UnityEngine;
using System.Collections;

public class SinaisMatlabMenuOpcoes : MonoBehaviour {

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
		this.GetComponent<GUIText>().color = Color.white;
	}
	
	void OnMouseDown ()
	{
		Application.LoadLevel ("MatLab_Definicoes");
	}
}
