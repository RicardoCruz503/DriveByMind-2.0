using UnityEngine;
using System.Collections;
using System.Timers;
using System;

public class EsquerdaEstimulo3D : Estimulo3D
{
    public Texture Mat1Esq, Mat2Esq;
    public GameObject ModelEsq;

    void Start()
    {
        base.setTextures(Mat1Esq, Mat2Esq);
        Model = ModelEsq;
        if (FreqEscolhe.DefFreqManual == true)
        {
            base.frequencia = FreqEscolhe.FreqEsqVal;
        }
        else
        {
            frequencia = 9;
        }
        base.Run();
    }
}


