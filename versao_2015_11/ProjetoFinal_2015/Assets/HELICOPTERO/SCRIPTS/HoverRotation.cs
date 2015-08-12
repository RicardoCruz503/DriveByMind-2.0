using UnityEngine;
using System.Collections;

public class HoverRotation : MonoBehaviour {

    public Rigidbody rigidbody;
    private float tiltFactor = 1.0f;
	// Use this for initialization
	void Start () {
	
	}
	
	// Update is called once per frame
	void Update () {
	
	}

    void FixedUpdate(){
        /*if (!Input.GetKey(KeyCode.UpArrow) && !FrenteAndroid.FrnAndroid == true)
        {
            if (this.gameObject.transform.position.y > EscolherAneisTeclado.PosicaoYAneis)
            {
                //this.transform.Translate(new Vector3(0, -7 * Time.deltaTime, 0));
            }
        }
        //this.transform.Translate(new Vector3(0, 0, 20 * Time.deltaTime));


        if (Input.GetKey(KeyCode.RightArrow) || DireitaAndroid.DirAndroid == true)
        {
            tiltFactor = 1.0f;
            rigidbody.rotation = Quaternion.Euler(new Vector3(0.0f, 0.0f, -tiltFactor));
            //this.transform.Rotate(new Vector3(0, 10 * Time.deltaTime, 0));
        }
        if (Input.GetKey(KeyCode.LeftArrow) || EsquerdaAndroid.EsqAndroid == true)
        {
            tiltFactor = -1.0f;
            rigidbody.rotation = Quaternion.Euler(new Vector3(0.0f, 0.0f, -tiltFactor));
            //this.transform.Rotate(new Vector3(0, -10 * Time.deltaTime, 0));
        }

        if (Input.GetKey(KeyCode.UpArrow) || FrenteAndroid.FrnAndroid == true)
        {
           // this.transform.Translate(new Vector3(0, 10 * Time.deltaTime, 0));
        }*/
    }
}
