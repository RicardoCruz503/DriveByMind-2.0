using UnityEngine;
using System.Collections;

public class RodarAnel : MonoBehaviour {

	public Transform AnelGold;
	public Transform AnelRed;

	// Use this for initialization
	void Start () {
	
	}

	// Update is called once per frame
	void Update () {
		AnelGold.Rotate(0, 0, 50 * Time.deltaTime);
		AnelRed.Rotate(0, 0, 50 * Time.deltaTime);
	}
}
