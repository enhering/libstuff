#ifndef SPECTRUM_H_
  #define SPECTRUM_H_

  #include "Base.h"
  #include "NIST.h"
  #include "DataFit.h"
  #include "DataSet.h"

  #include <vector>

  class Spectrum : public Base {

    public: 
      Spectrum();
      ~Spectrum();

      void SetNISTObjectPointer(NIST * pcNIST) { m_pcNIST = pcNIST; }
      void SetDatasetArrayRef(std::vector<DataSet *>  apcDataSet) { m_apcDataSet = apcDataSet; }
      void AnalizeFitResult(long);
      void ScanData(std::string);

    private:
      NIST * m_pcNIST;
      DataFit * m_pcDataFit;

      std::string m_strSearchElement;
      std::vector<DataSet *> m_apcDataSet;

      std::ofstream m_OutputFile;
  };
#endif