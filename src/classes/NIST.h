#ifndef NIST_H__
  #define NIST_H__

  #include <curl/curl.h>
  #include <stdlib.h>
  #include <string>
  #include <vector>
  #include <unordered_map>

  #include "Base.h"
  #include "NISTData.h"
  
  class NIST : public Base {

    public:
      NIST();
      ~NIST();
      void RetrieveNISTData();
      void ConvertAndLoadNISTData();

      std::unordered_map<std::string, NISTData> m_acNISTDataByElement;
      
    private:
      CURL     * m_pcCurl;
      CURLcode   m_cRes;

  };
#endif