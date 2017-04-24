#include "sphere.h"

#include <cmath>

#include  "../bsdf.h"
#include "../misc/sphere_drawing.h"

namespace CMU462 { namespace StaticScene {

bool Sphere::test(const Ray& r, double& t1, double& t2) const {

  // TODO:
  // Implement ray - sphere intersection test.
  // Return true if there are intersections and writing the
  // smaller of the two intersection times in t1 and the larger in t2.
  Vector3D p = r.o;
  Vector3D c = o;
  Vector3D n = r.d;
  
  
  double quad_a = n.x*n.x + n.y*n.y + n.z*n.z;
  double quad_b = 2 * 
                 (n.x * (p.x - c.x) +
                  n.y * (p.y - c.y) +
                  n.z * (p.z - c.z));
  double quad_c = p.x*p.x + p.y*p.y + p.z*p.z +
                  c.x*c.x + c.y*c.y + c.z*c.z +
             (-2) * (p.x * c.x + p.y * c.y + p.z * c.z)
             - r2;
  
  double det = quad_b*quad_b - 4*quad_a*quad_c;
            
  if(det < 0.0)
    return false;
  
  double root = sqrt(det);
  
  double test1 = (-quad_b + root) / (2*quad_a);
  double test2 = (-quad_b - root) / (2*quad_a);
  
  t1 = test1 < test2 ? test1 : test2;
  t2 = test1 < test2 ? test2 : test1;
  
  return true;

}

bool Sphere::intersect(const Ray& r) const {

  // TODO:
  // Implement ray - sphere intersection.
  // Note that you might want to use the the Sphere::test helper here.
  double dummy;
  double& dummy_ref = dummy;
  
  return test(r, dummy, dummy);

}

bool Sphere::intersect(const Ray& r, Intersection *i) const {

  // TODO:
  // Implement ray - sphere intersection.
  // Note again that you might want to use the the Sphere::test helper here.
  // When an intersection takes place, the Intersection data should be updated
  // correspondingly.
  
  double front;
  double back;
  double front_ref = front;
  double back_ref = back;
  
  //Rename variables
  Vector3D p = r.o;
  Vector3D c = o;
  Vector3D n = r.d;
  
  if(!test(r, front, back))
    return false;
    
  if(front < r.min_t || r.max_t < front ||
     front >= i->t)
  {
     //Back might still be valid
     if(back < r.min_t || r.max_t < back ||
       back >= i->t)
       return false;
     i->t = back;
     r.max_t = back;
     Vector3D normal = ((p + back*n) - c);
  
     normal.normalize();
     i->n = normal;
     i->primitive = this;
     i->bsdf = get_bsdf();
     return true;
  }
  
  i->t = front;
  r.max_t = front;
  Vector3D normal = ((p + front*n) - c);
  
  normal.normalize();
  i->n = normal;
  i->primitive = this;
  i->bsdf = get_bsdf();
  return true;
  
}

void Sphere::draw(const Color& c) const {
  Misc::draw_sphere_opengl(o, r, c);
}

void Sphere::drawOutline(const Color& c) const {
    //Misc::draw_sphere_opengl(o, r, c);
}


} // namespace StaticScene
} // namespace CMU462
