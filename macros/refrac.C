#include "include/refrac.h" 

double refrac(double el = 0.5, double alt = 1)
{


  gStyle->SetOptStat(0); 
  proprf::raytrace_setup setup; 
  setup.thrown_angle = el; 
  setup.start_alt=alt; 
  proprf::raytrace_result result; 

  proprf::raytrace(&setup, &result, new TCanvas("c","refraction", 1000, 200)); 

  setup.print(); 
  result.print(); 

  return result.actual_angle - result.apparent_angle; 


}


void makeTree()
{
  TFile of("refraction_tree.root","RECREATE"); 

  proprf::raytrace_setup setup; 
  proprf::raytrace_result result; ; 

  TTree t("refraction","Refraction"); 
  t.Branch("setup",&setup); 
  t.Branch("result",&result); 

  //adjust these to your liking
  int nang = 1000; 
  double dang = 0.01; 

  int nalt = 40; 
  double dalt = 0.1; 

  for (int iang = 0; iang <= nang; iang++) 
  {
    for (int ialt = 0; ialt <= nalt; ialt++) 
    {
      double ang = dang * iang; 
      double alt = dalt * ialt; 
      setup.thrown_angle = ang; 
      setup.start_alt = alt; 

      proprf::raytrace(&setup, &result); 
      setup.print(); 
      result.print(); 

      t.Fill(); 

    }
  }


  t.Write(); 




}
