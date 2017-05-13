using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class followCamera : MonoBehaviour {

    public Vector3 cameraPos;

	// Use this for initialization
	void Start () {

        GetComponent<Renderer>().enabled = false;

        cameraPos = Camera.main.transform.position;
        float x = cameraPos.x;
        float z = cameraPos.z;
        transform.position = new Vector3(x, 0.0f, z);
        transform.localScale = new Vector3(1.0f, 5.0f, 1.0f);

    }
	
	// Update is called once per frame
	void Update () {
        cameraPos = Camera.main.transform.position;

        float x = cameraPos.x;
        float z = cameraPos.z;
        transform.position = new Vector3(x, 0.0f, z);

    }
}
