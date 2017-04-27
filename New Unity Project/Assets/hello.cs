using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class hello : MonoBehaviour {

    public Vector3[] rays;
    // Use this for initialization
    void Start () {
        rays = new Vector3[196];
        int index = 0;
        for (float x = -1.0f; x <= 1.0f; x += 0.33f)
        {
            for (float y = 0.0f; y <= 1.0f; y += 0.33f)
            {
                for (float z = -1.0f; z <= 1.0f; z += 0.33f)
                {
                    Vector3 dir = transform.TransformDirection(new Vector3(x, y, z));
                    if (index <= 404)
                    {
                        rays[index] = dir;
                    }
                    index++;
                }
            }
        }


    }

    // Update is called once per frame
    void Update() {

  

        //Ray ray = new Ray(transform.position, dir);

        /*Vector3 fwd = transform.TransformDirection(Vector3.forward);
        Vector3 back = transform.TransformDirection(Vector3.back);
        Debug.DrawRay(transform.position, fwd, Color.green);
        RaycastHit hit;
        if (Physics.Raycast(transform.position, fwd, out hit))
        {
            Vector3 incomingVec = hit.point - transform.position;
            Vector3 reflectVec = Vector3.Reflect(incomingVec, hit.normal);
            Debug.DrawLine(transform.position, hit.point, Color.red);
        }
        if (Physics.Raycast(transform.position, back, out hit))
        {
            Vector3 incomingVec = hit.point - transform.position;
            Vector3 reflectVec = Vector3.Reflect(incomingVec, hit.normal);
            Debug.DrawLine(transform.position, hit.point, Color.green);
        }*/

        for(int i = 0; i < 196; i++)
        {
            Debug.DrawRay(transform.position, rays[i], Color.red);
        }


    }
}
