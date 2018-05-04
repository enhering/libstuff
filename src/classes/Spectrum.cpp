#include "Spectrum.h"

Spectrum::Spectrum() {
  std::string strOutputFileName = "FitResults.txt";
  m_OutputFile.open(strOutputFileName);

  if (! m_OutputFile.is_open()) { 
    std::cerr << "Error opening " << strOutputFileName << " for output." << std::endl;
    exit(0);
  }
}

Spectrum::~Spectrum() {
  m_OutputFile.close();
}


void Spectrum::ScanData(std::string strSearchElement) {

  m_strSearchElement = strSearchElement;

  // -------------------

  uint8_t nNumberOfDataSets = m_apcDataSet.size();

  // -------------------

  double fLambdaSearchStart = 263.873718;
  double fLambdaSearchEnd   = 700.0;

  double fLambdaMinWindowSize = 0.1;
  double fLambdaHalfWindowWidth = fLambdaMinWindowSize / 2;

  double fLambdaWindowStart = fLambdaSearchStart;

  // -------------------

  m_pcDataFit = new DataFit();
  FunctionNames eFittingFunction = VOIGT;

  // --------------------

  long nNISTIndex = 0;
  double fNISTLambda = 0;

  // --------------------

  uint8_t nDataSetNum;

  for (nDataSetNum = 0; nDataSetNum < nNumberOfDataSets; nDataSetNum += 1) {

    m_pcDataFit->SetDatasetPointer(m_apcDataSet[nDataSetNum]);

    // m_pcDataFit->EstimateDataBackground();
    // m_pcDataFit->RemoveBackground();

    // Search NIST data for first line after window start
    while (fNISTLambda < fLambdaSearchStart + fLambdaHalfWindowWidth) {
      fNISTLambda = m_pcNIST->m_acNISTDataByElement[m_strSearchElement].m_afObservedWavelength_nm[nNISTIndex];
      nNISTIndex++;
    }

    m_pcDataFit->SetFittingFunction(eFittingFunction);

    bool bLeave = false;

    do {

      std::cout << "NIST line: " << fNISTLambda << "nm " << std::endl;
      m_apcDataSet[nDataSetNum]->SetDomainWindow(fNISTLambda - fLambdaHalfWindowWidth, fNISTLambda + fLambdaHalfWindowWidth, 4);

      std::vector<double> afParam;
      afParam.clear();

      afParam.push_back(0.1); // Gaussian width
      afParam.push_back(0.1); // Lorentzian width (or vice versa. Sorry. Check CERN's TMAth, please and DataFit::Voigt
      afParam.push_back(fNISTLambda); // Voigt center
      afParam.push_back(100); // Voigt amplitude
       
      m_pcDataFit->InitializeParameters(afParam);

      m_pcDataFit->Fit();

      AnalizeFitResult(nNISTIndex);

      nNISTIndex++;

      if (nNISTIndex < m_pcNIST->m_acNISTDataByElement[m_strSearchElement].m_nNumLines) {
        fNISTLambda = m_pcNIST->m_acNISTDataByElement[m_strSearchElement].m_afObservedWavelength_nm[nNISTIndex];
      }
      
    } while ( ((fNISTLambda + fLambdaHalfWindowWidth) < fLambdaSearchEnd) ||
              (!bLeave) );
  }
}

void Spectrum::AnalizeFitResult(long nNISTIndex) {

  std::vector<double> afMinimizedParameters;
  std::vector<double> afMinimizedParameterErrors;
  m_pcDataFit->GetMinimizedParameters(afMinimizedParameters);
  m_pcDataFit->GetMinimizedParameterErrors(afMinimizedParameterErrors);

  double fNISTLambda;

  double fSigma, fSigmaError,
         fLg, fLgError,
         fCenter, fCenterError,
         fAmplitude, fAmplitudeError;

  fSigma = afMinimizedParameters[0];
  fSigmaError = afMinimizedParameterErrors[0];

  fLg      = afMinimizedParameters[1];
  fLgError = afMinimizedParameterErrors[1];

  fCenter = afMinimizedParameters[2];
  fCenterError = afMinimizedParameterErrors[2];   

  fAmplitude = afMinimizedParameters[3]; 
  fAmplitudeError = afMinimizedParameterErrors[3];

  double fChiSqr = m_pcDataFit->GetChiSqr();
  int    nStatus = m_pcDataFit->GetFitStatus();

  fNISTLambda = m_pcNIST->m_acNISTDataByElement[m_strSearchElement].m_afObservedWavelength_nm[nNISTIndex];

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

      m_OutputFile  << m_pcNIST->m_acNISTDataByElement[m_strSearchElement].m_astrIonizationLevel[nNISTIndex] << ";"
                    << m_pcNIST->m_acNISTDataByElement[m_strSearchElement].m_afObservedWavelength_nm[nNISTIndex] << ";"
                    << m_pcNIST->m_acNISTDataByElement[m_strSearchElement].m_afRitzWavelength_nm[nNISTIndex] << ";"
                    << fCenter     << "+-" << fCenterError    << ";"
                    << fLg         << "+-" << fLgError        << ";"
                    << fSigma      << "+-" << fSigmaError     << ";"
                    << fAmplitude  << "+-" << fAmplitudeError << ";"
                    << m_pcNIST->m_acNISTDataByElement[m_strSearchElement].m_afUncertainty_nm[nNISTIndex] << ";"
                    << m_pcNIST->m_acNISTDataByElement[m_strSearchElement].m_astrRelInt[nNISTIndex] << ";"
                    << m_pcNIST->m_acNISTDataByElement[m_strSearchElement].m_afAki_sec_minus_one[nNISTIndex] << ";"
                    << m_pcNIST->m_acNISTDataByElement[m_strSearchElement].m_astrAcc[nNISTIndex] << ";"
                    << m_pcNIST->m_acNISTDataByElement[m_strSearchElement].m_afEi_eV[nNISTIndex] << ";"
                    << m_pcNIST->m_acNISTDataByElement[m_strSearchElement].m_afEf_eV[nNISTIndex] << ";"
                    << m_pcNIST->m_acNISTDataByElement[m_strSearchElement].m_astrLowerLevelConf[nNISTIndex] << ";"
                    << m_pcNIST->m_acNISTDataByElement[m_strSearchElement].m_astrLowerLevelTerm[nNISTIndex] << ";"
                    << m_pcNIST->m_acNISTDataByElement[m_strSearchElement].m_astrLowerLevelJ[nNISTIndex] << ";"
                    << m_pcNIST->m_acNISTDataByElement[m_strSearchElement].m_astrUpperLevelConf[nNISTIndex] << ";"
                    << m_pcNIST->m_acNISTDataByElement[m_strSearchElement].m_astrUpperLevelTerm[nNISTIndex] << ";"
                    << m_pcNIST->m_acNISTDataByElement[m_strSearchElement].m_astrUpperLevelJ[nNISTIndex] << ";"
                    << std::endl;
    }
    else {
      std::cout << "large error of fitting parameters." << std::endl;
    }
  }
  else {
    std::cout << "Fitting failed." << std::endl;
  }
}
