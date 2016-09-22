#include "crpol.h"
#include "atmos.h"
#include "geomag.h"
#include "TTimeStamp.h"
#include "assert.h"

#include "Adu5Pat.h"
#include "AnitaGeomTool.h" 

#define DEG2RAD M_PI / 180 
#define RAD2DEG 180 / M_PI 



double proprf::getPolPlaneUpgoing( const Adu5Pat * payload, 
                                  double wanted_depth, double phi, double theta, 
                                  double * lat, double * lon, double * h, TVector3 * B, double * distance, 
                                  double hstep, const char * coeff_file)  
{

  assert(getenv("PROPRF_DATA_DIR")); 


  double height = payload->altitude/1e3; 


  double source_lon, source_lat; 


  TVector3 payload_location; 

  double p[3]; 
  AnitaGeomTool::Instance()->getCartesianCoords( payload->latitude, payload->longitude, payload->altitude, p); 
  payload_location.SetXYZ(p[0],p[1],p[2]); 


  TVector3 localvec;
  localvec.SetMagThetaPhi(1,(90-theta) * DEG2RAD, (phi - payload->heading) * DEG2RAD);  //TODO: check signs!!! 
//  propvec.Print(); 
  TVector3 propvec = localvec;
  propvec.RotateUz(payload_location.Unit()); 
//  propvec.Print(); 


  TVector3 current_location  = payload_location; 


  double zenith = payload_location.Angle(propvec) * RAD2DEG; 
  double column_depth = proprf::columnDepth(height, zenith); 
//  printf ("%f %f %f %f\n", theta, height, column_depth, zenith); 

  bool passed_us = column_depth < wanted_depth; 
  if (passed_us) hstep *=-1; 

  while (true) 
  {
    if (passed_us && column_depth >= wanted_depth)
    {
      break; 
    }
    if (!passed_us && column_depth <= wanted_depth) 
    {
      break; 
    }

    current_location += propvec * (hstep * 1e3); 

    zenith = current_location.Angle(propvec) * RAD2DEG; 
    current_location.GetXYZ(p); 
    
    AnitaGeomTool::Instance()->getLatLonAltFromCartesian(p, source_lat, source_lon, height); 
    height /= 1000; 

    column_depth = proprf::columnDepth(height, zenith  ); 
    assert(!isnan(column_depth)); 
//    printf("%f %f %f\n", height, column_depth, zenith ); 
    if (height <= 0) 
    {
      fprintf(stderr,"Oops, we hit the ground!\n"); 
      return -999; 
    }
  }


  if (lat) *lat = source_lat; 
  if (lon) *lon = source_lon; 
  if (h) *h = height; 
  if (distance) *distance = (current_location - payload_location).Mag() / 1e3 * ( passed_us ? -1 : 1);  // m -> tkm

  geomag_answer mag; 
  TString f; 
  TTimeStamp t(payload->realTime); 
  unsigned month, day, year; 
  t.GetDate(true,0, &year, &month, &day); 

  f.Form("%s/%s", getenv("PROPRF_DATA_DIR"),coeff_file); 
  geomag(f.Data(), &mag, year, month, day, source_lon, source_lat, height); 

  TVector3 Bfield(-mag.N, mag.E, -mag.Z);  //TODO: check sign conventions :(

  Bfield.RotateUz(current_location.Unit()); 

  if (B) 
  {
    *B = Bfield; 
  }
  

  TVector3 crvec = -propvec; 

  TVector3 polvec = crvec.Cross(Bfield); 

  return polvec.Angle(payload_location) * RAD2DEG - 90; 


}
