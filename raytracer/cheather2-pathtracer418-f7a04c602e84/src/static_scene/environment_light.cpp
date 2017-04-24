#include "environment_light.h"

namespace CMU462 { namespace StaticScene {


  bool compare_pdf(EnvironmentLight::pdf p1, EnvironmentLight::pdf p2)
  {
    return p1.prob < p2.prob;   
  }
  
  bool compare_pdf_double(const double &d,
                         const EnvironmentLight::pdf &p)
  {
    return d < p.prob;
  }
  
  double get_area(int j, int max)
  {
     double theta = ((double) j) / ((double) max);
     theta *= PI;
     return sin(theta);
  }
  
EnvironmentLight::EnvironmentLight(const HDRImageBuffer* envMap)
    : envMap(envMap) {
    
    marginal.resize(envMap->w);
    conditional.resize(envMap->w);
    
    //Missnomer: is total_flux
    double total_illum;
    for(int i = 0; i < envMap->w; i++)
    {
       conditional[i].resize(envMap->h);
       for(int j = 0; j < envMap->h; j++)
       {
           total_illum += envMap->data[i + envMap->w * j].illum() *
                          get_area(j, envMap->h);
       }
    }
    
    //Fill marginal array
    for(int i = 0; i < envMap->w; i++)
    {
       marginal[i].prob = 0.0;
       marginal[i].index = i;
       for(int j = 0; j < envMap->h; j++)
       {
           marginal[i].prob += 
                envMap->data[i + envMap->w * j].illum() /
                total_illum *
                get_area(j, envMap->h);
       }
    }
    
    //Fill conditional arrays CDF
    for(int i = 0; i < envMap->w; i++)
    {
       double cdf = 0.0;
       for(int j = 0; j < envMap->h; j++)
       {
           cdf += envMap->data[i + envMap->w * j].illum() /
                  total_illum * (1.0 / marginal[i].prob) *
                  get_area(j, envMap->h);
           
           conditional[i][j].index = j;
           
           conditional[i][j].prob = cdf;
       }    
    }
    
    //Replace marginal probabilities with CDF of the marginal
    //probabilities.
    double cdf = 0.0;
    for(int i = 0; i < envMap->h; i++)
    {
        
        cdf += marginal[i].prob;
        marginal[i].prob = cdf;        
    }
    
    //Sort all of the conditionals
    for(int i = 0; i < conditional.size(); i++)
    {
    
       std::sort(conditional[i].begin(),
                 conditional[i].begin() + conditional[i].size(),
                 compare_pdf);
    }
    //Sort the marginal
    std::sort(marginal.begin(),
              marginal.begin() + marginal.size(),
              compare_pdf);      
}
Spectrum EnvironmentLight::sample_L(const Vector3D& p, Vector3D* wi,
                                    float* distToLight,
                                    float* pdf) const {
  // TODO: Implement
  //Generate a random direction;
  //Old Code
  /*
  UniformHemisphereSampler3D sampleHem;
  Vector3D r = sampleHem.get_sample();
  if(std::rand() % 2 == 0)
     r.z *= -1.0;
  
  
  
  double x_normxz;
  double z_normxz;
  double theta;
  double phi;
  
  double denom = sqrt(r.x*r.x + 
                        r.z*r.z);
  if(denom != 0.0)
  {
     x_normxz = r.x / 
                   denom;
     z_normxz = r.z /
                   denom;
                        
     //randomDir.z = cos(theta)
     theta = acos(r.y);  
  
     //Converts (x,y) on circle ->phi
     phi = atan2(x_normxz, z_normxz) + PI;
     if(phi < 0.0 || phi > 2.0 * PI)
        printf("%f\n", phi);
  }
  else 
  {
     //pointing either up or down
     theta = r.y > 0.0 ? PI : 0.0;
     phi = 0.0;
  }
  double x = phi / (2.0 * PI) * envMap->w;
  double y = theta / (PI) * envMap->h;
  int xlow = floor(x);
  int xhigh = floor(x) + 1;
  int ylow = floor(y);
  int yhigh = floor(y) + 1;
  if(yhigh >= envMap->h)
     yhigh -= envMap->h;
  if(xhigh >= envMap->w)
     xhigh -= envMap->w;
  //Get samples
  Spectrum sample_hr = envMap->data[xhigh + envMap->w*yhigh]; 
  Spectrum sample_hl = envMap->data[xlow + envMap->w*yhigh];
  Spectrum sample_br = envMap->data[xhigh + envMap->w*ylow];
  Spectrum sample_bl = envMap->data[xlow + envMap->w*ylow];
  
  
  double w_weight = x - floor(x);
  double h_weight = y - floor(y);

  Spectrum sample_h = (1.0 - w_weight) * sample_hl + w_weight * sample_hr;
  Spectrum sample_b = (1.0 - w_weight) * sample_bl + w_weight * sample_br;
  Spectrum sample_final = (1.0 - h_weight) * sample_b + h_weight * sample_h;
  
  //Update pointer fields
  *pdf = 1.0 / 4 / PI;
  *wi = r;
  return sample_final;
  
  */
  
  //Sample using a random number
  UniformGridSampler2D sampler;
  Vector2D marg_cond = sampler.get_sample();
  //Search through the marginal cdf of one sample
  EnvironmentLight::pdf marginal_node;
  EnvironmentLight::pdf conditional_node;
  
  
  //Binary search on marginal CDF
  int start = 0;
  int end = marginal.size() - 1;
  while(end >= start)
  {
     int test = (start + end) / 2;
     
     if(marg_cond.x == marginal[test].prob)
        break;
     if(marg_cond.x > marginal[test].prob)
        start = test + 1;
     else
        end = test - 1;
  }
  if(start == marginal.size())
     start -= 1;
  marginal_node = marginal[start];
  std::vector<EnvironmentLight::pdf> 
       specific_conditional = conditional[marginal_node.index];
  
  //Binary search on the conditional CDFs
  start = 0;
  end = specific_conditional.size() - 1;
  while(end >= start)
  {
     int test = (start + end) / 2;
     
     if(marg_cond.y == specific_conditional[test].prob)
        break;
     if(marg_cond.y > specific_conditional[test].prob)
        start = test + 1;
     else
        end = test - 1;
  }
  if(start == specific_conditional.size())
     start -= 1;
  conditional_node = specific_conditional[start];
  
  int i = marginal_node.index;
  
  int j = conditional_node.index;
  
  
  
  //Angle calculations
  double theta = PI * ((double) j) / ((double) envMap->h);
  double phi = 2 * PI * ((double) i) / ((double) envMap->w);
  double y = sin(theta);
  double x = cos(theta) * sin(phi);
  double z = cos(theta) * cos(phi);
  
  
  //Remember side-effects
  *pdf = marginal_node.prob * conditional_node.prob;
  *wi = Vector3D(x, y, z);
  *distToLight = INFINITY;
  
  return envMap->data[i + envMap->w * j]; 
  
}

Spectrum EnvironmentLight::sample_dir(const Ray& r) const {
  // TODO: Implement
  double x_normxz;
  double z_normxz;
  double theta;
  double phi;
  
  double denom = sqrt(r.d.x*r.d.x + 
                        r.d.z*r.d.z);
  if(denom != 0.0)
  {
     x_normxz = r.d.x / 
                   denom;
     z_normxz = r.d.z /
                   denom;
                        
     //randomDir.z = cos(theta)
     theta = acos(r.d.y);  
  
     //Converts (x,y) on circle ->phi
     phi = atan2(x_normxz, z_normxz) + PI;
     if(phi < 0.0 || phi > 2.0 * PI)
        printf("%f\n", phi);
  }
  else 
  {
     //pointing either up or down
     theta = r.d.y > 0.0 ? PI : 0.0;
     phi = 0.0;
  }
  double x = phi / (2.0 * PI) * envMap->w;
  double y = theta / (PI) * envMap->h;
  int xlow = floor(x);
  int xhigh = floor(x) + 1;
  int ylow = floor(y);
  int yhigh = floor(y) + 1;
  if(yhigh >= envMap->h)
     yhigh -= envMap->h;
  if(xhigh >= envMap->w)
     xhigh -= envMap->w;
  //Get samples
  Spectrum sample_hr = envMap->data[xhigh + envMap->w*yhigh]; 
  Spectrum sample_hl = envMap->data[xlow + envMap->w*yhigh];
  Spectrum sample_br = envMap->data[xhigh + envMap->w*ylow];
  Spectrum sample_bl = envMap->data[xlow + envMap->w*ylow];
  
  
  double w_weight = x - floor(x);
  double h_weight = y - floor(y);

  Spectrum sample_h = (1.0 - w_weight) * sample_hl + w_weight * sample_hr;
  Spectrum sample_b = (1.0 - w_weight) * sample_bl + w_weight * sample_br;
  Spectrum sample_final = (1.0 - h_weight) * sample_b + h_weight * sample_h;
  return sample_final;
  
}

} // namespace StaticScene
} // namespace CMU462
