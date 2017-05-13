using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using HoloToolkit.Unity.SpatialMapping;

public class rayData : MonoBehaviour
{
    public Ray ray; //holds origin and directional vector
    public Vector3 endpoint;
    public float volume;
    public float distance;
    public AudioSource sound;
    public bool playing;
    public float startTime;

    //we don't need to store if its playing, we can just call sound.isPlaying to check

    public rayData(Ray r, Vector3 e, float v, float d, AudioSource s, float t)
    {
        ray = r;
        endpoint = e;
        volume = v;
        distance = d;
        sound = s;
        playing = false;
        startTime = t;
    }
}

public class ExampleObject : MonoBehaviour {

    public Ray[] rays;
    public float[] travelDist;
    public float[] volume;
    private static int maxDepth = 30;

    private AudioSource sound;
    private SpatialMappingManager mapper;
    private SpaceManager sections;
    private int numRays;


    // Use this for initialization
    void Start()  
    {
        float startTime = Time.realtimeSinceStartup;

        SpatialMappingManager mapper = SpatialMappingManager.Instance;
        SpaceManager sections = SpaceManager.Instance;


        if (mapper == null)
        {
            Debug.Log("Y'all's got issues. I'm out.");
            Destroy(this);
        }

        GameObject child = new GameObject();
        child.transform.parent = gameObject.transform;
        sound = child.AddComponent<AudioSource>();
        sound.clip = Resources.Load("long") as AudioClip;
        sound.playOnAwake = false;
        sound.loop = false;
        sound.spatialize = true;
        sound.rolloffMode = AudioRolloffMode.Linear;
        sound.volume = 0.3f;

        rays = new Ray[75];
        travelDist = new float[75];
        volume = new float[75];
        numRays = 75;

        int currIndex = 0;

        for(float x = -1.0f; x <= 1.0f; x += 0.5f)
        {
            for(float y = 0.0f; y <= 1.0f; y += 0.5f)
            {
                for(float z = -1.0f; z <= 1.0f; z += 0.5f)
                {
                    rays[currIndex] = new Ray(transform.position, transform.TransformDirection(new Vector3(x, y, z)));
                    volume[currIndex] = 0.4f;
                    currIndex++;


                }
            }
        }

        float startRay = Time.realtimeSinceStartup;

        for (int i = 0; i < maxDepth; i++)
        {
            RaycastHit hit;
            for (int j = 0; j < numRays; j++)
            {
                if (Physics.Raycast(rays[j], out hit, 100, mapper.LayerMask))
                {
                    Vector3 end = hit.point;
                    travelDist[j] += hit.distance;

                    sections.sendToQuad(new rayData(rays[j], end, volume[j], travelDist[j], sound, startRay));

                    /*if (hit.collider.gameObject.CompareTag("Player"))
                    {
                        float delay = travelDist[j] / 340.0f;

                        //Debug.Log("ray" + j + "hit player!");
                        //print("ray" + j + "hit player!");

                        //play dat Audio
                        StartCoroutine(PlaySoundAfterDelay(sound, volume[j], rays[j].origin, volume[j]));

                    }*/

                    //Debug.DrawLine(rays[j].origin, end, Color.green, 5);

                    volume[j] *= 0.9f;
                    Vector3 normal = hit.normal;
                    Vector3 reflectDir = Vector3.Reflect(end, normal);
                    rays[j] = new Ray(end, reflectDir);
                }
            }
        }


        float endTime = Time.realtimeSinceStartup;
        Debug.Log("Time spent ExampleObject Start: " + (endTime - startTime));
    }

    // Update is called once per frame
    void Update()
    {

    }

    /*IEnumerator PlaySoundAfterDelay(AudioSource audioSource, float delay, Vector3 loc, float vol)
    {

        yield return new WaitForSecondsRealtime(delay);
        AudioSource.PlayClipAtPoint(sound.clip, loc, vol);

    }*/
}
