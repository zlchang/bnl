void testNeighbor()
{
  gROOT->Macro("loadMuDst.C");
  gROOT->Macro("LoadLogger.C");

  gSystem->Load("StEmcUtil");
  StEmcGeom *geom = StEmcGeom::instance("bemc");
  TH1D *h = new TH1D("h", "h", 10, 0, 10);
  StEmcPosition EmcPosition;
  for(int it = 0; it < 4800; it++){
    int exitTowerId = it+1;
    int counter = 0;
    for(int ieta = -1; ieta <= 1; ieta++){
      for(int iphi = -1; iphi <=1; iphi++){
	int ntowerId = EmcPosition.getNextTowerId(exitTowerId, ieta, iphi);
	if(ntowerId <=0 || ntowerId > 4800) {
	  float eta,phi;
	  geom->getEtaPhi(exitTowerId, eta, phi);
	  Printf("tower %d neighboring towerId invalid Id: %d with ieta = %d and iphi = %d\n", exitTowerId, ntowerId, ieta, iphi); 
	  Printf("eta = %f, phi = %f\n", eta, phi);
	  counter++;
	}
      }
      h->Fill(counter);
      //Printf("ntowerId=%d, exitTowerId=%d, nee=%4.3lf, et=%4.3lf\n", ntowerId, exitTowerId, nee, ee);       
    }
  }
  h->Print("all");
}

