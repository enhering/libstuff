#include "DataFit.h"

DataFit::DataFit() {
  m_nMaxIndex = 0;
  m_nStartIndex = 0;

  m_nSelectedSize = 0;
  m_bWindowSelected = false;

  m_eSelectedFittingFunction = GAUSSIAN;
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

void DataFit::SetFittingFunction(FunctionNames eFunction) {
  m_eSelectedFittingFunction = eFunction;
}

void DataFit::GetNumberOfFunctionParameters() {
  switch (m_eSelectedFittingFunction) {
    
    case POWERLAW:                        m_nNumberOfFittingParameters = 3; break;
    case POWERLAW2:                       m_nNumberOfFittingParameters = 4; break;
    case POWERLAW_AND_GAUSSIAN:           m_nNumberOfFittingParameters = 6; break;
    case POWERLAW_AND_SPECIFIC_HEAT_JUMP: m_nNumberOfFittingParameters = 6; break;
    case SPECIFIC_HEAT_JUMP:              m_nNumberOfFittingParameters = 3; break;
    case TRANSITION:                      m_nNumberOfFittingParameters = 5; break;
    case GAUSSIAN:                        m_nNumberOfFittingParameters = 3; break;
    case GAP:                             m_nNumberOfFittingParameters = 4; break;
    default:                              m_nNumberOfFittingParameters = 6; break;
  }
  
  return m_nNumberOfFittingParameters;
}

void DataFit::InitializeParameters(std::vector<double> & afP) {
  int nNumberOfFittingParameters = GetNumberOfFunctionParameters;

  for (int nI = 0; nI < nNumberOfFittingParameters; nI++) {
    m_afInitialParameters.push_back(afP[nI]);
  }
}

int DataFit::F(const gsl_vector * x,
               void * data,
               gsl_vector * f) {
  
  size_t n = ((struct data *)data)->n;
  double *y = ((struct data *)data)->y;
  double *sigma = ((struct data *) data)->sigma;

  for (size_t i = 0; i < n; i++) {

    double t = ((struct data *)data)->x[i];
    double Yi = SelectedLaw(x, t);
    gsl_vector_set (f, i, (Yi - y[i])/sigma[i]);
  }
  
  return GSL_SUCCESS;
}

int DataFit::DF(const gsl_vector * x,
                void * data,
                gsl_matrix * J) {
  
  size_t n = ((struct data *)data)->n;
  double *sigma = ((struct data *) data)->sigma;

  size_t i;
  for (i = 0; i < n; i++) {
    double s = sigma[i];
    double t = ((struct data *)data)->x[i];
    
    switch (m_eSelectedFunction) {
      case POWERLAW_AND_GAUSSIAN:
        gsl_matrix_set (J, i, 0, WrapDPowerLawDA(gsl_vector_get(x,0), gsl_vector_get(x,1), gsl_vector_get(x,2),t)/s);
        gsl_matrix_set (J, i, 1, WrapDPowerLawDB(gsl_vector_get(x,0), gsl_vector_get(x,1), gsl_vector_get(x,2),t)/s);
        gsl_matrix_set (J, i, 2, WrapDPowerLawDC(gsl_vector_get(x,0), gsl_vector_get(x,1), gsl_vector_get(x,2),t)/s);

        gsl_matrix_set (J, i, 3, WrapDPowerLawDA(gsl_vector_get(x,3), gsl_vector_get(x,4), gsl_vector_get(x,5),t)/s);
        gsl_matrix_set (J, i, 4, WrapDPowerLawDB(gsl_vector_get(x,3), gsl_vector_get(x,4), gsl_vector_get(x,5),t)/s);
        gsl_matrix_set (J, i, 5, WrapDPowerLawDC(gsl_vector_get(x,3), gsl_vector_get(x,4), gsl_vector_get(x,5),t)/s);
        break;

      default:
        printf("\n *** Unknown function on DataFit::DF. Exiting.\n");
        exit(0);
        break;
    }
  }
  return GSL_SUCCESS;
}


void DataFit::Fit() {
  const gsl_multifit_nlinear_type *T = gsl_multifit_nlinear_trust;

  gsl_multifit_nlinear_workspace *w;

  gsl_multifit_nlinear_fdf fdf;

  gsl_multifit_nlinear_parameters fdf_params = gsl_multifit_nlinear_default_parameters();

  // Find number of data points to use
  
  size_t n;
  
  if (m_bWindowSelected) {
    n = m_afSelectedX.size();
  }
  else {
    n = m_afX.size();
  }
  // Default fitting function is GAUSSIAN. Set with SetFittingFunction above

  // Find number of parameters of fitting function
  const size_t p = GetNumberOfFunctionParameters();

  gsl_vector *f;
  gsl_matrix *J;
  gsl_matrix *covar = gsl_matrix_alloc (p, p);
  double y[n], weights[n];
  struct data d = { n, y, sigma };

  // Parameter initialization;
  double x_init[p]; 
  for(int nI = 0; nI < p; nI++) {
    x_init[nI] = m_afInitialParameters[nI];
  }

  // Define the function to be minimized
  fdf.f = expb_f;
  fdf.df = expb_df;   /* set to NULL for finite-difference Jacobian */
  fdf.fvv = NULL;     /* not using geodesic acceleration */
  fdf.n = n;
  fdf.p = p;
  fdf.params = &d;



}

// DataFit::

