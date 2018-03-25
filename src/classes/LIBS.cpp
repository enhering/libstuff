#include "LIBS.h"

LIBS::LIBS() {

}

LIBS::~LIBS() {

}

void LIBS::LoadData(std::string strFileName) {
  
  std::ifstream InputFile;
  std::string   strLine;

  InputFile.open(strFileName);

  TH1F * pcTomatoHistogram = new TH1F("pcTomatoHistogram", "LIBS data", 42000, 260, 450);

  if (! InputFile.is_open()) { 
    std::cerr << "Error opening " << strFileName << " for output." << std::endl;
    exit(0);
  }

  float fLambda, fIntensity;

  TCanvas *c1 = new TCanvas("c1","c1",10,10,1000,700);
  c1->Divide(1,2);
  c1->cd(1); 

  while (getline (InputFile, strLine)) {
    std::string strCol01, strCol02;
    std::string strSeparator = ";";
    std::size_t nPos1, nPos2;

    nPos2 = strLine.find(strSeparator, 0);
    strCol01 = strLine.substr(0, nPos2);
    ltrim(strCol01); rtrim(strCol01);

    strCol02 = strLine.substr(nPos2 + 1, strLine.size() - nPos2);
    ltrim(strCol02); rtrim(strCol02);

    // std::cout << "1: " << strCol01 << " 2: " << strCol02 << std::endl;

    fLambda = std::stof(strCol01);
    fIntensity = std::stof(strCol02);

    pcTomatoHistogram->Fill(fLambda, fIntensity);
  }
  pcTomatoHistogram->Draw();

  TH1F *h2 = (TH1F*)pcTomatoHistogram->Clone("h2");
  //Use TSpectrum to find the peak candidates
  TSpectrum *s = new TSpectrum(1000);

  Int_t nfound = s->Search(pcTomatoHistogram, 1 ,"new");
  printf("Found %d candidate peaks to fitn",nfound);
  c1->Update();
  c1->cd(2);

  // //estimate linear background
  // TF1 *fline = new TF1("fline","pol1",0,1000);
  // h->Fit("fline","qn");
  // //Loop on all found peaks. Eliminate peaks at the background level
  // par[0] = fline->GetParameter(0);
  // par[1] = fline->GetParameter(1);
  // npeaks = 0;
  // Float_t *xpeaks = s->GetPositionX();
  // for (p=0;p<nfound;p++) {
  //   Float_t xp = xpeaks[p];
  //   Int_t bin = h->GetXaxis()->FindBin(xp);
  //   Float_t yp = h->GetBinContent(bin);
  //   if (yp-TMath::Sqrt(yp) < fline->Eval(xp)) continue;
  //   par[3*npeaks+2] = yp;
  //   par[3*npeaks+3] = xp;
  //   par[3*npeaks+4] = 3;
  //   npeaks++;
  // }
  // printf("Found %d useful peaks to fitn",npeaks);
  // printf("Now fitting: Be patientn");
  // TF1 *fit = new TF1("fit",fpeaks,0,1000,2+3*npeaks);
  // TVirtualFitter::Fitter(h2,10+3*npeaks); //we may have more than the default 25 parameters
  // fit->SetParameters(par);
  // fit->SetNpx(1000);
  // h2->Fit("fit"); 


}