#include "LIBS.h"

LIBS::LIBS() {
  m_pcDataFit = new DataFit();
}

LIBS::~LIBS() {

}

void LIBS::LoadData(std::string strFileName) {
  
  std::string   strLine;
  std::ifstream InputFile;

  std::cout << "Input LIBS data file is: " << strFileName << std::endl << std::endl;

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

    m_pcDataFit->AddDataPoint(fLambda, fIntensity, 1.0);
  }
}

void LIBS::ScanData(std::string strElement) {
  double fLambdaSearchStart = 263.873718;
  double fLambdaSearchEnd   = 400.0;

  double fLambdaMinWindowSize = 0.1;
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
  std::string strOutputFileName = "FitResults.txt";
  OutputFile.open(strOutputFileName);

  if (! OutputFile.is_open()) { 
    std::cerr << "Error opening " << strOutputFileName << " for output." << std::endl;
    exit(0);
  }

  // m_pcDataFit->EstimateDataBackground();
  // m_pcDataFit->RemoveBackground();

  FunctionNames eFittingFunction = VOIGT;

  m_pcDataFit->SetFittingFunction(eFittingFunction);

  do {

    std::cout << "NIST line: " << fNISTLambda << "nm ";
    m_pcDataFit->SetSearchWindow(fNISTLambda - fLambdaHalfWindowWidth, fNISTLambda + fLambdaHalfWindowWidth);

    std::vector<double> afParam = { 0.1, 0.1, 100, fNISTLambda };
    m_pcDataFit->InitializeParameters(afParam);

    m_pcDataFit->Fit();

    std::vector<double> afMinimizedParameters;
    std::vector<double> afMinimizedParameterErrors;
    m_pcDataFit->GetMinimizedParameters(afMinimizedParameters);
    m_pcDataFit->GetMinimizedParameterErrors(afMinimizedParameterErrors);

    double fSigma = afMinimizedParameters[0];
    double fSigmaError = afMinimizedParameterErrors[0];

    double fLg      = afMinimizedParameters[1];
    double fLgError = afMinimizedParameterErrors[1];

    double fAmplitude = afMinimizedParameters[2];
    double fAmplitudeError = afMinimizedParameterErrors[2];

    double fCenter = afMinimizedParameters[3];
    double fCenterError = afMinimizedParameterErrors[3];    

    double fChiSqr = m_pcDataFit->GetChiSqr();
    int    nStatus = m_pcDataFit->GetFitStatus();

    if (nStatus == GSL_SUCCESS) {
      if (fCenterError < (fCenter * 0.1)) {

        std::cout  << "  Good fit: "
                   << fCenter     << "+-" << fCenterError << ", "
                   << fLg         << "+-" << fLgError << std::endl;
      }
    }

    nNISTIndex++;
    fNISTLambda     = m_pcNIST->m_acNISTDataByElement[strElement].m_afObservedWavelength_nm[nNISTIndex];
    
  } while ((fNISTLambda + fLambdaHalfWindowWidth) < fLambdaSearchEnd);

  OutputFile.close();
}

