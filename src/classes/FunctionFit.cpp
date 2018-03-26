#include "FunctionFit.h"

FunctionList FunctionFit::m_eSelectedFunction = POWERLAW_AND_GAUSSIAN;

FunctionFit::FunctionFit() {
  for (int nIndex = 0; nIndex < PARAMETER_VECTOR_SIZE; nIndex++) {
    m_padParameters[nIndex] = 1.0;
  }
  m_nNumParameters = 0;
}

void FunctionFit::GetParameters(double * padIndependents) {
  
  for (long nIndex = 0; nIndex < m_nNumParameters; nIndex++)
    padIndependents[nIndex] = m_padParameters[nIndex];
}

void FunctionFit::SetParameters(double * padIndependents, int nNumParameters) {
  
  for (long nIndex = 0; nIndex < nNumParameters; nIndex++)
    m_padParameters[nIndex] = padIndependents[nIndex];
}

void FunctionFit::InitGSL(gsl_multifit_function_fdf& f) {
  f.f = &F;
  f.df = NULL; //&DF;
  f.fdf = NULL; //&FDF;
}

void FunctionFit::FitIt(mglData * cXData, mglData * cYData,
                        mglData * cYStdDevData,
                        double * pdChiSqr) {
  
  FitData(cXData,cYData,
          cYStdDevData,
          pdChiSqr,
          m_padParameters,
          m_nNumParameters);
}

int FunctionFit::NumFunctionParameters(FunctionList eFunction) {

  switch (eFunction) {
    
    case POWERLAW: 
      m_nNumParameters = 3;
      break;

    case POWERLAW2: 
      m_nNumParameters = 4;
      break;

    case POWERLAW_AND_GAUSSIAN:
      m_nNumParameters = 6;
      break;
      
    case POWERLAW_AND_SPECIFIC_HEAT_JUMP:
      m_nNumParameters = 6;
      break;

    case SPECIFIC_HEAT_JUMP:
      m_nNumParameters = 3;
      break;

    case TRANSITION:
      m_nNumParameters = 5;
      break;

    case GAUSSIAN:
      m_nNumParameters = 3;
      break;

    case GAP:
      m_nNumParameters = 4;
      break;

    default:
      m_nNumParameters = 6;
      break;
  }
  
  return m_nNumParameters;
}

void FunctionFit::SelectFunction(FunctionList eFunction) {

  m_eSelectedFunction = eFunction;
  NumFunctionParameters(eFunction);
}

double FunctionFit::SelectedLaw(const gsl_vector * padIndependents, double dX) {

  double dValue;

  //printf("Evaluating law ->%d<-\n", m_eSelectedFunction);

  switch (m_eSelectedFunction) {
    
    case GAUSSIAN:
      dValue = WrapGaussian(padIndependents, dX, 0);
      return(dValue);
      break;

    case TRANSITION:
      dValue = WrapTransition(padIndependents, dX, 0);
      return(dValue);
      break;

    case POWERLAW:    
      dValue = WrapPowerLaw(padIndependents, dX, 0);
      return(dValue);
      break;

    case POWERLAW2:    
      dValue = WrapPowerLaw2(padIndependents, dX, 0);
      return(dValue);
      break;

    case POWERLAW_AND_GAUSSIAN:
      dValue = WrapGaussian(padIndependents, dX, 0)
             + WrapPowerLaw(padIndependents, dX, 3);
      return(dValue);
      break;
  
    case POWERLAW_AND_SPECIFIC_HEAT_JUMP:
      dValue = WrapSpecificHeatJump(padIndependents, dX, 0)
             + WrapPowerLaw(padIndependents, dX, 3);
      return (dValue);
      break;

    case SPECIFIC_HEAT_JUMP:
      dValue = WrapSpecificHeatJump(padIndependents, dX, 0);
      return (dValue);
      break;

    case GAP:
      dValue = WrapGap(padIndependents, dX, 0);
      return (dValue);
      break;
      
    default:
      break;
  }
  return (0);
}

