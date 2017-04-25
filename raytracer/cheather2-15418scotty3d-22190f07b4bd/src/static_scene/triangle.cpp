#include "triangle.h"

#include "CMU462/CMU462.h"
#include "GL/glew.h"

namespace CMU462 { namespace StaticScene {

Triangle::Triangle(const Mesh* mesh, vector<size_t>& v) :
    mesh(mesh), v(v) { }
Triangle::Triangle(const Mesh* mesh, size_t v1, size_t v2, size_t v3) :
    mesh(mesh), v1(v1), v2(v2), v3(v3) { }

BBox Triangle::get_bbox() const {
  
  // TODO: 
  // compute the bounding box of the triangle
  Vector3D p0 = mesh->positions[v1];
  Vector3D p1 = mesh->positions[v2];
  Vector3D p2 = mesh->positions[v3];
  
  Vector3D min = p0;
  
  min.x = p1.x < min.x ? p1.x : min.x;
  min.x = p2.x < min.x ? p2.x : min.x;
  
  min.y = p1.y < min.y ? p1.y : min.y;
  min.y = p2.y < min.y ? p2.y : min.y;
  
  min.z = p1.z < min.z ? p1.z : min.z;
  min.z = p2.z < min.z ? p2.z : min.z;
  
  Vector3D max = p0;
  
  max.x = p1.x > max.x ? p1.x : max.x;
  max.x = p2.x > max.x ? p2.x : max.x;
  
  max.y = p1.y > max.y ? p1.y : max.y;
  max.y = p2.y > max.y ? p2.y : max.y;
  
  max.z = p1.z > max.z ? p1.z : max.z;
  max.z = p2.z > max.z ? p2.z : max.z;
  
  return BBox(min, max);
}

bool Triangle::intersect(const Ray& r) const {
  
  // TODO: implement ray-triangle intersection
  
  Vector3D p0 = mesh->positions[v1];
  Vector3D p1 = mesh->positions[v2];
  Vector3D p2 = mesh->positions[v3];
  
  Vector3D m0 = p1 - p0;
  Vector3D m1 = p2 - p0;
  Vector3D m2 = -r.d;
  
  Matrix3x3 M;
  M[0] = m0;
  M[1] = m1;
  M[2] = m2;
  
  Matrix3x3 M_inv = M.inv();
  
  Vector3D uvt = M_inv * (r.o - p0);
  
  double u = uvt.x;
  double v = uvt.y;
  double t = uvt.z;
  
  if(u < 0.0 || v < 0.0 ||
     1.0 < u + v ||
     t < r.min_t || r.max_t < t)
    return false;
  else
    return true;
}



void barycentric(Vector3D n, Vector3D p,
                 Vector3D v1, Vector3D v2, Vector3D v3,  
                 double &a, double &b, double &g)
{
   double area =  dot(n, cross(v2 - v1, v3 - v1));
   double areaA = dot(n, cross(v2 - p, v3 - p));
   double areaB = dot(n, cross(v3 - p, v1 - p));
   
   a = areaA/area;
   b = areaB/area;
   g = 1.0 - a - b;
}


bool Triangle::intersect(const Ray& r, Intersection *isect) const {
  
  // TODO: 
  // implement ray-triangle intersection. When an intersection takes
  // place, the Intersection data should be updated accordingly
  
  Vector3D p0 = mesh->positions[v1];
  Vector3D p1 = mesh->positions[v2];
  Vector3D p2 = mesh->positions[v3];
  
  Vector3D m0 = p1 - p0;
  Vector3D m1 = p2 - p0;
  Vector3D m2 = -r.d;
  
  Matrix3x3 M;
  M[0] = m0;
  M[1] = m1;
  M[2] = m2;
  
  Matrix3x3 M_inv = M.inv();
  
  Vector3D uvt = M_inv * (r.o - p0);
  
  double u = uvt.x;
  double v = uvt.y;
  double t = uvt.z;
  
  if(u < 0.0 || v < 0.0 ||
     1.0 < u + v ||
     t < r.min_t || r.max_t < t ||
     t >= isect->t)
     
    return false;
  
  Vector3D n0 = mesh->normals[v1];
  Vector3D n1 = mesh->normals[v2];
  Vector3D n2 = mesh->normals[v3];
  Vector3D norm = cross(m0,m1);
  norm.normalize();
  
  bool flip_flag =  false;
  
  double a;
  double b;
  double g;
  barycentric(norm, r.o + t * r.d,
              p0, p1, p2,
              a, b, g);
  
  norm = n0*a + n1*b + n2*g;
  norm.normalize();
  
  if(dot(norm, r.d) > 0.0)
    flip_flag = true;
  
  
  
  r.max_t = t;
  
  isect->t = t;
  if(flip_flag)
    norm *= -1.0;
  
  
  isect->n = norm;
  isect->primitive = this;
  isect->bsdf = mesh->get_bsdf();
  return true;
}

void Triangle::draw(const Color& c) const {
  glColor4f(c.r, c.g, c.b, c.a);
  glBegin(GL_TRIANGLES);
  glVertex3d(mesh->positions[v1].x,
             mesh->positions[v1].y,
             mesh->positions[v1].z);
  glVertex3d(mesh->positions[v2].x,
             mesh->positions[v2].y,
             mesh->positions[v2].z);
  glVertex3d(mesh->positions[v3].x,
             mesh->positions[v3].y,
             mesh->positions[v3].z);
  glEnd();
}

void Triangle::drawOutline(const Color& c) const {
  glColor4f(c.r, c.g, c.b, c.a);
  glBegin(GL_LINE_LOOP);
  glVertex3d(mesh->positions[v1].x,
             mesh->positions[v1].y,
             mesh->positions[v1].z);
  glVertex3d(mesh->positions[v2].x,
             mesh->positions[v2].y,
             mesh->positions[v2].z);
  glVertex3d(mesh->positions[v3].x,
             mesh->positions[v3].y,
             mesh->positions[v3].z);
  glEnd();
}



} // namespace StaticScene
} // namespace CMU462
