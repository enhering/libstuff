#ifndef LIBS_H__
  #define LIBS_H__

  #include "Riostream.h"
  #include "TCanvas.h"
  #include "TH1.h"
  #include "TSpectrum.h"
  #include "TVirtualFitter.h"

  #include "Base.h"

  class LIBS : public Base {
    public:
      LIBS();
      ~LIBS();
      
      void LoadData(std::string);

    private:

  };
#endif