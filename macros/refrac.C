#include "include/refrac.h" 

double refrac(double el = 0.5, double alt = 1)
{


  gStyle->SetOptStat(0); 
  proprf::raytrace_setup setup; 
  setup.thrown_angle = el; 
  setup.start_alt=alt; 
  proprf::raytrace_result result; 

  proprf::raytrace(&setup, &result, new TCanvas); 

  setup.print(); 
  result.print(); 

  return result.actual_angle - result.apparent_angle; 


}
