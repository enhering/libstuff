#ifndef __FUNCTIONFIT_H_
#define __FUNCTIONFIT_H_

  #include "Fit.h"
  #include "FitFunctions.h"

  // #include <mgl2/mgl.h>

  #define PARAMETER_VECTOR_SIZE 100

  enum FunctionList {
    POWERLAW_AND_GAUSSIAN,
    POWERLAW,
    POWERLAW2,
    POWERLAW_AND_SPECIFIC_HEAT_JUMP,
    SPECIFIC_HEAT_JUMP,
    TRANSITION,
    GAUSSIAN,
    GAP
  };

  class FunctionFit : public Fit {

    public:
    
      FunctionFit();
    
      int  NumFunctionParameters(FunctionList);
      void SelectFunction(FunctionList);
    
      void GetParameters(double *);
      void SetParameters(double *, int);
    
      int GetNumParameters() { return m_nNumParameters; };
    
      void FitIt(mglData *, mglData *,
                 mglData *, double *);
    
      static double SelectedLaw(const gsl_vector * , double);
    
      static double WrapGaussian(const gsl_vector * , double, int);
      static double WrapPowerLaw(const gsl_vector * , double, int);
      static double WrapPowerLaw2(const gsl_vector * , double, int);

      static double WrapSpecificHeatJump(const gsl_vector * , double, int);

      static double WrapTransition(const gsl_vector * , double, int);
      static double WrapGap(const gsl_vector * , double, int); 

      int m_nNumParameters;
      double m_padParameters[PARAMETER_VECTOR_SIZE];

      static FunctionList m_eSelectedFunction;
    
      static double WrapDPowerLawDA(double dA, double dB, double dC, double dX);
      static double WrapDPowerLawDB(double dA, double dB, double dC, double dX);
      static double WrapDPowerLawDC(double dA, double dB, double dC, double dX);
    
      static double WrapDGaussianDA(double dA, double dB, double dC, double dX);
      static double WrapDGaussianDB(double dA, double dB, double dC, double dX);
      static double WrapDGaussianDC(double dA, double dB, double dC, double dX);
    
    protected:
    
      virtual void InitGSL(gsl_multifit_function_fdf& f);

      static int F(const gsl_vector *,
                   void *,
                   gsl_vector *);
    
      static int DF(const gsl_vector *,
                    void *,
                    gsl_matrix *);
    
      static int FDF(const gsl_vector *,
                     void *,
                     gsl_vector *,
                     gsl_matrix *);
  };

#endif