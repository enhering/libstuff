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
  double fLambdaSearchEnd   = 700.0;

  double fLambdaMinWindowSize = 0.1;
  double fLambdaHalfWindowWidth = fLambdaMinWindowSize / 2;

  double fLambdaWindowStart = fLambdaSearchStart;

  FunctionNames eFittingFunction = TRIPLEVOIGT;

  long nNISTIndex = 0;
  double fNISTLambda = 0;
  double fNISTLambdaN0, fNISTLambdaN1, fNISTLambdaN2;

  std::ofstream OutputFile;
  std::string strOutputFileName = "FitResults.txt";
  OutputFile.open(strOutputFileName);

  if (! OutputFile.is_open()) { 
    std::cerr << "Error opening " << strOutputFileName << " for output." << std::endl;
    exit(0);
  }

  // m_pcDataFit->EstimateDataBackground();
  // m_pcDataFit->RemoveBackground();

  // Search NIST data for first line after window start
  while (fNISTLambda < fLambdaSearchStart + fLambdaHalfWindowWidth) {
    fNISTLambda = m_pcNIST->m_acNISTDataByElement[strElement].m_afObservedWavelength_nm[nNISTIndex];
    nNISTIndex++;
  }

  switch (eFittingFunction) {
    case TRIPLEVOIGT:
      // Select two more lines
      if (nNISTIndex < (m_pcNIST->m_acNISTDataByElement[strElement].m_nNumLines - 3)) {
        nNISTIndex = nNISTIndex + 2;
        fNISTLambdaN0 = m_pcNIST->m_acNISTDataByElement[strElement].m_afObservedWavelength_nm[nNISTIndex - 2];
        fNISTLambdaN1 = m_pcNIST->m_acNISTDataByElement[strElement].m_afObservedWavelength_nm[nNISTIndex - 1];
        fNISTLambdaN2 = m_pcNIST->m_acNISTDataByElement[strElement].m_afObservedWavelength_nm[nNISTIndex];
        std::cout << "Searching " << fNISTLambdaN0 << "nm, " 
                                    << fNISTLambdaN1 << "nm, "
                                    << fNISTLambdaN2 << "nm:" << std::endl;
      }
      else {
        eFittingFunction = VOIGT;
        fNISTLambda = m_pcNIST->m_acNISTDataByElement[strElement].m_afObservedWavelength_nm[nNISTIndex];
      }
      break;      
  
    default: 
      break;
  }

  m_pcDataFit->SetFittingFunction(eFittingFunction);

  do {
    // std::cout << "NIST line: " << fNISTLambda << "nm ";
    switch (eFittingFunction) {
      case VOIGT:
        m_pcDataFit->SetSearchWindow(fNISTLambda - fLambdaHalfWindowWidth, fNISTLambda + fLambdaHalfWindowWidth);
        break;
      case TRIPLEVOIGT:
        m_pcDataFit->SetSearchWindow(fNISTLambdaN0 - fLambdaHalfWindowWidth, fNISTLambdaN2 + fLambdaHalfWindowWidth);
        break;
      default:
        break;
    }

    // Initialize fitting parameters
    std::vector<double> afParam;

    afParam.clear();

    switch (eFittingFunction) {
      case VOIGT:
        afParam.push_back(0.1); // Gaussian width
        afParam.push_back(0.1); // Lorentzian width (or vice versa. Sorry. Check CERN's TMAth, please and DataFit::Voigt
        afParam.push_back(fNISTLambda); // Voigt center
        afParam.push_back(100); // Voigt amplitude
        break;

      case TRIPLEVOIGT:
        afParam.push_back(0.1); // Gaussian width
        afParam.push_back(0.1); // Lorentzian width (or vice versa. Sorry. Check CERN's TMAth, please and DataFit::Voigt
        afParam.push_back(fNISTLambdaN0); // Voigt center
        afParam.push_back(100); // Voigt amplitude

        afParam.push_back(0.1); // Gaussian width
        afParam.push_back(0.1); // Lorentzian width (or vice versa. Sorry. Check CERN's TMAth, please and DataFit::Voigt
        afParam.push_back(fNISTLambdaN1); // Voigt center
        afParam.push_back(100); // Voigt amplitude

        afParam.push_back(0.1); // Gaussian width
        afParam.push_back(0.1); // Lorentzian width (or vice versa. Sorry. Check CERN's TMAth, please and DataFit::Voigt
        afParam.push_back(fNISTLambdaN2); // Voigt center
        afParam.push_back(100); // Voigt amplitude
        break;

      default:
        break;
    }
     
    m_pcDataFit->InitializeParameters(afParam);

    m_pcDataFit->Fit();

    std::vector<double> afMinimizedParameters;
    std::vector<double> afMinimizedParameterErrors;
    m_pcDataFit->GetMinimizedParameters(afMinimizedParameters);
    m_pcDataFit->GetMinimizedParameterErrors(afMinimizedParameterErrors);

    double fSigma, fSigmaError,
           fLg, fLgError,
           fCenter, fCenterError,
           fAmplitude, fAmplitudeError;

    switch (eFittingFunction) {
      case VOIGT:
        fSigma = afMinimizedParameters[0];
        fSigmaError = afMinimizedParameterErrors[0];

        fLg      = afMinimizedParameters[1];
        fLgError = afMinimizedParameterErrors[1];

        fCenter = afMinimizedParameters[2];
        fCenterError = afMinimizedParameterErrors[2];   

        fAmplitude = afMinimizedParameters[3]; 
        fAmplitudeError = afMinimizedParameterErrors[3];
        break;

      case TRIPLEVOIGT:
        // select middle voigt
        fSigma = afMinimizedParameters[4];
        fSigmaError = afMinimizedParameterErrors[4];

        fLg      = afMinimizedParameters[5];
        fLgError = afMinimizedParameterErrors[5];

        fCenter = afMinimizedParameters[6];
        fCenterError = afMinimizedParameterErrors[6];   

        fAmplitude = afMinimizedParameters[7]; 
        fAmplitudeError = afMinimizedParameterErrors[7];
        break;

      default:
        break;
    }

    double fChiSqr = m_pcDataFit->GetChiSqr();
    int    nStatus = m_pcDataFit->GetFitStatus();

    std::cout  << "Searching NIST line: " << fNISTLambda << "nm: ";

    if (nStatus == GSL_SUCCESS) {
      if ( (fCenterError < (fCenter     * 0.1)) &&
           (fabs(fNISTLambda - fCenter) < 0.05) &&
           (fLg          < 1) &&
           (fSigma       < 1) ) {

        std::cout  << "Found: " 
                   << fCenter     << "+-" << fCenterError    << ", "
                   << fLg         << "+-" << fLgError        << ", "
                   << fSigma      << "+-" << fSigmaError     << std::endl;

        OutputFile  << m_pcNIST->m_acNISTDataByElement[strElement].m_astrIonizationLevel[nNISTIndex] << ";"
                    << m_pcNIST->m_acNISTDataByElement[strElement].m_afObservedWavelength_nm[nNISTIndex] << ";"
                    << m_pcNIST->m_acNISTDataByElement[strElement].m_afRitzWavelength_nm[nNISTIndex] << ";"
                    << fCenter     << "+-" << fCenterError    << ";"
                    << fLg         << "+-" << fLgError        << ";"
                    << fSigma      << "+-" << fSigmaError     << ";"
                    << fAmplitude  << "+-" << fAmplitudeError << ";"
                    << m_pcNIST->m_acNISTDataByElement[strElement].m_afUncertainty_nm[nNISTIndex] << ";"
                    << m_pcNIST->m_acNISTDataByElement[strElement].m_astrRelInt[nNISTIndex] << ";"
                    << m_pcNIST->m_acNISTDataByElement[strElement].m_afAki_sec_minus_one[nNISTIndex] << ";"
                    << m_pcNIST->m_acNISTDataByElement[strElement].m_astrAcc[nNISTIndex] << ";"
                    << m_pcNIST->m_acNISTDataByElement[strElement].m_afEi_eV[nNISTIndex] << ";"
                    << m_pcNIST->m_acNISTDataByElement[strElement].m_afEf_eV[nNISTIndex] << ";"
                    << m_pcNIST->m_acNISTDataByElement[strElement].m_astrLowerLevelConf[nNISTIndex] << ";"
                    << m_pcNIST->m_acNISTDataByElement[strElement].m_astrLowerLevelTerm[nNISTIndex] << ";"
                    << m_pcNIST->m_acNISTDataByElement[strElement].m_astrLowerLevelJ[nNISTIndex] << ";"
                    << m_pcNIST->m_acNISTDataByElement[strElement].m_astrUpperLevelConf[nNISTIndex] << ";"
                    << m_pcNIST->m_acNISTDataByElement[strElement].m_astrUpperLevelTerm[nNISTIndex] << ";"
                    << m_pcNIST->m_acNISTDataByElement[strElement].m_astrUpperLevelJ[nNISTIndex] << ";"
                    << std::endl;
      }
      else {
        std::cout << "large error of fitting parameters." << std::endl;
      }
    }
    else {
      std::cout << "Fitting failed." << std::endl;
    }

    nNISTIndex++;

    switch (eFittingFunction) {
      case TRIPLEVOIGT:
        // Select two more lines
        if (nNISTIndex < (m_pcNIST->m_acNISTDataByElement[strElement].m_nNumLines - 3)) {
          fNISTLambdaN0 = m_pcNIST->m_acNISTDataByElement[strElement].m_afObservedWavelength_nm[nNISTIndex - 2];
          fNISTLambdaN1 = m_pcNIST->m_acNISTDataByElement[strElement].m_afObservedWavelength_nm[nNISTIndex - 1];
          fNISTLambdaN2 = m_pcNIST->m_acNISTDataByElement[strElement].m_afObservedWavelength_nm[nNISTIndex];
          std::cout << "Searching " << fNISTLambdaN0 << "nm, " 
                                    << fNISTLambdaN1 << "nm, "
                                    << fNISTLambdaN2 << "nm." << std::endl;
        }
        else {
          eFittingFunction = VOIGT;
          fNISTLambda = m_pcNIST->m_acNISTDataByElement[strElement].m_afObservedWavelength_nm[nNISTIndex];
        }
        break;      
      case VOIGT:
        if (nNISTIndex < m_pcNIST->m_acNISTDataByElement[strElement].m_nNumLines) {
          fNISTLambda = m_pcNIST->m_acNISTDataByElement[strElement].m_afObservedWavelength_nm[nNISTIndex];
        }
        break;

      default: 
        break;
    }
    

    // std::cin.get();
    
  } while ((fNISTLambda + fLambdaHalfWindowWidth) < fLambdaSearchEnd);

  OutputFile.close();
}

