/* root tester for geomag */ 

extern "C"
{
#include "geomag.h"
}
#include  <cstdio>
#include <cstdlib>

int main(int nargs, char ** args)
{


  int year = 2010; 
  int day = 20; 
  int month = 10; 

  double alt = 35; 
  double lon = 10; 
  double lat = -75; 

  const char * file = "IGRF12.COF"; 

  char * cmd; 
  asprintf(&cmd, "./geomag70.exe %s %04d,%02d,%02d D K%g %g %g", file, year, month, day, alt, lat, lon); 

  system(cmd); 

  geomag_answer_t answer; 

  geomag(file, &answer, year, month, day, lon, lat, alt); 

  printf( "ANSWER: \n"); 

  printf(" Decl: %f\n", answer.D); 
  printf(" Incl: %f\n", answer.I); 
  printf(" H: %f\n", answer.H); 
  printf(" N: %f\n", answer.N); 
  printf(" E: %f\n", answer.E); 
  printf(" Z: %f\n", answer.Z); 
  printf(" F: %f\n", answer.F); 
  printf(" dH: %f\n", answer.dH); 
  printf(" dN: %f\n", answer.dN); 
  printf(" dE: %f\n", answer.dE); 
  printf(" dZ: %f\n", answer.dZ); 
  printf(" dF: %f\n", answer.dF); 



}




