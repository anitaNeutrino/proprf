#include "atmos.h" 

#include <cmath>
#include "TMath.h"
#include "TF1.h"



/*  US atmosphere model 
 * ripped off from http://www.pdas.com/programs/atmos.f90 */ 
const double REARTH = 6369; 
const double GMR = 34.163195; 
const int NTAB= 8; 

static double htab[] = {0.0, 11.0, 20.0, 32.0, 47.0, 51.0, 71.0, 84.852  };
static double ttab[] = { 288.15, 216.65, 216.65, 228.65, 270.65, 270.65, 214.65, 186.946 };
static double ptab[] = { 1.0, 2.233611E-1, 5.403295E-2, 8.5666784E-3, 1.0945601E-3, 6.6063531E-4, 3.9046834E-5, 3.68501E-6 }; 
static double gtab[] = {-6.5, 0.0, 1.0, 2.8, 0.0, -2.8, -2.0, 0.0 }; 

int proprf::atmosphere(double alt, double* save_sigma, double * save_delta, double * save_theta) 
{
  int i,j,k; 
  double h; 
  double tgrad, tbase; 
  double tlocal; 
  double deltah; 
  double sigma, delta, theta; 


  h = alt * REARTH / (alt + REARTH); 

  i = 0; 
  j = NTAB-1; 
 
  while (j > i+1) 
  {
    k = (i+j)/2; 

    if (h < htab[k])
    {
      j = k; 
    }
    else
    {
      i = k; 
    }
  }

  tgrad = gtab[i]; 
  tbase = ttab[i]; 
  deltah = h - htab[i]; 
  tlocal = tbase + tgrad * deltah; 

  theta = tlocal / ttab[0]; 

  if (tgrad == 0) 
  {
    delta = ptab[i] * exp(- GMR*deltah / tbase); 
  }
  else
  {
    delta = ptab[i] * pow(tbase / tlocal , GMR/tgrad); 
  }

  sigma = delta/theta; 

  if (save_sigma) *save_sigma = sigma; 
  if (save_delta) *save_delta = delta; 
  if (save_theta) *save_theta = theta; 

  return 0; 
}





double proprf::temperature(double alt, double t0)
{
  double t; 
  atmosphere(alt,0, 0,&t); 
  return t0 * t; 

}

double proprf::density(double alt, double sld)
{
  double d; 
  atmosphere(alt, &d); 
  return sld * d; 

}


const double R = 8.3144986; 
const double M = 29; //molar mass of dry air
const double g0 = 9.8; 
const double X_0 = 1030; // g/cm^2 ... rough adjustment for temperature


static TF1 chapint("chapint","exp([0] - [0] * sin([1]) / sin(x)) / (sin(x) * sin(x))", 0, 180); 


double proprf::columnDepth(double height, double zenith, double T0)
{

  //must calculate scale height
  double T = temperature(height,T0); 
  double g = g0 * TMath::Power(REARTH/(height + REARTH),2); 
  double hs =  R*T / (M *g); 
//  printf("scale height: %f\n", hs); 
  double x = (REARTH + height) / hs; 
  double X0 = X_0 * 298. / T0; 

  double Xabove = X0 * exp(-height/hs); 

//  printf("Above: %f\n", Xabove); 
  if (zenith == 0) return Xabove; 

  double theta = zenith * (TMath::Pi()/ 180);  //zenith, in radians 


  double Ch = x * sin(theta); 
  chapint.SetParameter(0, x); 
  chapint.SetParameter(1, theta); 
  Ch *= chapint.Integral(0,theta); 
  return Xabove * Ch; 

}
