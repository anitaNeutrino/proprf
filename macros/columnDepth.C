

void columnDepth(double alt = 20) 
{

gSystem->Load("lib/libcrpol.so"); 

  TGraph * g = new TGraph(100); 

  for (int i = 0; i < 100; i++) 
  {
    g->SetPoint(i, i, crpol::columnDepth(alt,i)); 
  }


  g->Draw(); 


}
