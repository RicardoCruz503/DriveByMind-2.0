﻿using UnityEngine;
using System.Collections;
using System.Collections.Generic;

public class ApanharMoedas : MonoBehaviour
{
		public static int Pontuacao= 0;

	void Start(){
		Pontuacao= 0;
	}

		void OnTriggerEnter (Collider collision)
		{

				GameObject.Find ("Main Camera").GetComponent<AudioSource>().Play ();
				this.gameObject.SetActive (false);
				Pontuacao += 1;
		}

		
}
