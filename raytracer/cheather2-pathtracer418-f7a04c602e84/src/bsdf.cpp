#include "bsdf.h"

#include <iostream>
#include <algorithm>
#include <utility>
 
#include "sampler.h"

using std::min;
using std::max;
using std::swap;

namespace CMU462 {

void make_coord_space(Matrix3x3& o2w, const Vector3D& n) {

    Vector3D z = Vector3D(n.x, n.y, n.z);
    Vector3D h = z;
    if (fabs(h.x) <= fabs(h.y) && fabs(h.x) <= fabs(h.z)) h.x = 1.0;
    else if (fabs(h.y) <= fabs(h.x) && fabs(h.y) <= fabs(h.z)) h.y = 1.0;
    else h.z = 1.0;

    z.normalize();
    Vector3D y = cross(h, z);
    y.normalize();
    Vector3D x = cross(z, y);
    x.normalize();

    o2w[0] = x;
    o2w[1] = y;
    o2w[2] = z;
}

// Diffuse BSDF //

Spectrum DiffuseBSDF::f(const Vector3D& wo, const Vector3D& wi) {
  return albedo * (1.0 / PI);
}

Spectrum DiffuseBSDF::sample_f(const Vector3D& wo, Vector3D* wi, float* pdf) {
  
  Vector3D randomHemisphere = this->sampler.get_sample();
  *wi = randomHemisphere;
  *pdf = (0.5 / PI);
  return albedo * (0.5 / PI);
          
}

// Mirror BSDF //

Spectrum MirrorBSDF::f(const Vector3D& wo, const Vector3D& wi) {
    if(wo.x == -wi.x ||
       wo.y == -wi.y ||
       wo.z == wi.z)
       return reflectance;
    else
       return Spectrum(0.0, 0.0, 0.0);
  
}

Spectrum MirrorBSDF::sample_f(const Vector3D& wo, Vector3D* wi, float* pdf) {

  // TODO:
  // Implement MirrorBSDF
  Vector3D direct_in;
  reflect(wo, &direct_in);
  *wi = direct_in;
  *pdf = 1.f;
  return reflectance ;
}

// Glossy BSDF //

/*
Spectrum GlossyBSDF::f(const Vector3D& wo, const Vector3D& wi) {
  return Spectrum();
}

Spectrum GlossyBSDF::sample_f(const Vector3D& wo, Vector3D* wi, float* pdf) {
  *pdf = 1.0f;
  return reflect(wo, wi, reflectance);
}
*/

// Refraction BSDF //

Spectrum RefractionBSDF::f(const Vector3D& wo, const Vector3D& wi) {
  
}

Spectrum RefractionBSDF::sample_f(const Vector3D& wo, Vector3D* wi, float* pdf) {

  // TODO:
  // Implement RefractionBSDF

  return Spectrum();
}

// Glass BSDF //

Spectrum GlassBSDF::f(const Vector3D& wo, const Vector3D& wi) {
  Vector3D wi_test;
  float pdf;
  sample_f(wo, &wi_test, &pdf);
  if(wi == wi_test)
    return transmittance;
  
}

Spectrum GlassBSDF::sample_f(const Vector3D& wo, Vector3D* wi, float* pdf) {

  // TODO:
  // Compute Fresnel coefficient and either reflect or refract based on it.
  Vector3D direct_in;
  
  
  
  
  UniformGridSampler2D sampler;
  if(refract(wo, &direct_in, ior))
  {
     //Decide whether to refract or reflect
     //Calculate Fresnel
     double nt_i;
     double ni_t;
     double ni_i;
     double nt_t;
     
     if(wo.z > 0.0)
     //Switch n_i and n_t if necessary
     //And account for sign
     {
         nt_i = wo.z;
         
         ni_t = -ior*direct_in.z;
         
         ni_i = ior*wo.z;
         nt_t = -direct_in.z;
     }
     else
     {
         nt_i = -ior*wo.z;
         ni_t = direct_in.z;
         ni_i = -wo.z;
         nt_t = ior*direct_in.z;    
     }
     
     double r_par = (nt_i - ni_t) / (nt_i + ni_t);
     double r_perp = (ni_i - nt_t) / (ni_i + nt_t);
     
     double F = 0.5 * (r_par*r_par + r_perp*r_perp);
     
     if(sampler.get_sample().x < F)
     {
        reflect(wo, &direct_in);
        *pdf = 1.0;
     }
     else
     {
        *pdf = 1.0; //Do nothing, already calculate refractance vector.
     }
  }
  else
  {
     //Can only reflect
     *pdf = 1.0;
     reflect(wo, &direct_in);
  }
  *wi = direct_in;
  
  return transmittance;
}

void BSDF::reflect(const Vector3D& wo, Vector3D* wi) {

  // TODO:
  // Implement reflection of wo about normal (0,0,1) and store result in wi.
  *wi = Vector3D(-wo.x, -wo.y, wo.z);

}

bool BSDF::refract(const Vector3D& wo, Vector3D* wi, float ior) {

  // TODO:
  // Use Snell's Law to refract wo surface and store result ray in wi.
  // Return false if refraction does not occur due to total internal reflection
  // and true otherwise. When dot(wo,n) is positive, then wo corresponds to a
  // ray entering the surface through vacuum.
  
  double cos_theta_i = wo.z;
  bool inside_flag = false;
  
  if(wo.z > 0.0)
  {
     inside_flag = true;
     ior = 1.0 / ior;
  }
  
  double det = 1.0 - (1.0 - cos_theta_i*cos_theta_i) * (ior*ior);
  if(det <= 0.0)
  {
    return false;
  }
  
  //System of equations:
  //x^2 + y^2 + z^2 = 1
  //z =  constant
  //y = kx
  //maintain quadrant of (x,y)
  
  double cos_theta_t = sqrt(det);
  double x2 = wo.x*wo.x;
  double y2 = wo.x*wo.x;
  double z2 = cos_theta_t * cos_theta_t;
  //y = kx;
  double k;
  if(wo.x == 0)
  {
     double y = sqrt(1-z2);
     if(wo.y < 0)
       y *= -1.0;
     *wi = Vector3D(0.f, y, cos_theta_t);
     return true;
  }
  if(z2 == 1.0)
  {
    *wi = Vector3D(0.0f, 0.0f, 1.f);
    return true;
  }
  k = wo.y / wo.x;
  
  double k2 = k*k;
  double x = sqrt((1.0-z2) / (1 + k2));
  double y = sqrt(1.0 - x*x - z2);
  
  
  //Adjust signs
  if(wo.x > 0.0)
    x *= -1.0;
  if(wo.y > 0.0)
    y *= -1.0;
  //Z needs to be pointing through the surface
  double z = cos_theta_t;
  if(inside_flag)
    z*= -1.0;
  
  
  
  
  
  *wi = Vector3D(x, y, z);
  
  
  return true;
  
  
  
  
  

}

// Emission BSDF //

Spectrum EmissionBSDF::f(const Vector3D& wo, const Vector3D& wi) {
  return Spectrum();
}

Spectrum EmissionBSDF::sample_f(const Vector3D& wo, Vector3D* wi, float* pdf) {
  *wi  = sampler.get_sample(pdf);
  return Spectrum();
}

} // namespace CMU462
