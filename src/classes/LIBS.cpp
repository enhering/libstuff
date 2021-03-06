#include "LIBS.h"

LIBS::LIBS() {

}

LIBS::~LIBS() {

}

void LIBS::LoadData(std::string strFileName, DataSet * pcDataSet) {
  
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

    pcDataSet->AddDataPoint(fLambda, fIntensity, 1.0);
    //m_pcDataFit->AddDataPoint(fLambda, fIntensity, 1.0);
  }
}

