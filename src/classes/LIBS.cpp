#include "LIBS.h"

LIBS::LIBS() {
  m_pcDataFit = new DataFit();
}

LIBS::~LIBS() {

}

void LIBS::LoadData(std::string strFileName) {
  
  std::string   strLine;
  std::ifstream InputFile;

  std::cout << "Input data file is: " << strFileName << std::endl;

  InputFile.open(strFileName);

  if (! InputFile.is_open()) { 
    std::cerr << "Error opening " << strFileName << " for output." << std::endl;
    exit(0);
  }

  float fLambda, fIntensity;

  // TCanvas *c1 = new TCanvas("c1","c1",10,10,1000,500);
  // c1->Divide(1,2);
  // c1->cd(1); 

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

    // pcTomatoHistogram->Fill(fLambda, fIntensity);

    m_pcDataFit->AddDataPoint(fLambda, fIntensity, 1.0);
  }
}

void LIBS::ScanData(std::string strElement) {
  double fLambdaSearchStart = 263.873718;
  double fLambdaSearchEnd   = 400.0;

  double fLambdaMinWindowSize = 0.2;
  double fLambdaHalfWindowWidth = fLambdaMinWindowSize / 2;

  double fLambdaWindowStart = fLambdaSearchStart;

  // Search NIST data for first line after window start
  long nNISTIndex = 0;
  double fNISTLambda = 0;

  while (fNISTLambda < fLambdaSearchStart + fLambdaHalfWindowWidth) {
    fNISTLambda = m_pcNIST->m_acNISTDataByElement[strElement].m_afObservedWavelength_nm[nNISTIndex];
    nNISTIndex++;
  }

  std::ofstream OutputFile;
  std::string strOutputFileName = "FitData.txt";
  OutputFile.open(strOutputFileName);

  if (! OutputFile.is_open()) { 
    std::cerr << "Error opening " << strOutputFileName << " for output." << std::endl;
    exit(0);
  }

  m_pcDataFit->SetFittingFunction(VOIGT);

  do {
  
    std::cout << "NIST line: " << fNISTLambda << "nm ";
    m_pcDataFit->SetSearchWindow(fNISTLambda - fLambdaHalfWindowWidth, fNISTLambda + fLambdaHalfWindowWidth);

    std::vector<double> afParam = { 1.0, fNISTLambda, 0.001, 1.0, 0.001, 0.5 };
    m_pcDataFit->InitializeParameters(afParam);
    m_pcDataFit->Fit();

    double fAmplitude = m_pcDataFit->GetGaussianAmplitude();
    double fCenter    = m_pcDataFit->GetGaussianCenter();
    double fWidth     = m_pcDataFit->GetGaussianWidth();
    double fChiSqr    = m_pcDataFit->GetChiSqr();
    int    nStatus    = m_pcDataFit->GetFitStatus();

    if (nStatus == GSL_SUCCESS) {
      OutputFile << fNISTLambda << " "
                 << fCenter     << " "
                 << fAmplitude  << " "
                 << fWidth      << " "
                 << fChiSqr     << std::endl;
    }

    nNISTIndex++;
    fNISTLambda     = m_pcNIST->m_acNISTDataByElement[strElement].m_afObservedWavelength_nm[nNISTIndex];
    
  } while ((fNISTLambda + fLambdaHalfWindowWidth) < fLambdaSearchEnd);

  OutputFile.close();
}

