using UnityEngine;
using System.Collections;
using System.Collections.Generic;
using System.Threading;
using System.Timers;
using System;

public class Estimulo3D : MonoBehaviour
{

    public static List<Estimulo3D> myEstimulos = new List<Estimulo3D>();
    int delay;
    bool state;
    public float frequencia;
    private Texture Mat1, Mat2;
    protected GameObject Model;
    private DateTime before;
    private Boolean running = false;
    
    // Use this for initialization


    public void setTextures(Texture Mat1 , Texture Mat2)
    {
        this.Mat1 = Mat1;
        this.Mat2 = Mat2;
    }

    public void Run()
    {
        delay = (int)(1000 / frequencia) / 2;

        before = DateTime.Now;
    }

    void FixedUpdate()
    {
        Blink();
    }

    // Update is called once per frame
    void Update()
    {
        if (state)
        {
            Model.GetComponent<Renderer>().material.mainTexture = Mat1;
        }
        else
        {
            Model.GetComponent<Renderer>().material.mainTexture = Mat2;
        }
        Debug.Log(Model.GetComponent<Renderer>().material.name);
    }

    void Blink()
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

    public static void abortThreads()
    {

    }

    void OnApplicationQuit()
    {
        running = false;
    }
}
