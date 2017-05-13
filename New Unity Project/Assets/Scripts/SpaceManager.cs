using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.VR.WSA;

public class SpaceManager : MonoBehaviour {

    public static SpaceManager Instance = null;
    public int numSubs = 8;
    public int rows = 2;
    public int cols = 4;
    public int spaceSize = 10;

    private SurfaceObserver surfaceObserver;
    public System.Collections.Generic.Dictionary<int, Quadrant> subspaces = new System.Collections.Generic.Dictionary<int, Quadrant>();
    private System.Collections.Generic.Dictionary<SurfaceId, GameObject> spatialMeshObjects = new System.Collections.Generic.Dictionary<SurfaceId, GameObject>();
    private Bounds worldBounds = new Bounds();

    private int width;
    private int height;


    // Use this for initialization

    private void Awake()
    {
        if (Instance == null)
        {
            Instance = this;
        }
        width = spaceSize / cols;
        height = spaceSize / rows;

        surfaceObserver = new SurfaceObserver();
        surfaceObserver.SetVolumeAsAxisAlignedBox(Vector3.zero, new Vector3(spaceSize, spaceSize, spaceSize));
        for (int i = 0; i < numSubs; i++)
        {

            Bounds newBound = new Bounds(new Vector3((i%cols)*width + width/2, 5, (i/cols)*height + height/2), new Vector3(spaceSize/cols, 5, spaceSize/rows));
            Quadrant quad = new Quadrant(newBound);
            subspaces[i] = quad;
        }


    }
    void Start() {

    }

    
   public int locToQuad(Vector3 pos)
   {

        for(int i = 0; i < subspaces.Count; i++)
        {
            if(subspaces[i].bounds.Contains(pos))
            {
                return i;
            }

        }

        return 0;
   }

    //maps rays to the quadrants they intersect
    //utilizes SIMD
    public void sendToQuad(List<rayData> data)
    {

        for (int i = 0; i < numSubs; i++)
        {
            float centerx = subspaces[i].bounds.center.x;
            float centerz = subspaces[i].bounds.center.z;
            float x = subspaces[i].bounds.extents.x;
            float z = subspaces[i].bounds.extents.z;

            Mono.Simd.Vector4f centers = new Mono.Simd.Vector4f(centerx, centerz, centerx, centerz);
            Mono.Simd.Vector4f offsets = new Mono.Simd.Vector4f(-x, -z, x, z);

            Mono.Simd.Vector4f boundaries = centers + offsets;


            foreach (rayData r in data)
            {
                float xs = r.ray.origin.x;
                float zs = r.ray.origin.z;

                float xd = r.ray.direction.x;
                float zd = r.ray.direction.z;

                Mono.Simd.Vector4f origins = new Mono.Simd.Vector4f(xs, zs, xs, zs);
                Mono.Simd.Vector4f dirs = new Mono.Simd.Vector4f(xd, zd, xd, zd);

                Mono.Simd.Vector4f t = boundaries - origins;
                t = t / dirs;

                if( !(t[1] > t[2] || t[0] > t[3]))
                {
                    subspaces[i].addRay(r);
                }


            }
        }
    }


    public Quadrant getPlayerQuadrant()
    {
        return subspaces[locToQuad(Camera.main.transform.position)];
    }



   private void OnSurfaceChanged(SurfaceId surfaceId, SurfaceChange changeType, Bounds bounds, System.DateTime updateTime)
    {
        float start = Time.realtimeSinceStartup;

        switch (changeType)
        {
            case SurfaceChange.Added:
            case SurfaceChange.Updated:
                Vector3 closePoint = (bounds.center + bounds.extents.magnitude * (bounds.center - worldBounds.center));
                if (!worldBounds.Contains(closePoint))
                {
                    worldBounds.Encapsulate(closePoint);
                }
                if (!spatialMeshObjects.ContainsKey(surfaceId))
                {
                    spatialMeshObjects[surfaceId] = new GameObject("spatial-mapping-" + surfaceId);
                    spatialMeshObjects[surfaceId].transform.parent = this.transform;
                    spatialMeshObjects[surfaceId].AddComponent<MeshRenderer>();
                }
                GameObject target = spatialMeshObjects[surfaceId];
                SurfaceData sd = new SurfaceData(
                    //the surface id returned from the system
                    surfaceId,
                    //the mesh filter that is populated with the spatial mapping data for this mesh
                    target.GetComponent<MeshFilter>() ?? target.AddComponent<MeshFilter>(),
                    //the world anchor used to position the spatial mapping mesh in the world
                    target.GetComponent<WorldAnchor>() ?? target.AddComponent<WorldAnchor>(),
                    //the mesh collider that is populated with collider data for this mesh, if true is passed to bakeMeshes below
                    target.GetComponent<MeshCollider>() ?? target.AddComponent<MeshCollider>(),
                    //triangles per cubic meter requested for this mesh
                    1000,
                    //bakeMeshes - if true, the mesh collider is populated, if false, the mesh collider is empty.
                    true
                    );
                surfaceObserver.RequestMeshAsync(sd, OnDataReady);
                break;
            case SurfaceChange.Removed:
                var obj = spatialMeshObjects[surfaceId];
                spatialMeshObjects.Remove(surfaceId);
                if (obj != null)
                {
                    GameObject.Destroy(obj);
                }
                break;
            default:
                break;
        }

        float end = Time.realtimeSinceStartup;
        float ellapsed = end - start;
        Debug.Log("Time on mesh update: " + ellapsed);
    }

    private void OnDataReady(SurfaceData bakedData, bool outputWritten, float elapsedBakeTimeSeconds)
    {
        int quad = locToQuad(bakedData.outputMesh.transform.position);

        subspaces[quad].insertMesh(bakedData.id, spatialMeshObjects[bakedData.id]);
        subspaces[quad].rerollRays();

    }

    private void insertSource(GameObject source)
    {
        subspaces[locToQuad(source.transform.position)].insertSource(source);
    }

    // Update is called once per frame
    void Update () {
		
	}
}
