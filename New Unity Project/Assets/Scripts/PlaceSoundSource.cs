using System.Collections;
using System.Collections.Generic;
using UnityEngine.VR.WSA.Input;
using UnityEngine;
using System;

public class PlaceSoundSource : MonoBehaviour {

    public GameObject soundSphere;
    private GestureRecognizer recognizer;

    private void Awake()
    {

        recognizer = new GestureRecognizer();

        recognizer.SetRecognizableGestures(GestureSettings.Tap);

        recognizer.TappedEvent += Recognizer_TappedEvent;


        recognizer.StartCapturingGestures();
    }

    private void Recognizer_TappedEvent(InteractionSourceKind source, int tapCount, Ray headRay)
    {
        
        Instantiate(soundSphere, Camera.main.transform.position + Camera.main.transform.forward.normalized, Quaternion.LookRotation(-Camera.main.transform.forward));    
    }

    // Use this for initialization
    void Start () {
		
	}
	
	// Update is called once per frame
	void Update () {
		
	}
}
