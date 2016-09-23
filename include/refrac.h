#ifndef PROPRF_REFRAC_H 
#define PROPRF_REFRAC_H 

#include <stdio.h>

class TPad; 



namespace proprf
{

  /** Returns the refractivity, that is, (n-1) * 1e-6 
   *
   *  Right now this assumes relative humidity is zero, thus only the
   *  "dry term" is modeled. This should be fixed! 
   *
   *   Pressure is in millibar, temperature in Kelvin. 
   * */ 

  inline double refractivity( double P, double T) { return 77.6 * P/T ; } 

  struct raytrace_setup
  {
    raytrace_setup() 
    {
      thrown_angle = 5; 
      start_alt = 1; 
      end_alt = 37; 
      step_size = 0.01; 
      sea_level_pressure = 990; 
      sea_level_temp = 273-15; 
    }
    double thrown_angle; //degrees above horizon
    double start_alt; //km
    double end_alt; // km 
    double step_size; //km
    double sea_level_pressure; //mb; 
    double sea_level_temp; //K
    int print(FILE * f = stdout) const; 

  }; 

  struct raytrace_result
  {
    double apparent_angle; //angle where ray appears
    double actual_angle; //angle where ray actually is
    double actual_distance; //km
    int print(FILE * f = stdout) const; 
  }; 


  /** Raytraces a vector starting at start altitude and elevation to given altitude 
   *
   * For now, spherical earth assumed. Later might include a latitude parameter. 
   * 
   * */ 


  int raytrace(const raytrace_setup * setup, raytrace_result * result, TPad * draw = 0); 

}

#endif
