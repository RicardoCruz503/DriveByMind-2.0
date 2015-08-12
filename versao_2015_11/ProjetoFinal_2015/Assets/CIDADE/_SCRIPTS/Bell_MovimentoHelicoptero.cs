using UnityEngine;
using System.Collections;

public class Bell_MovimentoHelicoptero : MonoBehaviour {

	// Use this for initialization
	public int velocidadeHeli = 10;
	void Start () {
	
	}
	
	// Update is called once per frame
	void Update () {
		if (Input.GetKey (KeyCode.Space)) {
			GetComponent<Rigidbody>().useGravity = false;
			transform.Translate (Input.GetAxis ("Horizontal") * 10 * Time.deltaTime, Input.GetAxis ("Vertical") * 10 * Time.deltaTime, velocidadeHeli * Time.deltaTime);
		} else {
			GetComponent<Rigidbody>().useGravity =true;	
		}
	}
	}
