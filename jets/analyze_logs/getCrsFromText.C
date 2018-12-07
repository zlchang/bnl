int getCrsFromText(const char* textcrs = "ptbin.crs.txt", const char* dir="../output_pt", char *ver = "text.full")
{
  TFile *fout = new TFile(Form("ptbin.info.%s.root",  ver), "recreate");
  const float ptbin[14] = {2, 3, 4, 5, 7, 9, 11, 15, 20, 25, 35, 45, 55, 100};
  TH1F *hcrs = new TH1F("crs", "partonic_cross_section;p_{T}", 13, ptbin);
  TH1F *hnevent = new TH1F("nevents", "partonic_nevents;p_{T}", 13, ptbin);
  TH1F *hweight = new TH1F("weight", "partonic_weight;p_{T}", 13, ptbin);
  char pt[16];
  ifstream in(textcrs);
  float crs;
  int bin = 1;
  ofstream of(Form("%s.info.%s.txt", textcrs, ver));
  cout<<setw(10)<<"Bin"<<setw(10)<<"Xsection"<<setw(10)<<"Nevents"<<endl;
  of<<setw(10)<<"Bin"<<setw(10)<<"Xsection"<<setw(10)<<"Nevents"<<endl;
  while(in >> pt >> crs){
    ifstream in_num(Form("%s/%s.nevents.txt", dir, pt));
    char junks[32];
    int nevents;
    in_num>>junks>>nevents;
    in_num.close();
    if(nevents == 0) {bin++; continue;}
    float weight = crs/(nevents+0.);
    hcrs->SetBinContent(bin, crs);
    hnevent->SetBinContent(bin, nevents);
    hweight->SetBinContent(bin, weight);
    cout<<setw(10)<<pt<<setw(10)<<setprecision(2)<<crs<<setw(10)<<nevents<<endl;
    of<<setw(10)<<pt<<setw(10)<<setprecision(2)<<crs<<setw(10)<<nevents<<endl;
    bin++;
  }
  of.close();
  in.close();
  fout->Write();
  fout->Close();

  return 1;
}

