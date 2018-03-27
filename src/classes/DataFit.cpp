#include "DataFit.h"

DataFit::DataFit() {
  m_nMaxIndex = 0;
  m_nStartIndex = 0;

  m_nSelectedSize = 0;
  m_bWindowSelected = false;
}

DataFit::~DataFit() {

}

void DataFit::AddDataPoint(double fX, double fY, double fYSD) {
  m_afX.push_back(fX);
  m_afY.push_back(fY);
  m_afYSD.push_back(fYSD);

  m_nMaxIndex++;

  m_nStartIndex = 0;
  m_nEndIndex = m_nMaxIndex; // ATTENTION HERE! ADDING A NEW DATAPOINT RESETS SEARCH WINDOW!
}

// REQUIREMENT: DATA MUST BE SORTED!
void DataFit::SetSearchWindow(double fXStart, double fXEnd) {
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
  }
}

void DataFit::ClearSearchWindow() {
  m_bWindowSelected = false;
  m_nSelectedSize = 0;

  m_afSelectedX.clear();
  m_afSelectedY.clear();
  m_afSelectedYSD.clear();
}

void Fit() {
  const gsl_multifit_nlinear_type *T = gsl_multifit_nlinear_trust;

  gsl_multifit_nlinear_workspace *w;

  gsl_multifit_nlinear_fdf fdf;

  gsl_multifit_nlinear_parameters fdf_params = gsl_multifit_nlinear_default_parameters();

  // Find number of data points to use
  const size_t n;
  if (m_bWindowSelected) {
    n = m_afSelectedX.size();
  }
  else {
    n = m_afX.size();
  }

  // Find number of parameters of fitting function
  const size_t p = 3;




}

