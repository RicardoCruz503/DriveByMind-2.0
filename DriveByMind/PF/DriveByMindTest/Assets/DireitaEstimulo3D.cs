using UnityEngine;
using System.Collections;
using System.Timers;
using System;

public class DireitaEstimulo3D : Estimulo3D
{
    public Texture Mat1Dir, Mat2Dir;
    public GameObject ModelDir;

    void Start()
    {
        base.setTextures(Mat1Dir, Mat2Dir);
        Model = ModelDir;
        if (FreqEscolhe.DefFreqManual == true)
        {
            base.frequencia = FreqEscolhe.FreqDirVal;
        }
        else
        {
            frequencia = 9;
        }
        base.Run();
    }
}

