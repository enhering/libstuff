#ifndef NIST_H__
  #define NIST_H__

  #include <curl/curl.h>
  #include <stdlib.h>
  #include <string>
  #include <vector>
  #include <unordered_map>

  #include "Base.h"

  struct NISTData {
    long nNumLines;

    std::vector<float> fObservedWavelength_nm;
    std::vector<float> fRitzWavelength_nm;
    std::vector<float> fUncertainty_nm;
    std::vector<std::string> strRelInt;
    std::vector<float> fAki_sec_minus_one;
    std::vector<std::string> strAcc;
    std::vector<float> fEi_eV;
    std::vector<float> fEf_eV;
    std::vector<std::string> strLowerLevelConf;
    std::vector<std::string> strLowerLevelTerm;
    std::vector<std::string> strLowerLevelJ;
    std::vector<std::string> strUpperLevelConf;
    std::vector<std::string> strUpperLevelTerm;
    std::vector<std::string> strUpperLevelJ;
  };

  class NIST : public Base {

    public:
      NIST();
      ~NIST();
      void RetrieveNISTData();
      void ConvertAndLoadNISTData();

      std::unordered_map<std::string, NISTData *> m_asNISTDataByElement;
      
    private:
      CURL     * m_pcCurl;
      CURLcode   m_cRes;

  };
#endif