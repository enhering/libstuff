#include "DataSet.h"

DataSet::DataSet() {

}

DataSet::~DataSet() {

}

void DataSet::AddDataPoint(double fX, double fY, double fYSD) {
  m_afX.push_back(fX);
  m_afY.push_back(fY);
  m_afYSD.push_back(fYSD);

  m_nMaxIndex++;

  m_nStartIndex = 0;
  m_nEndIndex = m_nMaxIndex; // ATTENTION HERE! ADDING A NEW DATAPOINT RESETS SEARCH WINDOW!
}

// REQUIREMENT: DATA MUST BE SORTED!
void DataSet::SetDomainWindow(double fXStart, double fXEnd, uint8_t nMinNumDatapoints) {
  long nIndex = 0;

  while (nIndex < m_nMaxIndex) {
    if (m_afX[nIndex] > fXStart) {
      break;
    }
    nIndex++;
  }
  m_nStartIndex = nIndex;

  while (nIndex < m_nMaxIndex) {
    if (m_afX[nIndex] > fXEnd) {
      break;
    }
    nIndex++;
  }
  m_nEndIndex = nIndex;

  if ((m_nEndIndex - m_nStartIndex) < nMinNumDatapoints ) {
    m_nEndIndex = m_nStartIndex + nMinNumDatapoints;
  }

  std::cout << "DW[" << fXStart << "nm:" << fXEnd << "nm] IW["
            << m_nStartIndex << ":" << m_nEndIndex << "] ";

  m_bWindowSelected = true;
  m_nSelectedSize = 0;

  m_afSelectedX.clear();
  m_afSelectedY.clear();
  m_afSelectedYSD.clear();

  for (long nIndex = m_nStartIndex; nIndex <= m_nEndIndex; nIndex++) {
    m_afSelectedX.push_back(m_afX[nIndex]);
    m_afSelectedY.push_back(m_afY[nIndex]);
    m_afSelectedYSD.push_back(m_afYSD[nIndex]);

    m_nSelectedSize++;

    // std::cout << "Selected lambda: " << m_afSelectedX[m_nSelectedSize - 1] <<
    //              "nm, intensity: "   << m_afSelectedY[m_nSelectedSize - 1] <<
    //              " counts and std dev: "     << m_afSelectedYSD[m_nSelectedSize - 1] << std::endl;
  }
  //std::cout << m_nSelectedSize << " datapoints selected." << std::endl << std::endl;
}

void DataSet::ClearDomainWindow() {
  m_bWindowSelected = false;
  m_nSelectedSize = 0;

  m_afSelectedX.clear();
  m_afSelectedY.clear();
  m_afSelectedYSD.clear();
}