int FunctionFit::F(const gsl_vector * x,
                   void * data,
                   gsl_vector * f) {
  
  //printf("Entering F. Parameter pointer address is %p\n", x);

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

int FunctionFit::DF(const gsl_vector * x,
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

        gsl_matrix_set (J, i, 0, WrapDPowerLawDA(gsl_vector_get(x,0),
                                                 gsl_vector_get(x,1),
                                                 gsl_vector_get(x,2),
                                                 t)/s);
        gsl_matrix_set (J, i, 1, WrapDPowerLawDB(gsl_vector_get(x,0),
                                                 gsl_vector_get(x,1),
                                                 gsl_vector_get(x,2),
                                                 t)/s);
        gsl_matrix_set (J, i, 2, WrapDPowerLawDC(gsl_vector_get(x,0),
                                                 gsl_vector_get(x,1),
                                                 gsl_vector_get(x,2),
                                                 t)/s);

        gsl_matrix_set (J, i, 3, WrapDPowerLawDA(gsl_vector_get(x,3),
                                                 gsl_vector_get(x,4),
                                                 gsl_vector_get(x,5),
                                                 t)/s);
        gsl_matrix_set (J, i, 4, WrapDPowerLawDB(gsl_vector_get(x,3),
                                                 gsl_vector_get(x,4),
                                                 gsl_vector_get(x,5),
                                                 t)/s);
        gsl_matrix_set (J, i, 5, WrapDPowerLawDC(gsl_vector_get(x,3),
                                                 gsl_vector_get(x,4),
                                                 gsl_vector_get(x,5),
                                                 t)/s);
        break;

      default:
        printf("\n *** Unknown function on FunctionFit::DF. Exiting.\n");
        exit(0);
        break;
    }
    
  }

  return GSL_SUCCESS;
}

int FunctionFit::FDF (const gsl_vector * x,
                         void *data,
                         gsl_vector * f,
                         gsl_matrix * J) {

  F(x, data, f);
  
  DF(x, data, J);
  
  return GSL_SUCCESS;
}

double FunctionFit::WrapGaussian(const gsl_vector * padIndependents, double dX, int nOffset) {
  return (Gaussian(padIndependents, dX, nOffset));
}

double FunctionFit::WrapPowerLaw(const gsl_vector * padIndependents, double dX, int nOffset) {
  return(PowerLaw(padIndependents, dX, nOffset));
}

double FunctionFit::WrapPowerLaw2(const gsl_vector * padIndependents, double dX, int nOffset) {
  return(PowerLaw2(padIndependents, dX, nOffset));
}

double FunctionFit::WrapSpecificHeatJump(const gsl_vector * padIndependents, double dX, int nOffset) {
  return(SpecificHeatJump(padIndependents, dX, nOffset));
}

double FunctionFit::WrapTransition(const gsl_vector * padIndependents, double dX, int nOffset) {
  return(Transition(padIndependents, dX, nOffset));
}

double FunctionFit::WrapGap(const gsl_vector * padIndependents, double dX, int nOffset)  {
  return(Gap(padIndependents, dX, nOffset));
}

// IN USE, THOSE BELOW ARE NOT

double FunctionFit::WrapDGaussianDA(double dA, double dB, double dC, double dX) {
  return(DGaussianDA(dA, dB, dC, dX));
}

double FunctionFit::WrapDGaussianDB(double dA, double dB, double dC, double dX) {
  return(DGaussianDB(dA, dB, dC, dX));
}

double FunctionFit::WrapDGaussianDC(double dA, double dB, double dC, double dX) {
  return(DGaussianDC(dA, dB, dC, dX));
}






double FunctionFit::WrapDPowerLawDA(double dA, double dB, double dC, double dX) {
  return(DPowerLawDA(dA, dB, dC, dX));
}

double FunctionFit::WrapDPowerLawDB(double dA, double dB, double dC, double dX) {
  return(DPowerLawDB(dA, dB, dC, dX));
}

double FunctionFit::WrapDPowerLawDC(double dA, double dB, double dC, double dX) {
  return(DPowerLawDC(dA, dB, dC, dX));
}



