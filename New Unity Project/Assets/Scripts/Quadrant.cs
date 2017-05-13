using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using HoloToolkit.Unity.SpatialMapping;
using UnityEngine.VR.WSA;
using System;

public class Quadrant : MonoBehaviour {

    public Bounds bounds;
    public Vector3 center;
    public Vector3 dimmensions;

    private GameObject supersource;

    private SpatialMappingManager mapper;
    private RemoteMappingManager remoteMapper;
    private System.Collections.Generic.List<GameObject> sources = new System.Collections.Generic.List<GameObject>();
    private System.Collections.Generic.Dictionary<SurfaceId, GameObject> spatialMeshObjects = new System.Collections.Generic.Dictionary<SurfaceId, GameObject>();

    public System.Collections.Generic.List<rayData> rays = new System.Collections.Generic.List<rayData>();

    //initialize quadrant
    public Quadrant(Bounds bound)
    {
        bounds = bound;
        center = bound.center;
        dimmensions = bound.size;
        supersource = new GameObject();
        supersource.transform.position = center;
        
    }

    // Use this for initialization
    void Start () {

		if(SpatialMappingManager.Instance != null)
        {
            mapper = SpatialMappingManager.Instance;
        }
        if(RemoteMappingManager.Instance != null)
        {
            remoteMapper = RemoteMappingManager.Instance;
        } 
	}

    public void insertMesh(SurfaceId ID, GameObject newMesh) {
        spatialMeshObjects[ID] = newMesh;
    }

    public void insertSource(GameObject source)
    {
        center *= sources.Count;
        sources.Add(source);
        center += source.transform.position; 
        center /= sources.Count;
        rerollRays();
    }

    internal void removeRay(rayData r)
    {
        rays.Remove(r);
        Destroy(r);
    }

    internal void addRay(rayData data)
    {
        rays.Add(data);
    }

    internal bool intersectsRay(Ray ray)
    {
        return bounds.IntersectRay(ray);
    }

    public void removeSource(GameObject source)
    {
        center *= sources.Count;
        sources.Remove(source);
        center -= source.transform.position; 
        center /= sources.Count;
        rerollRays();
    }

    internal void rerollRays()
    {
        return;
    }
}
