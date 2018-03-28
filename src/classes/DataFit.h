#ifndef DATAFIT_H__
  #define DATAFIT_H__

  #include <vector>
  #include <math.h>

  #include <gsl/gsl_matrix.h>
  #include <gsl/gsl_vector.h>
  #include <gsl/gsl_blas.h>
  #include <gsl/gsl_multifit_nlinear.h>

  #include "Base.h"
  
  enum FunctionNames {
    GAUSSIAN
  };

  struct data {
    size_t n;
    double * x;
    double * y;
    double * sigma;
  };

  class DataFit : public Base {
    public:
      DataFit();
      ~DataFit();

      void AddDataPoint(double, double, double);
      void SetSearchWindow(double, double);
      void ClearSearchWindow();
      void SetFittingFunction(FunctionNames);
      int  GetNumberOfFunctionParameters();
      void InitializeParameters(std::vector<double> &);

      static double SelectedLaw(const gsl_vector *, double);
      static int F (const gsl_vector *, void *, gsl_vector *);
      static int DF(const gsl_vector *, void *, gsl_matrix *);
      static void Callback(const size_t, void *, const gsl_multifit_nlinear_workspace *);
      void Fit();
      static double Gaussian(const gsl_vector *, double);
      static double DGaussianDA(double, double, double, double);
      static double DGaussianDB(double, double, double, double);
      static double DGaussianDC(double, double, double, double);

    private:
      std::vector<double> m_afX;
      std::vector<double> m_afY;
      std::vector<double> m_afYSD;

      std::vector<double> m_afSelectedX;
      std::vector<double> m_afSelectedY;
      std::vector<double> m_afSelectedYSD;      

      std::vector<double> m_afInitialParameters;
      std::vector<double> m_afParameters;
      bool                m_bParametersIntialized;

      double m_fStdDev;

      long m_nStartIndex, m_nEndIndex, m_nMaxIndex;
      bool m_bWindowSelected;
      long m_nSelectedSize;

      static FunctionNames m_eSelectedFittingFunction;
      uint8_t              m_nNumberOfFittingParameters;
  };

#endif