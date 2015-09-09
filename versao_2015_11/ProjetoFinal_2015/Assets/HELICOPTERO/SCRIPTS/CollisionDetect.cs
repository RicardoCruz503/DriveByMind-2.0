using UnityEngine;
using System.Collections;

public class CollisionDetect : MonoBehaviour {
	public GameObject explosao;
	public GameObject Helicoptero;
	int numeroColisao = 0; // Apenas faz a explosao quando bate a primeira vez em algo
	// Use this for initialization
	void Start () {
	
	}
	
	// Update is called once per frame
	void Update () {
	
	}
	void  OnCollisionEnter (Collision collision)
	{
		Debug.Log(collision.gameObject.name);
		HeliLivre.Colisao = true;
		explosao.SetActive (true);
		if (numeroColisao == 0) {
			explosao.GetComponent<AudioSource> ().Play ();
		}
		Helicoptero.GetComponent<Rigidbody> ().useGravity = true;
		Helicoptero.GetComponent<AudioSource> ().Stop ();
		if (collision.gameObject.name.Contains ("floor") || collision.gameObject.name.Contains ("Terrain") || collision.gameObject.name.Contains ("Street") || collision.gameObject.name.Contains ("Soccer") || numeroColisao == 4) {
			HeliLivre.JogoAcabou = true;
			Helicoptero.GetComponent<RodarHelices> ().enabled = false;
			
			Time.timeScale = 0.0f;
		}
		numeroColisao++;
	}
}
