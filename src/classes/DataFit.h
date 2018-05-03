#ifndef DATAFIT_H__
  #define DATAFIT_H__

  #include <vector>
  #include <math.h>

  #include <gsl/gsl_matrix.h>
  #include <gsl/gsl_vector.h>
  #include <gsl/gsl_blas.h>
  #include <gsl/gsl_multifit_nlinear.h>

  #include "TMath.h"

  #include "Base.h"
  #include "Dataset.h"
  
  enum FunctionNames {
    GAUSSIAN,
    VOIGT, 
    LINEAR, 
    TRIPLEVOIGT
  };

  struct data {
    size_t n;
    double * x;
    double * y;
    double * weights;
  };

  class DataFit : public Base {
    public:
      DataFit();
      ~DataFit();

      // void AddDataPoint(double, double, double);
      // double GetY(long nIndex)   { return m_afY[nIndex]; }
      // double GetX(long nIndex)   { return m_afX[nIndex]; }
      // double GetYSD(long nIndex) { return m_afYSD[nIndex]; }

      // void SetSearchWindow(double, double);
      // void ClearSearchWindow();

      void SetDatasetPointer(DataSet * pcDataSet) { m_pcDataSet = pcDataSet; }

      void SetFittingFunction(FunctionNames);
      int  GetNumberOfFunctionParameters();
      void InitializeParameters(std::vector<double> &);

      static double SelectedLaw(const gsl_vector *, double);
      static int F (const gsl_vector *, void *, gsl_vector *);
      static int DF(const gsl_vector *, void *, gsl_matrix *);
      static void Callback(const size_t, void *, const gsl_multifit_nlinear_workspace *);
      void Fit();
      void EstimateDataBackground();
      void RemoveBackground()      { m_bRemoveBakground = true;  };
      void DoNotRemoveBackground() { m_bRemoveBakground = false; };

      double GetChiSqr()            { return m_fChiSqr; };
      void   GetMinimizedParameters(std::vector<double> &);
      void   GetMinimizedParameterErrors(std::vector<double> &);
      int    GetFitStatus()         { return m_nFitStatus; }; 

      static double Gaussian(const gsl_vector *, double);
      static double DGaussianDA(double, double, double, double);
      static double DGaussianDB(double, double, double, double);
      static double DGaussianDC(double, double, double, double);

      static double TripleVoigt(const gsl_vector * , double);
      static double Voigt(const gsl_vector * , double);
      static double Linear(const gsl_vector * , double);

    private:
      // std::vector<double> m_afX;
      // std::vector<double> m_afY;
      // std::vector<double> m_afYSD;

      // std::vector<double> m_afSelectedX;
      // std::vector<double> m_afSelectedY;
      // std::vector<double> m_afSelectedYSD;   

      DataSet * m_pcDataSet;   

      std::vector<double> m_afInitialParameters;
      bool                m_bParametersIntialized;

      // long m_nStartIndex, m_nEndIndex, m_nMaxIndex;
      // bool m_bWindowSelected;
      // long m_nSelectedSize;

      static FunctionNames m_eSelectedFittingFunction;
      uint8_t              m_nNumberOfFittingParameters;

      std::vector<double> m_afParameters;
      std::vector<double> m_afParameterErrors;

      double m_fChiSqr;
      int    m_nFitStatus;

      bool   m_bBackgroundEstimated;
      double m_fLinearBackgroundA, m_fLinearBackgroundB; // f=a+bx
      bool   m_bRemoveBakground;
  };

#endif