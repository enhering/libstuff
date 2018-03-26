#ifndef LIBS_H__
  #define LIBS_H__

  // #include "Riostream.h"
  // #include "TCanvas.h"
  // #include "TH1.h"
  // #include "TF1.h"
  // #include "TSpectrum.h"
  // #include "TVirtualFitter.h"

  #include <vector>
  #include "Base.h"

  class LIBS : public Base {
    public:
      LIBS();
      ~LIBS();
      
      void LoadData(std::string);

    private:
      std::vector<float> m_afLibsData_Lambda;
      std::vector<float> m_afLibsData_Intensity;

      long m_nStartIndex, m_nEndIndex, m_nMaxIndex;
  };
#endif