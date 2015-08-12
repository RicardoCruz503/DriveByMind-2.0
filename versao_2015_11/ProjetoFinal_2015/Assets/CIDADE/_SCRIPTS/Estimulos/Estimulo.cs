using UnityEngine;
using System.Collections;
using System.Collections.Generic;
using System.Threading;
using System.Timers;
using System;

public class Estimulo : MonoBehaviour {
	
	public static List<Estimulo> myEstimulos = new List<Estimulo> ();
	int delay;
	bool state;
	public float frequencia;
	private GUITexture Seta1;
	private GUITexture Seta2;
	private DateTime before;
	private Boolean running = false;
	// Use this for initialization


	public void setTextures(GUITexture Seta1, GUITexture Seta2){
		this.Seta1 = Seta1;
		this.Seta2 = Seta2;
	}

	public void Run()
	{
		delay = (int) (1000 / frequencia)/2;
		before = DateTime.Now;
	}

    void FixedUpdate()
    {
        Blink();
    }
	
	// Update is called once per frame
	void Update ()
	{
		if (state) {
			Seta1.enabled=true;
			Seta2.enabled=false;
		} else {
			Seta2.enabled=true;
			Seta1.enabled=false;
		}
	}
	
	void Blink ()
	{
		/*while (running) {
			//BEFORE - Easy on Processor / Not Accurate
			/*int resto = DateTime.Now.Millisecond % delay;
			state = !state;
			Thread.Sleep(delay-resto);*/

			//AFTER - Accurate / Hard on Processor
			/*DateTime now = DateTime.Now;
			 if(now.Subtract(before).Milliseconds >= delay){
				state = !state;
				before = DateTime.Now;
			 }


			//COMBINED - Accurate / Easy on Processor
			DateTime now = DateTime.Now;
			Thread.Sleep(delay - now.Subtract(before).Milliseconds);
			state = !state;
			before = DateTime.Now;
		}*/

        DateTime now = DateTime.Now;
        if (now.Subtract(before).Milliseconds >= delay)
        {
            state = !state;
            before = DateTime.Now;
        }
	}

	public static void abortThreads(){
		
	}

	void OnApplicationQuit()
	{
		running = false;
	}
}
