#ifndef NIST_H__
  #define NIST_H__

  #include <curl/curl.h>
  #include <stdlib.h>
  #include <string.h>

  #include "Base.h"

  class NIST : public Base {

    public:
      NIST();
      ~NIST();
      void RetrieveNISTData();
      void ConvertNISTData();
      
    private:
      CURL     * m_pcCurl;
      CURLcode   m_cRes;
  };
#endif