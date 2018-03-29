#ifndef BASE_H__
  #define BASE_H__

  #include <stdio.h>
  #include <iostream>
  #include <fstream>
  #include <vector>

  #include "TCanvas.h"
  #include "TROOT.h"
  #include "TGraph.h"
  #include "TMultiGraph.h"
  #include "TF1.h"
  #include "TLegend.h"
  #include "TArrow.h"
  #include "TLatex.h"
  #include "TSystem.h"
  #include "TApplication.h"
  #include "TAxis.h"

  #include "BuildNumber.h"

  class Base {
    public: 
      Base();
      ~Base();
      void ltrim(std::string &);
      void rtrim(std::string &);

    private:

  };
#endif