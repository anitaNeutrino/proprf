#include "refrac.h" 
#include "atmos.h" 
#include <cmath>
#include "TVector3.h" 
#include "TPad.h" 
#include "TH2.h" 
#include "TGraph.h" 
#include "TPaveText.h" 


const double REARTH = 6369; 

/** For drawing */ 
static std::vector<double> xs; 
static std::vector<double> ys; 
static TGraph earth; 
static TGraph ice; 
static TGraph trajectory; 
static TGraph los; 
static TPaveText txt(0.15,0.7,0.5,0.9,"NDC"); 
static TH2I axis("axis","Refraction Trajectory", 10, -900,900, 10, REARTH-20, REARTH+50); 





int proprf::raytrace(const raytrace_setup * setup, raytrace_result * result, TPad * draw) 
{

  double P0, T0; 
  double P1, T1; 
  double el = setup->thrown_angle * M_PI/180; 
  double N0,N1; 
  double n0,n1; 
  double h = setup->start_alt; 

  //We are assuming a spherical Earth here, so put somewhere on sphere
  TVector3 x(0,0,REARTH+h);
  TVector3 x0 = x; 
  TVector3 l(cos(el),0,sin(el)); 
  atmosphere(h, 0, &P0, &T0); 
  P0 *= setup->sea_level_pressure; 
  T0 *= setup->sea_level_temp; 
  N0 = refractivity(P0,T0); 
  n0 = N0*1e-6 + 1; 


  std::vector<double> xs; 
  std::vector<double> ys; 


  if (draw) 
  {
    draw->cd(); 
    axis.Draw(); 
    xs.clear(); 
    ys.clear(); 


    if (earth.GetN() == 0) 
    {
      earth.Set(200); 
      ice.Set(200); 
      double phi_start = 15*M_PI/32;
      double phi_step = M_PI/16/199; 

      for (int i = 0; i < 200; i++) 
      {
        double phi = phi_start + i * phi_step ; 
        earth.GetX()[i]  = REARTH * cos(phi); 
        earth.GetY()[i]  = REARTH * sin(phi); 
        ice.GetX()[i]  = (REARTH+h) * cos(phi); 
        ice.GetY()[i]  = (REARTH+h) * sin(phi); 
 
      }

      earth.SetFillColor(11); 
      ice.SetFillColor(38); 
    }
    ice.Draw("lf"); 
    earth.Draw("lf"); 
  }



  while (h < setup->end_alt) 
  {

    if (draw) 
    {
      xs.push_back(x.x()); 
      ys.push_back(x.z()); 
    }

    x+=l * setup->step_size; 
    h = x.Mag() - REARTH; 
    atmosphere(h, 0, &P1, &T1); 
    P1 *= setup->sea_level_pressure; 
    T1 *= setup->sea_level_temp; 
    N1 = refractivity(P1,T1); 
    n1 = N1*1e-6 + 1; 
    double r = n0/n1; 

    //  This is the normal AFTER the step ,but whatever it shouldn't matter too much. We could split the difference and put it in the middle, I guess
    TVector3 norml = -x.Unit(); //

    double c = -norml.Dot(l); 

    //new direction vector
    //From Wikipedia's vector equation for Snell's law. 
    l = r * l +  (r*c - sqrt(1-r*r*(1-c*c))) * norml; 

    n0=n1; 
  }

  if (draw) 
  {
    xs.push_back(x.x()); 
    ys.push_back(x.z()); 
    trajectory.Set(xs.size()); 
    memcpy(trajectory.GetX(), &xs[0], xs.size() * sizeof(double)); 
    memcpy(trajectory.GetY(), &ys[0], ys.size() * sizeof(double)); 

    los.Set(2); 
    los.GetX()[0] = x0.x(); 
    los.GetX()[1] = x.x(); 
    los.GetY()[0] = x0.z(); 
    los.GetY()[1] = x.z(); 

    axis.GetXaxis()->SetRangeUser(-0.1 * x.x(), x.x() *1.1);
    axis.GetYaxis()->SetRangeUser(REARTH-10, x.z() +10);
    axis.GetXaxis()->SetTitle("km"); 
    axis.GetYaxis()->SetTitle("km"); 
    trajectory.SetLineColor(2); 
    trajectory.Draw("lsame"); 
    los.SetLineColor(3); 
    los.Draw("lsame"); 
    
  }


  result->apparent_angle = 90- x.Angle(l) * 180/M_PI; 
  TVector3 diff = x-x0; 
  result->actual_angle = 90- x.Angle(diff) * 180/M_PI; 
  result->actual_distance = diff.Mag(); 


  if (draw)
  {
    txt.Clear(); 
    txt.AddText(TString::Format("Thrown angle: %g", setup->thrown_angle)); 
    txt.AddText(TString::Format("true (straight-line) angle: %g, apparent angle: %g", result->actual_angle, result->apparent_angle)); 
    txt.AddText(TString::Format("start alt: %g km, stop alt: %g km", setup->start_alt, setup->end_alt)); 
    txt.AddText(TString::Format("LOS distance %g km", result->actual_distance)); 
    txt.Draw() ; 
  }




  return 0; 
}




int proprf::raytrace_result::print(FILE * f) const
{
  int ret = 0; 
  ret += fprintf(f, "raytrace_result at %p:\n" ,this); 
  ret += fprintf(f, "  apparent angle: %g\n", apparent_angle); 
  ret += fprintf(f, "  actual (LOS) angle:: %g \n", actual_angle); 
  ret += fprintf(f, "  actual distance: %g km\n", actual_distance); 
  return ret; 
}



int proprf::raytrace_setup::print(FILE * f) const
{
  int ret = 0; 
  ret += fprintf(f, "raytrace_setup at %p:\n" ,this); 
  ret += fprintf(f, "  thrown angle: %g\n", thrown_angle); 
  ret += fprintf(f, "  start alt: %g km\n", start_alt); 
  ret += fprintf(f, "  end alt: %g km\n", end_alt); 
  ret += fprintf(f, "  step size: %g km\n", step_size); 
  ret += fprintf(f, "  sea level P: %g mb\n", sea_level_pressure); 
  ret += fprintf(f, "  sea level T: %g K\n", sea_level_temp); 
  return ret; 
}



