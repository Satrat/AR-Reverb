// Given a time between 0 and 1, evaluates a cubic polynomial with
// the given endpoint and tangent values at the beginning (0) and
// end (1) of the interval.  Optionally, one can request a derivative
// of the spline (0=no derivative, 1=first derivative, 2=2nd derivative).
template <class T>
inline T Spline<T>::cubicSplineUnitInterval(
      const T& position0,
      const T& position1,
      const T& tangent0,
      const T& tangent1,
      double normalizedTime,
      int derivative )
{
   double t = normalizedTime;
   double t2 = t*t;
   double t3 = t2*t;
   double h00, h10, h01, h11;
   switch (derivative) {
     case 0:
       h00 = 2*t3 - 3*t2 + 1;
       h10 = t3 - 2*t2 + t;
       h01 = -2*t3 + 3*t2;
       h11 = t3 - t2;
       break;
     case 1:
       h00 = 6*t2 - 6*t;
       h10 = 3*t2 - 4*t + 1;
       h01 = -6*t2 + 6*t;
       h11 = 3*t2 - 2*t;
       break;
     default:
       h00 = 12*t - 6;
       h10 = 6*t - 4;
       h01 = -12*t + 6;
       h11 = 6*t - 2;
       break;
   }
   return h00*position0 + h10*tangent0 + h01*position1 + h11*tangent1;
}
            
// Returns a state interpolated between the values directly before and after the given time.
template <class T>
inline T Spline<T>::evaluate( double time, int derivative )
{
   // TODO IMPLEMENT ME (TASK 1B)
   if (knots.empty()) return T();
   if (knots.size() == 1) return (derivative) ? T() : knots.begin()->second;
   if (knots.begin()->first > time) return (derivative) ? T() : knots.begin()->second;
   if (knots.upper_bound(time) == knots.end()) return (derivative) ? T() : prev(knots.end())->second;

   
   T p0, p1, p2, p3;
   double t0, t1, t2, t3;
   KnotIter k2 = knots.upper_bound(time);
   p2 = k2->second; t2 = k2->first;

   KnotIter k1 = prev(k2);
   p1 = k1->second; t1 = k1->first;   

   double dt = t2 - t1;
   T dp = p2 - p1;
 
   if (k1 == knots.begin()) {
     t0 = t1 - dt;
     p0 = p1 - dp;
   } else {
     KnotIter k0 = prev(k1);
     t0 = k0->first;
     p0 = k0->second;
   }

   if (next(k2) == knots.end()) {
     t3 = t2 + dt;
     p3 = p2 + dp;
   } else {
     KnotIter k3 = next(k2);
     t3 = k3->first;
     p3 = k3->second;
   }
 
   T m1 = (p2 - p0) * dt / (t2 - t0);
   T m2 = (p3 - p1) * dt / (t3 - t1);

   double norm_t = (time - t1) / dt;

   T result = cubicSplineUnitInterval(p1, p2, m1, m2, norm_t, derivative);
   return result / pow(dt, derivative);
      
}

// Removes the knot closest to the given time,
//    within the given tolerance..
// returns true iff a knot was removed.
template <class T>
inline bool Spline<T>::removeKnot(double time, double tolerance )
{
   // Empty maps have no knots.
   if( knots.size() < 1 )
   {
      return false;
   }

   // Look up the first element > or = to time.
   typename std::map<double, T>::iterator t2_iter = knots.lower_bound(time);
   typename std::map<double, T>::iterator t1_iter;
   t1_iter = t2_iter;
   t1_iter--;

   if( t2_iter == knots.end() )
   {
      t2_iter = t1_iter;
   }

   // Handle tolerance bounds,
   // because we are working with floating point numbers.
   double t1 = (*t1_iter).first;
   double t2 = (*t2_iter).first;

   double d1 = fabs(t1 - time);
   double d2 = fabs(t2 - time);


   if(d1 < tolerance && d1 < d2)
   {
      knots.erase(t1_iter);
      return true;
   }

   if(d2 < tolerance && d2 < d1)
   {
      knots.erase(t2_iter);
      return t2;
   }

   return false;
}

// Sets the value of the spline at a given time (i.e., knot),
// creating a new knot at this time if necessary.
template <class T>
inline void Spline<T>::setValue( double time, T value )
{
   knots[ time ] = value;
}

template <class T>
inline T Spline<T>::operator()( double time )
{
   return evaluate( time );
}
