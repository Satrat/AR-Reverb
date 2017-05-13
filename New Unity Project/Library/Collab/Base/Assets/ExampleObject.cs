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
    public int clipInd;

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
        clipInd = Random.Range(0, 2);
    }
}

public class ExampleObject : MonoBehaviour
{

    public Ray[] rays;
    public float[] travelDist;
    public float[] volume;
    private static int maxDepth = 30;

    private AudioSource sound;
    private SpatialMappingManager mapper;
    private SpaceManager sections;
    private int numRays;

    private List<rayData> currRays;
    private List<rayData> nextRays;

    // Use this for initialization
    void Start()
    {
        float startTime = Time.realtimeSinceStartup;
        currRays = new List<rayData>();
        nextRays = new List<rayData>();

        SpatialMappingManager mapper = SpatialMappingManager.Instance;
        SpaceManager sections = SpaceManager.Instance;


        if (mapper == null)
        {
            Debug.Log("Y'all's got issues. I'm out.");
            Destroy(this);
        }

        rays = new Ray[75];
        travelDist = new float[75];
        volume = new float[75];
        numRays = 75;

        for (float x = -1.0f; x <= 1.0f; x += 0.5f)
        {
            for (float y = 0.0f; y <= 1.0f; y += 0.5f)
            {
                for (float z = -1.0f; z <= 1.0f; z += 0.5f)
                {
                    Ray r = new Ray(transform.position, transform.TransformDirection(new Vector3(x, y, z)));
                    currRays.Add(new rayData(r, new Vector3(0, 0, 0), 0.4f, 0.0f, null, 0.0f));
                }
            }
        }

        float startRay;

        for (int i = 0; i < maxDepth; i++)
        {
            RaycastHit hit;
            startRay = Time.realtimeSinceStartup;

            foreach (rayData r in currRays)
            {
                if (Physics.Raycast(r.ray, out hit, 100, mapper.LayerMask))
                {
                    Vector3 end = hit.point;
                    r.distance += hit.distance;
                    r.endpoint = end;
                    r.startTime = startRay;

                    //sections.sendToQuad(r);

                    Vector3 normal = hit.normal;
                    Vector3 reflectDir = Vector3.Reflect(end, normal);
                    nextRays.Add(new rayData(new Ray(end, reflectDir), new Vector3(0, 0, 0), r.volume * 0.95f, r.distance, null, 0.0f));
                }

            }

            sections.sendToQuad(currRays);

            currRays = nextRays;
            nextRays = new List<rayData>();


        }


        float endTime = Time.realtimeSinceStartup;
        Debug.Log("Time spent ExampleObject Start: " + (endTime - startTime));
    }

    // Update is called once per frame
    void Update()
    {

    }
}
