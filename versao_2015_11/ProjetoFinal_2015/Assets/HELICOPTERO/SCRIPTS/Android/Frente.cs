using UnityEngine;
using System.Collections;

public class Frente : MonoBehaviour {

	// Use this for initialization
	public static bool FrenteTeclado=false;
	
	public Texture Frnt;
	
	void Start () {
		if (Application.platform == RuntimePlatform.Android) {
			this.GetComponent<GUITexture>().texture = Frnt;
		} else if (Application.platform == RuntimePlatform.IPhonePlayer) {
			this.GetComponent<GUITexture>().texture = Frnt;
		}
	}
	
	// Update is called once per frame
	void Update () {
		
	}
	
	void OnMouseDown(){
		FrenteTeclado = true;
	}
}
