#include "dens.h" 
#include "TGraph.h"
#include "assert.h"
#include "TMath.h"


static TGraph * gEarth = 0; 

double proprf::earthdensity(double r) 
{

  assert(getenv("proprf_DATA_DIR")); 


  if (gEarth == 0)
  {
    gEarth = new TGraph(TString::Format("%s/prem_earth.dat",getenv("proprf_DATA_DIR"))); 
  }

  return gEarth->Eval(r); 
}

double proprf::chordThroughEarth(double h, double ang, double dx, double * distance ) 
{
  TVector3 pos(0,0, 6371 + h); 
  double theta = ang * TMath::Pi()/180; 
  TVector3 l(0, cos(theta), -sin(theta)); 

  return chordThroughEarth(pos,l, dx,distance); 

}

double proprf::chordThroughEarth(const TVector3  & x0, const TVector3 & dir, double dx, double * dist ) 
{
  TVector3 p0,p1; 
  int n = sphereLineIntersection(x0,dir,6371, &p0,&p1); 
  if (n < 2) return 0; 
   

  TVector3 dp = (p1 - p0); 
  double distance = dp.Mag(); 
  if (dist) *dist = distance; 

  dp = dp.Unit(); 

  double answer = 0; 

  double dx_in_cm = dx * 1e5; 
  double d = 0; 
  while (d < distance)
  {
    answer += dx_in_cm * earthdensity((p0 + d * dp).Mag()); 
    d += dx; 
  }

  return answer; 
}
int proprf::sphereLineIntersection(const TVector3 & o, const TVector3 & dir, double r,  TVector3 * point0, TVector3 * point1 )
{

  TVector3 l = dir.Unit(); 

  double a = 1; 
  double b = 2 * ( l.Dot(o)); 
  double c = o.Dot(o) - r*r; 

  double insqrt = b*b - 4 *a*c; 

  if (insqrt < 0) return 0; 


  double d0 = (-b + sqrt(insqrt))/(2*a); 
  if (insqrt == 0)
  {
    if (point0)
    {
      *point0 = d0 * l + o; 
    }
    return 1; 
  }
  double d1 = (-b - sqrt(insqrt))/(2*a); 

  if (point0) 
  {
    *point0 = d0 * l + o; 
  }
  if (point1) 
  {
    *point1 = d1 * l + o; 
  }

  return 2; 
}



