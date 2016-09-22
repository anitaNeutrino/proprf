#ifndef _PROPRF_GEOMAG_H
#define _PROPRF_GEOMAG_H
/* 
 *  Interface to geomag_70 mod 
 *
 *  This isn't nicely namespaced or anythign since it's a c thing. 
 */ 

#ifdef __cplusplus
extern "C" { 
#endif

typedef struct geomag_answer 
{
  double D; /*declination */
  double I; /*inclination  */
  double H; /*horizontal field strength */
  double N; /* north component */ 
  double E; /* east component  */  
  double Z; /* down component */
  double F; /* total field strength */ 

  /* derivatives of above */
  double dD, dI, dH, dN, dE, dZ, dF; 
} geomag_answer_t; 


/* hardcoded for now... */
/* altitude in km!!!! */ 

int geomag(const char * model_file, geomag_answer_t * answer, 
           int year, int month, int day, 
           double longitude, double latitude, double altitude_in_km); 


#ifdef __cplusplus
} 
#endif


#endif
