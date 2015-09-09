using UnityEngine;
using System.Collections;
using System.Threading;
using System.Timers;
using System;

public class CalibracaoPiscarSetas : MonoBehaviour {

	public static Thread ThreadBlink;
	int delay;
	bool state;
	float frequencia;
	public GameObject Seta1;
	public GameObject Seta2;
	public Sprite[] sprites;
	// Use this for initialization
	void Start ()
	{
		frequencia = Calibracao.frequencia;
		delay = (int) (1000 / frequencia)/2;

		ThreadBlink = new Thread (Blink);
		ThreadBlink.Start ();

		if (EstimulosEscolha.Seta1Usada == 0) {
			Seta1.GetComponent<SpriteRenderer> ().sprite = sprites [0];		
		}
		if (EstimulosEscolha.Seta2Usada == 0) {
			Seta2.GetComponent<SpriteRenderer> ().sprite = sprites [0];		
		}
		if (EstimulosEscolha.Seta1Usada == 1) {
			Seta1.GetComponent<SpriteRenderer> ().sprite = sprites [1];		
		}
		if (EstimulosEscolha.Seta2Usada == 1) {
			Seta2.GetComponent<SpriteRenderer> ().sprite = sprites [1];		
		}
		if (EstimulosEscolha.Seta1Usada == 2) {
			Seta1.GetComponent<SpriteRenderer> ().sprite = sprites [2];		
		}
		if (EstimulosEscolha.Seta2Usada == 2) {
			Seta2.GetComponent<SpriteRenderer> ().sprite = sprites [2];		
		}
		if (EstimulosEscolha.Seta1Usada == 3) {
			Seta1.GetComponent<SpriteRenderer> ().sprite = sprites [3];		
		}
		if (EstimulosEscolha.Seta2Usada == 3) {
			Seta2.GetComponent<SpriteRenderer> ().sprite = sprites [3];		
		}
		if (EstimulosEscolha.Seta1Usada == 4) {
			Seta1.GetComponent<SpriteRenderer> ().sprite = sprites [4];		
		}
		if (EstimulosEscolha.Seta2Usada == 4) {
			Seta2.GetComponent<SpriteRenderer> ().sprite = sprites [4];		
		}
		if (EstimulosEscolha.Seta1Usada == 5) {
			Seta1.GetComponent<SpriteRenderer> ().sprite = sprites [5];		
		}
		if (EstimulosEscolha.Seta2Usada == 5) {
			Seta2.GetComponent<SpriteRenderer> ().sprite = sprites [5];		
		}
	}
	
	// Update is called once per frame
	void Update ()
	{
		frequencia = Calibracao.frequencia;
		delay = (int) (1000 / frequencia)/2;

		if (state) {
			Seta1.SetActive (true);
			Seta2.SetActive (false);
		} else {
			Seta1.SetActive (false);
			Seta2.SetActive (true);
		}
	}
	
	void Blink ()
	{
		while (true) {
			int resto = DateTime.Now.Millisecond % delay;
			state = !state;
			Thread.Sleep (delay-resto);
		}
	}
	
	void OnApplicationQuit()
	{
		ThreadBlink.Abort ();
	}
}
