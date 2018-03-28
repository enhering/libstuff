#ifndef LIBS_H__
  #define LIBS_H__

  // #include "Riostream.h"
  // #include "TCanvas.h"
  // #include "TH1.h"
  // #include "TF1.h"
  // #include "TSpectrum.h"
  // #include "TVirtualFitter.h"

  #include <vector>

  #include <gsl/gsl_matrix.h>
  #include <gsl/gsl_vector.h>
  #include <gsl/gsl_blas.h>
  #include <gsl/gsl_multifit_nlinear.h>

  #include "Base.h"
  #include "DataFit.h"

  class LIBS : public Base {
    public:
      LIBS();
      ~LIBS();
      
      void LoadData(std::string);
      void SetSearchWindow(float, float);
      void ScanData();
    private:
      DataFit * m_pcDataFit;

  };
#endif