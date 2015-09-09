using UnityEngine;
using System.Collections;

public class EstimulosEscolha : MonoBehaviour 
{
	public GUITexture VerSeta1;
	public GUITexture VerSeta2;
	public Texture[] GUIArrow;
	public static int numeroSeta = 1;
	public static int Seta1Usada = 1;
	public static int Seta2Usada = 0;
	public static int aux = 0;
	// Use this for initialization
	void Start ()
	{

	}
	
	// Update is called once per frame
	void Update ()
	{
		
	}
	
	void OnMouseDown ()
	{
		aux++;
		if (aux == 2) {
			GameObject.Find ("MenuPref").GetComponent<PiscarSetas> ().enabled = true;
		}
		if (this.GetComponent<GUITexture>().name == "SetaAmarela") {
			if (numeroSeta == 1) {
				VerSeta1.GetComponent<GUITexture>().texture = GUIArrow[0];
				numeroSeta = 2;
				Seta1Usada = 0;
				//Seta1Usada = "SetaAmarela";
			} else {
				VerSeta2.GetComponent<GUITexture>().texture = GUIArrow[0];
				numeroSeta = 1;
				Seta2Usada = 0;
				//Seta2Usada = "SetaAmarela";
			}
			
		}
		if (this.GetComponent<GUITexture>().name == "SetaAzul") {
			if (numeroSeta == 1) {
				VerSeta1.GetComponent<GUITexture>().texture = GUIArrow[1];
				numeroSeta = 2;
				Seta1Usada = 1;

				//Seta1Usada = "SetaAzul";
			} else {
				VerSeta2.GetComponent<GUITexture>().texture = GUIArrow[1];
				numeroSeta = 1;
				Seta2Usada = 1;
				//Seta2Usada = "SetaAzul";
			}
			
		}
		if (this.GetComponent<GUITexture>().name == "SetaBranca") {
			if (numeroSeta == 1) {
				VerSeta1.GetComponent<GUITexture>().texture = GUIArrow[2];
				numeroSeta = 2;
				Seta1Usada = 2;
				//Seta1Usada = "SetaBranca";
			} else {
				VerSeta2.GetComponent<GUITexture>().texture = GUIArrow[2];
				numeroSeta = 1;
				Seta2Usada = 2;
				//Seta2Usada = "SetaBranca";
			}
			
		}
		if (this.GetComponent<GUITexture>().name == "SetaVerde") {
			if (numeroSeta == 1) {
				VerSeta1.GetComponent<GUITexture>().texture = GUIArrow[3];
				numeroSeta = 2;
				Seta1Usada = 3;
				//Seta1Usada = "SetaVerde";
			} else {
				VerSeta2.GetComponent<GUITexture>().texture = GUIArrow[3];
				numeroSeta = 1;
				Seta2Usada = 3;
				//Seta2Usada = "SetaVerde";
			}
			
		}
		if (this.GetComponent<GUITexture>().name == "Xadrez1") {
			if (numeroSeta == 1) {
				VerSeta1.GetComponent<GUITexture>().texture = GUIArrow[4];
				numeroSeta = 2;
				Seta1Usada = 4;
				//Seta1Usada = "Xadrez1";

			} else {
				VerSeta2.GetComponent<GUITexture>().texture = GUIArrow[4];
				numeroSeta = 1;
				//Seta2Usada = "Xadrez1";
				Seta2Usada = 4;
			}		
		}
		if (this.GetComponent<GUITexture>().name == "Xadrez2") {
			if (numeroSeta == 1) {
				VerSeta1.GetComponent<GUITexture>().texture = GUIArrow[5];
				numeroSeta = 2;
				Seta1Usada = 5;
				//Seta1Usada = "Xadrez2";
			} else {
				VerSeta2.GetComponent<GUITexture>().texture = GUIArrow[5];
				numeroSeta = 1;
				Seta2Usada = 5;
				//Seta2Usada = "Xadrez2";
			}	
		}
		if (this.gameObject.name == "Bt_Definir") {
			aux = 0;
			PiscarSetas.ThreadBlink.Abort();
		}
		
	}
}
