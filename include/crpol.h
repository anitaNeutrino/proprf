#ifndef _PROPRF_CRPOL_H
#define _PROPRF_CRPOL_H


class Adu5Pat; 
class TVector3; 

namespace proprf
{




  double getPolPlaneUpgoing( const Adu5Pat * payload, double column_depth_g_cm2,
                     double phiWave, double thetaWave, 
                     double * latitude_at_column_depth = 0, double * longitude_at_column_depth = 0, 
                     double * height_at_column_depth = 0, TVector3 * Bfield_at_column_depth = 0, double * distance_to_column_depth = 0,  double hstep_in_km = 0.1, 
                     const char * coefficients_file = "IGRF12.COF"); 



}





#endif
