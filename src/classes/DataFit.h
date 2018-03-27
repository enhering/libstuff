#ifndef DATAFIT_H__
  #define DATAFIT_H__

  #include <vector>

  #include <gsl/gsl_matrix.h>
  #include <gsl/gsl_vector.h>
  #include <gsl/gsl_blas.h>
  #include <gsl/gsl_multifit_nlinear.h>

  #include "Base.h"
  
  enum FunctionNames {
    POWERLAW_AND_GAUSSIAN,
    POWERLAW,
    POWERLAW2,
    POWERLAW_AND_SPECIFIC_HEAT_JUMP,
    SPECIFIC_HEAT_JUMP,
    TRANSITION,
    GAUSSIAN,
    GAP
  };

  class DataFit : public Base {
    public:
      DataFit();
      ~DataFit();

      void AddDataPoint(double, double, double);
      void SetSearchWindow(double, double);
      void ClearSearchWindow();
      void SetFittingFunction(FunctionNames);
      void GetNumberOfFunctionParameters();
      void Fit();

    private:
      std::vector<double> m_afX;
      std::vector<double> m_afY;
      std::vector<double> m_afYSD;

      std::vector<double> m_afSelectedX;
      std::vector<double> m_afSelectedY;
      std::vector<double> m_afSelectedYSD;      

      std::vector<double> m_afInitialParameters;
      std::vector<double> m_afParameters;

      double m_fStdDev;

      long m_nStartIndex, m_nEndIndex, m_nMaxIndex;
      bool m_bWindowSelected;
      long m_nSelectedSize;

      FunctionNames m_eSelectedFittingFunction;
      uint8_t       m_nNumberOfFittingParameters;
  };

#endif