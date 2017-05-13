using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using HoloToolkit.Unity.SpatialMapping;

public class rayData : MonoBehaviour
{
    public Ray ray; //holds origin and directional vector
    public Vector3 origin;
    public Vector3 direction;
    public Vector3 endpoint;
    public float volume;
    public float distance;
    public AudioSource sound;
    public bool playing;
    public float startTime;
    public int clipInd;

    public rayData(Ray r, Vector3 e, float v, float d, AudioSource s, float t)
    {
        ray = r;
        origin = r.origin;
        direction = r.direction;
        endpoint = e;
        volume = v;
        distance = d;
        sound = s;
        playing = false;
        startTime = t;
        clipInd = Random.Range(0, 3);
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


    //All raytracing with the mesh done here
    //called each time a new sound source is created
    void Start()
    {
        float startTime = Time.realtimeSinceStartup;

        currRays = new List<rayData>();
        nextRays = new List<rayData>();

        SpatialMappingManager mapper = SpatialMappingManager.Instance;
        SpaceManager sections = SpaceManager.Instance;


        if (mapper == null)
        {
            Debug.Log("Error finding the mesh");
            Destroy(this);
        }


        numRays = 75;

        //creates starting rays from sound source
        for (float x = -1.0f; x <= 1.0f; x += 0.33f)
        {
            for (float y = 0.0f; y <= 1.0f; y += 0.33f)
            {
                for (float z = -1.0f; z <= 1.0f; z += 0.33f)
                {
                    Ray r = new Ray(transform.position, transform.TransformDirection(new Vector3(x, y, z)));
                    currRays.Add(new rayData(r, new Vector3(0, 0, 0), 0.4f, 0.0f, null, 0.0f));
                }
            }
        }

        float startRay;

        //raycasting
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

                    Vector3 normal = hit.normal;
                    Vector3 reflectDir = Vector3.Reflect(end, normal);
                    nextRays.Add(new rayData(new Ray(end, reflectDir), new Vector3(0, 0, 0), r.volume * 0.95f, r.distance, null, 0.0f));
                }

            }

            //calculate which rays intersect which boxes
            //utilizes SIMD
            sections.sendToQuad(currRays);

            currRays = nextRays;
            nextRays = new List<rayData>();


        }


        float endTime = Time.realtimeSinceStartup;
        Debug.Log("Time spent ExampleObject Start: " + (endTime - startTime));
    }

    void Update()
    {

    }
}
