using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using HoloToolkit.Unity.SpatialMapping;

//keeps track of the users current quadrant 
//raycasts all rays that pass through their quadrant to check for hit
public class userUpdates : MonoBehaviour
{

    private Vector3 currentPos;
    private SpaceManager sections;
    private Quadrant currentQuad;
    private int quadInd;
    public AudioClip clip;

    public List<rayData> toRemove;

    private int i = 0;


    // Use this for initialization
    void Start () {

        clip = Resources.Load("dog") as AudioClip;
        currentPos = transform.position;
        toRemove = new List<rayData>();

        if(SpaceManager.Instance != null)
        {
            sections = SpaceManager.Instance;
            quadInd = sections.locToQuad(currentPos);
            currentQuad = sections.subspaces[quadInd];
        }

        ///while (sections == null)
        //{

        //}

        //currentPlaying = new List<AudioSource>();
        
    }

    // Update is called once per frame
    void Update()
    {

        float start = Time.realtimeSinceStartup;

        currentPos = transform.position;
        int prevInd = quadInd;
        quadInd = sections.locToQuad(currentPos);

        //only start new clips if we move
        // TO DO FOR SARA: cross fade between old and new sounds
        //this is an approximation of how it will work I'll fix it later promise

        currentQuad = sections.subspaces[quadInd];

        RaycastHit hit;

        float currTime = Time.realtimeSinceStartup;

        float clipLen = clip.length;

        foreach (rayData r in currentQuad.rays)
        {
            if (!(r.playing))
            {
                if (Physics.Raycast(r.ray, out hit, 100))
                {
                    if (hit.collider.gameObject.CompareTag("Player"))
                    {
                        float delay = r.distance / 340.0f;
                        //nextPlaying.Add(r.sound);
                        //r.sound.volume = 0.0f;
                        r.playing = true;

                        GameObject child = new GameObject();
                        child.transform.position = r.ray.origin;
                        AudioSource sound = child.AddComponent<AudioSource>();
                        sound.clip = Resources.Load("dog") as AudioClip;
                        sound.playOnAwake = false;
                        sound.loop = false;
                        sound.spatialize = true;
                        sound.rolloffMode = AudioRolloffMode.Linear;
                        sound.volume = r.volume;
                        float offset = Time.realtimeSinceStartup - currTime;
                        sound.time = (offset - r.startTime);
                        Debug.Log("TAP TO SOUND: " + sound.time);
                        //StartCoroutine(PlaySoundAfterDelay(r, delay, r.ray.origin, r.volume));
                        sound.Play();
                        //StartCoroutine(PlaySoundAfterDelay(sound, delay, r));


                    }
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

        float end = Time.realtimeSinceStartup;
        float ellapsed = end - start;

        if(i % 100 == 0)
        {
            Debug.Log("update time: " + ellapsed);
        }

        i++;


    }


    IEnumerator PlaySoundAfterDelay(AudioSource sound, float delay, rayData ray)
    {
        yield return new WaitForSecondsRealtime(delay);
        //AudioSource.PlayClipAtPoint(ray.sound.clip, loc, vol);
        sound.Play();
        currentQuad.rays.Remove(ray);
    }
}
