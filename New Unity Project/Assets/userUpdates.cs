using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using HoloToolkit.Unity.SpatialMapping;
#if !UNITY_EDITOR
using System.Threading.Tasks;
#endif

//keeps track of the users current quadrant 
//raycasts all rays that pass through their quadrant to check for hit
public class userUpdates : MonoBehaviour
{
    public AudioClip[] clips;

    private Vector3 currentPos;
    private SpaceManager sections;
    private Quadrant currentQuad;
    private int quadInd;
    private bool[] toPlay;

    public List<rayData> toRemove;

    private int i = 0;


    // Use this for initialization
    void Start () {

        clips = new AudioClip[4];
        clips[0] = Resources.Load("clip1") as AudioClip;
        clips[1] = Resources.Load("clip2") as AudioClip;
        clips[2] = Resources.Load("clip3") as AudioClip;
        clips[3] = Resources.Load("dog") as AudioClip;

        currentPos = transform.position;
        toRemove = new List<rayData>();

        if(SpaceManager.Instance != null)
        {
            sections = SpaceManager.Instance;
            quadInd = sections.locToQuad(currentPos);
            currentQuad = sections.subspaces[quadInd];
        }
        
    }

    // Update is called once per frame
    void Update()
    {

        float start = Time.realtimeSinceStartup;

        currentPos = transform.position;
        int prevInd = quadInd;
        quadInd = sections.locToQuad(currentPos);

        //only start new clips if we move

        currentQuad = sections.subspaces[quadInd];

        RaycastHit hit;

        float currTime = Time.realtimeSinceStartup;

        float clipLen = 20.0f;

#if !UNITY_EDITOR
        toPlay = new bool[currentQuad.rays.Count];

        computeSounds();
        for (int i = 0; i < toPlay.Length; i++)
        {
            rayData r = currentQuad.rays[i];
            if (toPlay[i])
            {
                //only check clips that are not already playing
                if (!(r.playing))
                {
                    float delay = r.distance / 340.0f;
                    r.playing = true;

                    GameObject child = new GameObject();
                    child.transform.position = r.ray.origin;
                    r.sound = child.AddComponent<AudioSource>();
                    r.sound.clip = clips[r.clipInd];
                    r.sound.playOnAwake = false;
                    r.sound.loop = false;
                    r.sound.spatialize = true;
                    r.sound.rolloffMode = AudioRolloffMode.Logarithmic;
                    r.sound.volume = r.volume;

                    float offset = currTime - r.startTime;
                    if( offset < r.sound.clip.length) r.sound.time = (currTime - r.startTime);
                    r.sound.Play();

                }
            }

            if ((currTime - r.startTime) > (clipLen - 0.2f))
            {
                toRemove.Add(r);
            }

        }
        foreach (rayData r in toRemove)
        {
            currentQuad.rays.Remove(r);
        }
   
#endif

#if UNITY_EDITOR
        foreach (rayData r in currentQuad.rays)
        {
            if (!(r.playing))
            {
                if (Physics.Raycast(r.ray, out hit, 100))
                {
                    if (hit.collider.gameObject.CompareTag("Player"))
                    {
                        float delay = r.distance / 340.0f;
                        r.playing = true;

                        GameObject child = new GameObject();
                        child.transform.position = r.ray.origin;
                        r.sound = child.AddComponent<AudioSource>();
                        r.sound.clip = clips[r.clipInd];
                        r.sound.playOnAwake = false;
                        r.sound.loop = false;
                        r.sound.spatialize = true;
                        r.sound.rolloffMode = AudioRolloffMode.Logarithmic;
                        r.sound.volume = r.volume;


                        float offset = currTime - r.startTime;
                        if( offset < r.sound.clip.length) r.sound.time = (currTime - r.startTime);
                        r.sound.Play();
                    }
                }

            }

            if ((currTime - r.startTime) > (clipLen - 0.2f))
            {
                toRemove.Add(r);
            }

        }

        if(prevInd != quadInd)
        {
            foreach(rayData r in sections.subspaces[prevInd].rays)
            {
                if(r.sound.isPlaying)
                {
                    r.sound.Stop();
                }
            }


        }

        foreach (rayData r in toRemove)
        {
            currentQuad.rays.Remove(r);
        }

        float end = Time.realtimeSinceStartup;
        float ellapsed = end - start;

        if(i % 1000 == 0)
        {
            Debug.Log("update time: " + ellapsed);
        }

        i++;
#endif

    }

    bool hitPlayer(Vector3 origin, Vector3 direction)
    {

        int rad = 1;

        float zInt = origin.z + (direction * (currentPos.x / origin.x)).z;
        float xInt = origin.x + (direction * (currentPos.z / origin.z)).x;


        return (zInt >= currentPos.z - rad && zInt <= currentPos.z + rad) || (xInt >= currentPos.x - rad && xInt <= currentPos.x + rad);
    }

#if !UNITY_EDITOR
    void computeSounds()
    {
        int len = currentQuad.rays.Count;
        Parallel.For(0, len, i => { toPlay[i] = hitPlayer(currentQuad.rays[i].origin, currentQuad.rays[i].direction); });
    }
    
#endif


    IEnumerator PlaySoundAfterDelay(AudioSource sound, float delay, rayData ray)
    {
        yield return new WaitForSecondsRealtime(delay);
        sound.Play();
    }
}