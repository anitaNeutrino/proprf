#ifndef _DENS_H
#define _DENS_H


#include "TVector3.h" 

namespace proprf
{

  /** Returns density of Earth at the radius according to PREM in g/cm^34*/ 
  double earthdensity(double r_in_km); 

  /** Returns column density (g/cm^2) of chord through Earth where x0 is a point on the vector and dir is the direction. dx is integration step in km */ 
  double chordThroughEarth(const TVector3  & x0, const TVector3 & dir, double dx = 0.1, double * distance = 0); 

  /** Returns column density (g/cm^2) of chord through corresponding to payload altitude (in km) and downward angle (in degrees). dx is integration step in km. */
  double chordThroughEarth(double height = 37, double angle = 27, double dx = 0.1, double * distance = 0); 

  /** Returns number of intersection points of origin-centered sphere of radius r with line with point x0 and direction dir. If point0 and point1 are non-null, they are set to
   * intersection points (unless there are no intersection points, in which case they are untouched, or just one intersection point where just point1 is touched). */
  int sphereLineIntersection(const TVector3 & x0, const TVector3 & dir, double r = 6371, TVector3 * point0  = 0, TVector3 * point1 = 0); 



}; 

#endif
