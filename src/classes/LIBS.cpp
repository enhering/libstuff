#include "LIBS.h"

LIBS::LIBS() {

}

LIBS::~LIBS() {

}

void LIBS::LoadData(std::string strFileName) {
  
  std::string   strLine;
  std::ifstream InputFile.open(strFileName);

  m_nMaxIndex = 0;

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
    m_afLIBSData_Lambda.push_back(fLambda);
    m_afLIBSData_Intensity.push_back(fIntensity);
    m_nMaxIndex++;
  }
}

void LIBS::SetSearchWindow(float fLambdaStart, float fLambdaEnd) {
  long nIndex = 0;
  while (nIndex < m_nMaxIndex) {
    if (m_afLIBSData_Lambda[nIndex] > fLambdaStart) {
      break;
    }
    nIndex++;
  }
  m_nStartIndex = nIndex;

  while (nIndex < m_nMaxIndex) {
    if (m_afLIBSData_Lambda[nIndex] > fLambdaEnd) {
      break;
    }
    nIndex++;
  }
  m_nEndIndex = nIndex;
}

