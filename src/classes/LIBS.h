#ifndef LIBS_H__
  #define LIBS_H__

  #include <vector>

  // #include <gsl/gsl_matrix.h>
  // #include <gsl/gsl_vector.h>
  // #include <gsl/gsl_blas.h>
  // #include <gsl/gsl_multifit_nlinear.h>

  #include "Base.h"
  #include "DataSet.h"

  class LIBS : public Base {
    public:
      LIBS();
      ~LIBS();
      
      void LoadData(std::string, DataSet *);
      // void SetSearchWindow(float, float);
      // void ScanData(std::string);

    private:
      // DataFit * m_pcDataFit;
      // NIST    * m_pcNIST;

  };
#endif