using UnityEngine;
using System.Collections;

public class DireitaAndroid : MonoBehaviour {

	public static bool DirAndroid = false;
	// Use this for initialization
	void Start () {
	
	}
	
	// Update is called once per frame
	void Update () {
	
		if (Input.touches.Length <= 0) {
			DirAndroid = false;
		} else {
			for (int i=0; i<Input.touchCount; i++) {
				if (this.GetComponent<GUITexture>().HitTest (Input.GetTouch (i).position)) {
					if (Input.GetTouch (i).phase == TouchPhase.Stationary) {
						DirAndroid = true;
					}
				}
			}
		}
	}
}
