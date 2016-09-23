void do_events() 
{

  gSystem->Load("lib/libcrpol.so"); 

  TFile f("data/upgoing.root"); 

  TTree * t = (TTree*) f.Get("upgoing"); 


  Adu5Pat * pat =0 ;
  double phi = 0; 
  double theta = 0; 
  int event = 0; 


  t->SetBranchAddress("pat",&pat); 
  t->SetBranchAddress("phi",&phi); 
  t->SetBranchAddress("theta",&theta); 
  t->SetBranchAddress("event",&event); 


  for (int i = 0; i < t->GetEntries(); i++) 
  {
    t->GetEntry(i); 

    double lat, lon, alt,dist; 
    TVector3 B; 
    if (theta < -10) continue;  //oops should not have included non-upgoing events in tree! 

    printf("==============================================================================================\n"); 
    printf("EVENT: %d\n", event); 
    printf("   location:  lat: %g, long: %g, alt: %g km\n", pat->latitude, pat->longitude, pat->altitude/1e3); 
    printf("   heading of payload: %g\n", pat->heading); 
    printf("   heading of event: %g\n", pat->heading - phi); 
    printf("   zenith: %g\n",90-theta); 
    double Xp = crpol::columnDepth(pat->altitude/1e3, 90-theta); 
    printf("   X at payload: %f\n", Xp); 

    double pol_angle = crpol::getPolPlaneUpgoing(pat, Xp, phi, theta, &lat, &lon, &alt, &B,&dist,0.01); 
    printf("   Bfield at payload (in Cartesian): (%g, %g, %g) \n", B.x(), B.y(), B.z()); 
    printf("     Pol angle from Bfield at payload: %g \n ", pol_angle); 

    double Xs[4] = {100,300,800,1200}; 
    for (int xi = 0; xi < 4; xi++)
    {
    double pol_angle = crpol::getPolPlaneUpgoing(pat, Xs[xi], phi, theta, &lat, &lon, &alt, &B,&dist, 0.01); 
      printf("\n\t(X=%g):\n\t\talt: %g km\t lat: %g\t lon: %g\t dist: %g km \t pol_angle: %g\n\n", Xs[xi], alt, lat, lon,dist, pol_angle); 
    }
  }
}
