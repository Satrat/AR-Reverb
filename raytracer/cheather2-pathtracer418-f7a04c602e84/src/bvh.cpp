#include "bvh.h"

#include "CMU462/CMU462.h"
#include "static_scene/triangle.h"

#include <iostream>
#include <stack>

using namespace std;

namespace CMU462 { namespace StaticScene {

BVHAccel::BVHAccel(const std::vector<Primitive *> &_primitives,
                   size_t max_leaf_size) {

  this->primitives = _primitives;

  // TODO:
  // Construct a BVH from the given vector of primitives and maximum leaf
  // size configuration. The starter code build a BVH aggregate with a
  // single leaf node (which is also the root) that encloses all the
  // primitives.

  

  BBox bb;
  for (size_t i = 0; i < primitives.size(); ++i) {
    bb.expand(primitives[i]->get_bbox());
  }

  root = new BVHNode(bb, 0, primitives.size());
  if(primitives.size() > max_leaf_size)
    splitNode(primitives, max_leaf_size,
            root);
}

struct compareX {
  bool operator()(Primitive * p1, Primitive * p2)
  {
    BBox bb_1 = p1->get_bbox();
    BBox bb_2 = p2->get_bbox();
    Vector3D c1 = bb_1.centroid();
    Vector3D c2 = bb_2.centroid();
    
    return c1.x > c2.x;
    
  }
};

struct compareY {
  bool operator()(Primitive * p1, Primitive * p2)
  {
    BBox bb_1 = p1->get_bbox();
    BBox bb_2 = p2->get_bbox();
    Vector3D c1 = bb_1.centroid();
    Vector3D c2 = bb_2.centroid();
    
    return c1.y > c2.y;
    
  }
};

struct compareZ {
  bool operator()(Primitive * p1, Primitive * p2)
  {
    BBox bb_1 = p1->get_bbox();
    BBox bb_2 = p2->get_bbox();
    Vector3D c1 = bb_1.centroid();
    Vector3D c2 = bb_2.centroid();
    
    return c1.z > c2.z;
    
  }
};

/* Splits the root node recursively until all nodes are of
   size less than max_leaf_size. Returns false if the node
   was not split due to it already being below max_leaf_size.
   */
bool BVHAccel::splitNode(const std::vector<Primitive *> &_primitives,
                         size_t max_leaf_size,
                         BVHNode* parent)
{
   BBox bb = parent->bb;
   BBox bb_left_x;
   BBox bb_left_y;
   BBox bb_left_z;
   BBox bb_right_x;
   BBox bb_right_y;
   BBox bb_right_z;
   
   std::vector<double> right_handed_SA =
      std::vector<double>(parent->range);
      
   //Individual SAH arrays for each possible dimension
   std::vector<double> SAH_x =
      std::vector<double>(parent->range - 1);
   std::vector<double> SAH_y =
      std::vector<double>(parent->range - 1);
   std::vector<double> SAH_z =
      std::vector<double>(parent->range - 1);
   //Flag to tell which dimension has the optimal cost.
   //0 for X; 1 for Y; 2 for Z;
   int dimension_flag;
   
  //Sort along x
  std::sort(primitives.begin() + parent->start, 
            primitives.begin() + parent->start + parent->range,
            compareX());
  
  //Compute SAH_x
   
   //Iterate through backwards generating all possible
   //right-handed bounding boxes.
   //Note: index 0 is never assigned
   for(int i = parent->range - 1; i > 0; i--)
   {
      bb_right_x.expand(primitives[i + parent->start]->get_bbox());
      right_handed_SA[i] = bb_right_x.surface_area();
   } 
   
   
   //Calculate all SAH's
   for(int i = 0; i < parent->range - 2; i++)
   {
      //Can simply expand this bounding box
      bb_left_x.expand(primitives[i + parent->start]->get_bbox());
      
      
      //Compute the SAH
      double S_n = bb.surface_area();
      double S_l = bb_left_x.surface_area();
      double S_r = right_handed_SA[i+1];
      
      double C = (i + 1)*S_l + 
                 (parent->range - i - 1)*S_r;
      C /= S_n;
      SAH_x[i] = C; 
   }
   
   //Compute SAH for the Y dimension
   //Sort along y
  std::sort(primitives.begin() + parent->start, 
            primitives.begin() + parent->start + parent->range,
            compareY());
   
   //Iterate through backwards generating all possible
   //right-handed bounding boxes.
   //Note: index 0 is never assigned
   for(int i = parent->range - 1; i > 0; i--)
   {
      bb_right_y.expand(primitives[i + parent->start]->get_bbox());
      right_handed_SA[i] = bb_right_y.surface_area();
   } 
   
   
   //Calculate all SAH's
   for(int i = 0; i < parent->range - 2; i++)
   {
      //Can simply expand this bounding box
      bb_left_y.expand(primitives[i + parent->start]->get_bbox());
      
      
      //Compute the SAH
      double S_n = bb.surface_area();
      double S_l = bb_left_y.surface_area();
      double S_r = right_handed_SA[i+1];
      
      double C = (i + 1)*S_l + 
                 (parent->range - i - 1)*S_r;
      C /= S_n;
      SAH_y[i] = C; 
   }
   
   //Compute SAH for the Z dimension
   //Sort along Z
   std::sort(primitives.begin() + parent->start, 
            primitives.begin() + parent->start + parent->range,
            compareZ());
   
   //Iterate through backwards generating all possible
   //right-handed bounding boxes.
   //Note: index 0 is never assigned
   for(int i = parent->range - 1; i > 0; i--)
   {
      bb_right_z.expand(primitives[i + parent->start]->get_bbox());
      right_handed_SA[i] = bb_right_z.surface_area();
   } 
   
   
   //Calculate all SAH's
   for(int i = 0; i < parent->range - 2; i++)
   {
      //Can simply expand this bounding box
      bb_left_z.expand(primitives[i + parent->start]->get_bbox());
      
      
      //Compute the SAH
      double S_n = bb.surface_area();
      double S_l = bb_left_z.surface_area();
      double S_r = right_handed_SA[i+1];
      
      double C = (i + 1)*S_l + 
                 (parent->range - i - 1)*S_r;
      C /= S_n;
      SAH_z[i] = C; 
   }
   
   
   //Find the lowest SAH from any dimension and settle on it
   int optimal_index = 0;
   double optimal_cost = SAH_x[0];
   //x
   for(int i = 0; i < parent->range - 2; i++)
   {
      if(SAH_x[i] < optimal_cost)
      {
        dimension_flag = 0; //0 -> X
        optimal_index = i;
        optimal_cost = SAH_x[i];
      }
   }
   
   //y
   for(int i = 0; i < parent->range - 2; i++)
   {
      if(SAH_y[i] < optimal_cost)
      {
        dimension_flag = 1; //1 -> Y
        optimal_index = i;
        optimal_cost = SAH_y[i];
      }
   }
   //z
   
   for(int i = 0; i < parent->range - 2; i++)
   {
      if(SAH_z[i] < optimal_cost)
      {
        dimension_flag = 2; //2 -> Z
        optimal_index = i;
        optimal_cost = SAH_z[i];
        
      }
   }
   
   //Sort back to the dimension that is optimal
   
     
     if(dimension_flag == 0)
     {
       
       std::sort(primitives.begin() + parent->start, 
            primitives.begin() + parent->start + parent->range,
            compareX());
     }
     
     if(dimension_flag == 1)
     {
       
       std::sort(primitives.begin() + parent->start, 
            primitives.begin() + parent->start + parent->range,
            compareY());
     }
       
   
   
   
   //Recompute the left bounding box
   BBox bb_left_final;
   for(int i = 0; i < optimal_index + 1; i++)
   {
     bb_left_final.expand(primitives[i + parent->start]->get_bbox());
     
   }
   //Recompute the right bounding box
   BBox bb_right_final;
   for(int i = optimal_index + 1; i < parent->range; i++)
   {
     bb_right_final.expand(primitives[i + parent->start]->get_bbox());
      
   }
   
   
   BVHNode* left = 
     new BVHNode(bb_left_final, 
                 parent->start, 
                 optimal_index + 1);
     
     
   BVHNode* right =
     new BVHNode(bb_right_final, 
                 parent->start + optimal_index + 1, 
                 parent->range - optimal_index - 1);
                 
   parent->l = left;
   parent->r = right;
   //Recurse if necessary
   
   if(!(left->range < max_leaf_size))
      splitNode(primitives,
                max_leaf_size,
                left);
   
   if(!(right->range < max_leaf_size))
      splitNode(primitives,
                max_leaf_size,
                right);
   
   return true;
   
}

void BVHAccel::deallocNode(BVHNode * parent)
{
  if(parent->l != NULL)
    deallocNode(parent->l);
  if(parent->r != NULL)
    deallocNode(parent->r);
  delete(parent);
  return;
}

BVHAccel::~BVHAccel() {

  deallocNode(root->l);
  deallocNode(root->r);
}

BBox BVHAccel::get_bbox() const {
  return root->bb;
}

//Helper function for intersect()
bool BVHAccel::searchNode(BVHNode *parent,const Ray &ray) const
{
    if(parent->l == NULL || parent->r == NULL)
    {
      //On a leaf, so examine the polygons
      bool hit = false;
      for (size_t p = 0; p < parent->range; ++p) 
      {
        if(primitives[p + parent->start]->intersect(ray)) 
           hit = true;
      }
      return hit;
    }
    Vector3D inv = ray.inv_d;
    const int* sign = ray.sign;
    double t_min_test;
    double t_max_test;
    double t_min;
    double t_max;
    double t1;
    double t2;
    Vector3D bb_min = parent->bb.min;
    Vector3D bb_max = parent->bb.max;
    
    
    //Calculate value of t for...
    //X
    t1 = (bb_min.x - ray.o.x) * inv.x;
    t2 = (bb_max.x - ray.o.x) * inv.x;
            
    t_min = t1 < t2 ? t1 : t2;
    t_max = t1 < t2 ? t2 : t1;
    
    //in Y
    t1 = (bb_min.y - ray.o.y) * inv.y;
    t2 = (bb_max.y - ray.o.y) * inv.y;
    
    t_min_test = t1 < t2 ? t1 : t2;
    t_max_test = t1 < t2 ? t2 : t1;
    
    if(t_min_test > t_min)
      t_min = t_min_test;
    if(t_max_test < t_max)
      t_max = t_max_test; 
    
  
      
    //in Z
    t1 = (bb_min.z - ray.o.z) * inv.z;
    t2 = (bb_max.z - ray.o.z) * inv.z;
    
    t_min_test = t1 < t2 ? t1 : t2;
    t_max_test = t1 < t2 ? t2 : t1;
    
    if(t_min_test > t_min)
      t_min = t_min_test;
    if(t_max_test < t_max)
      t_max = t_max_test; 
      
    
    //Now we know if we hit the box or not
    
    if(t_max < t_min)
    {
    //Box was not hit
      return false;
    }
    if(t_min > ray.max_t || t_max < ray.min_t)
    {
      return false;
      
    }
    bool a = searchNode(parent->l, ray);
    bool b = searchNode(parent->r, ray);
    return a || b;
}


bool BVHAccel::intersect(const Ray &ray) const {

  // TODO:
  // Implement ray - bvh aggregate intersection test. A ray intersects
  // with a BVH aggregate if and only if it intersects a primitive in
  // the BVH that is not an aggregate.
  return searchNode(root, ray);

}

bool BVHAccel::searchNode(BVHNode *parent,const Ray &ray, Intersection *i) const
{
    if(parent->l == NULL || parent->r == NULL)
    {
      //On a leaf, so examine the polygons
      bool hit = false;
      for (size_t p = 0; p < parent->range; ++p) 
      {
        if(primitives[p + parent->start]->intersect(ray, i)) 
           hit = true;
      }
      return hit;
    }
    Vector3D inv = ray.inv_d;
    const int* sign = ray.sign;
    double t_min_test;
    double t_max_test;
    double t_min;
    double t_max;
    double t1;
    double t2;
    Vector3D bb_min = parent->bb.min;
    Vector3D bb_max = parent->bb.max;
    
    
    //Calculate value of t for...
    //X
    t1 = (bb_min.x - ray.o.x) * inv.x;
    t2 = (bb_max.x - ray.o.x) * inv.x;
            
    t_min = t1 < t2 ? t1 : t2;
    t_max = t1 < t2 ? t2 : t1;
    //printf("Should be guarenteed: %f, %f", t_min, t_max);
    
    //in Y
    t1 = (bb_min.y - ray.o.y) * inv.y;
    t2 = (bb_max.y - ray.o.y) * inv.y;
    
    t_min_test = t1 < t2 ? t1 : t2;
    t_max_test = t1 < t2 ? t2 : t1;
    
    if(t_min_test > t_min)
      t_min = t_min_test;
    if(t_max_test < t_max)
      t_max = t_max_test; 
    
  
      
    //in Z
    t1 = (bb_min.z - ray.o.z) * inv.z;
    t2 = (bb_max.z - ray.o.z) * inv.z;
    
    t_min_test = t1 < t2 ? t1 : t2;
    t_max_test = t1 < t2 ? t2 : t1;
    
    if(t_min_test > t_min)
      t_min = t_min_test;
    if(t_max_test < t_max)
      t_max = t_max_test; 
      
    
    //Now we know if we hit the box or not
    
    if(t_max < t_min)
    {
    //Box was not hit
      return false;
    }
    if(t_min > ray.max_t || t_max < ray.min_t)
    {
      return false;
    }
    bool a = searchNode(parent->l, ray, i);
    bool b = searchNode(parent->r, ray, i);
    return a || b;
}


bool BVHAccel::intersect(const Ray &ray, Intersection *i) const {

  // TODO:
  // Implement ray - bvh aggregate intersection test. A ray intersects
  // with a BVH aggregate if and only if it intersects a primitive in
  // the BVH that is not an aggregate. When an intersection does happen.
  // You should store the non-aggregate primitive in the intersection data
  // and not the BVH aggregate itself.
  /*bool hit = false;
  for(size_t p = 0; p < primitives.size(); ++p)
  {
     if(primitives[p]->intersect(ray, i)) 
           hit = true;
       
  }
  return hit;*/
  return searchNode(root, ray, i);
 

}

}  // namespace StaticScene
}  // namespace CMU462
