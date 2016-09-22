#ifndef _ATMOSPHERE_H
#define _ATMOSPHERE_H

namespace proprf 
{

  // US standard model . Returns values relative to sea-level! 
  int atmosphere(double alt_in_km, double * density_over_sea_level = 0, double * pressure_over_sea_level = 0, double * temperature_over_sea_level = 0); 
  double density(double alt_in_km, double sea_level_density = 1.225 * (273 + 15.) / (273-15.));
  double temperature(double alt_in_km, double sea_level_temperature = 273-15);


  // Returns column depth relative to column depth above, using approximations to Chapman's formula
  double columnDepth(double height, double zenith, double T0 = 273-15); 

}

#endif
