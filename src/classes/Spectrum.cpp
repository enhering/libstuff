#include "Spectrum.h"

Spectrum::Spectrum() {

}

Spectrum::~Spectrum() {

}

void Spectrum::ScanData(std::string strElement) {

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
  double fNISTLambdaN0, fNISTLambdaN1, fNISTLambdaN2;

  // --------------------
  std::cout << "1" << std::endl;

  std::ofstream OutputFile;
  std::string strOutputFileName = "FitResults.txt";
  OutputFile.open(strOutputFileName);

  if (! OutputFile.is_open()) { 
    std::cerr << "Error opening " << strOutputFileName << " for output." << std::endl;
    exit(0);
  }

  uint8_t nDataSetNum;

  std::cout << "2" << std::endl;

  for (nDataSetNum = 0; nDataSetNum < nNumberOfDataSets; nDataSetNum += 1) {

    std::cout << "3" << std::endl;

    m_pcDataFit->SetDatasetPointer(m_apcDataSet[nDataSetNum]);

    // m_pcDataFit->EstimateDataBackground();
    // m_pcDataFit->RemoveBackground();

    std::cout << "4" << std::endl;

    // Search NIST data for first line after window start
    while (fNISTLambda < fLambdaSearchStart + fLambdaHalfWindowWidth) {
      fNISTLambda = m_pcNIST->m_acNISTDataByElement[strElement].m_afObservedWavelength_nm[nNISTIndex];
      nNISTIndex++;
    }

    std::cout << "5" << std::endl;

    // switch (eFittingFunction) {
    //   case TRIPLEVOIGT:
    //     // Select two more lines
    //     if (nNISTIndex < (m_pcNIST->m_acNISTDataByElement[strElement].m_nNumLines - 3)) {
    //       nNISTIndex = nNISTIndex + 2;
    //       fNISTLambdaN0 = m_pcNIST->m_acNISTDataByElement[strElement].m_afObservedWavelength_nm[nNISTIndex - 2];
    //       fNISTLambdaN1 = m_pcNIST->m_acNISTDataByElement[strElement].m_afObservedWavelength_nm[nNISTIndex - 1];
    //       fNISTLambdaN2 = m_pcNIST->m_acNISTDataByElement[strElement].m_afObservedWavelength_nm[nNISTIndex];
    //       std::cout << "Searching " << fNISTLambdaN0 << "nm, " 
    //                                   << fNISTLambdaN1 << "nm, "
    //                                   << fNISTLambdaN2 << "nm:" << std::endl;
    //     }
    //     else {
    //       eFittingFunction = VOIGT;
    //       fNISTLambda = m_pcNIST->m_acNISTDataByElement[strElement].m_afObservedWavelength_nm[nNISTIndex];
    //     }
    //     break;      
    
    //   default: 
    //     break;
    // }

    m_pcDataFit->SetFittingFunction(eFittingFunction);

    bool bLeave = false;

    do {

      std::cout << "NIST line: " << fNISTLambda << "nm " << std::endl;
      // switch (eFittingFunction) {
      //   case VOIGT:
          m_apcDataSet[nDataSetNum]->SetDomainWindow(fNISTLambda - fLambdaHalfWindowWidth, fNISTLambda + fLambdaHalfWindowWidth, 4);
      //     break;
      //   case TRIPLEVOIGT:
      //     m_pcDataFit->SetDomainWindow(fNISTLambdaN0 - fLambdaHalfWindowWidth, fNISTLambdaN2 + fLambdaHalfWindowWidth);
      //     break;
      //   default:
      //     break;
      // }

      // Initialize fitting parameters
      std::vector<double> afParam;

      afParam.clear();

      // switch (eFittingFunction) {
      //   case VOIGT:
          afParam.push_back(0.1); // Gaussian width
          afParam.push_back(0.1); // Lorentzian width (or vice versa. Sorry. Check CERN's TMAth, please and DataFit::Voigt
          afParam.push_back(fNISTLambda); // Voigt center
          afParam.push_back(100); // Voigt amplitude
      //     break;

      //   case TRIPLEVOIGT:
      //     afParam.push_back(0.1); // Gaussian width
      //     afParam.push_back(0.1); // Lorentzian width (or vice versa. Sorry. Check CERN's TMAth, please and DataFit::Voigt
      //     afParam.push_back(fNISTLambdaN0); // Voigt center
      //     afParam.push_back(100); // Voigt amplitude

      //     afParam.push_back(0.1); // Gaussian width
      //     afParam.push_back(0.1); // Lorentzian width (or vice versa. Sorry. Check CERN's TMAth, please and DataFit::Voigt
      //     afParam.push_back(fNISTLambdaN1); // Voigt center
      //     afParam.push_back(100); // Voigt amplitude

      //     afParam.push_back(0.1); // Gaussian width
      //     afParam.push_back(0.1); // Lorentzian width (or vice versa. Sorry. Check CERN's TMAth, please and DataFit::Voigt
      //     afParam.push_back(fNISTLambdaN2); // Voigt center
      //     afParam.push_back(100); // Voigt amplitude
      //     break;

      //   default:
      //     break;
      // }
       
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

      // switch (eFittingFunction) {
      //   case VOIGT:
          fSigma = afMinimizedParameters[0];
          fSigmaError = afMinimizedParameterErrors[0];

          fLg      = afMinimizedParameters[1];
          fLgError = afMinimizedParameterErrors[1];

          fCenter = afMinimizedParameters[2];
          fCenterError = afMinimizedParameterErrors[2];   

          fAmplitude = afMinimizedParameters[3]; 
          fAmplitudeError = afMinimizedParameterErrors[3];
      //     break;

      //   case TRIPLEVOIGT:
      //     // select middle voigt
      //     fSigma = afMinimizedParameters[4];
      //     fSigmaError = afMinimizedParameterErrors[4];

      //     fLg      = afMinimizedParameters[5];
      //     fLgError = afMinimizedParameterErrors[5];

      //     fCenter = afMinimizedParameters[6];
      //     fCenterError = afMinimizedParameterErrors[6];   

      //     fAmplitude = afMinimizedParameters[7]; 
      //     fAmplitudeError = afMinimizedParameterErrors[7];
      //     break;

      //   default:
      //     break;
      // }

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

      // switch (eFittingFunction) {
      //   case TRIPLEVOIGT:
      //     // Select two more lines
      //     if (nNISTIndex < (m_pcNIST->m_acNISTDataByElement[strElement].m_nNumLines - 3)) {
      //       fNISTLambdaN0 = m_pcNIST->m_acNISTDataByElement[strElement].m_afObservedWavelength_nm[nNISTIndex - 2];
      //       fNISTLambdaN1 = m_pcNIST->m_acNISTDataByElement[strElement].m_afObservedWavelength_nm[nNISTIndex - 1];
      //       fNISTLambdaN2 = m_pcNIST->m_acNISTDataByElement[strElement].m_afObservedWavelength_nm[nNISTIndex];
      //       std::cout << "Searching " << fNISTLambdaN0 << "nm, " 
      //                                 << fNISTLambdaN1 << "nm, "
      //                                 << fNISTLambdaN2 << "nm." << std::endl;
      //     }
      //     else {
        //     eFittingFunction = VOIGT;
        //     fNISTLambda = m_pcNIST->m_acNISTDataByElement[strElement].m_afObservedWavelength_nm[nNISTIndex];
        //   }
        //   break;      
        // case VOIGT:
          if (nNISTIndex < m_pcNIST->m_acNISTDataByElement[strElement].m_nNumLines) {
            fNISTLambda = m_pcNIST->m_acNISTDataByElement[strElement].m_afObservedWavelength_nm[nNISTIndex];
          }
      //     break;

      //   default: 
      //     break;
      // }
      

      // std::cin.get();

      nonblock(1);
      if (keyState(32)) {
        bLeave = true;
      }
        //32 in ASCII table correspond to Space Bar
      nonblock(0);
      
    } while ( ((fNISTLambda + fLambdaHalfWindowWidth) < fLambdaSearchEnd) ||
              (!bLeave) );
  }

  OutputFile.close();
}

