#include <curl/curl.h>
#include <stdlib.h>
#include <string.h>

#include "Base.h"

class NIST {

  public:
    NIST();
    ~NIST();
    void RetrieveNISTData();
    void ConvertNISTData();
    void ltrim(std::string &);
    void rtrim(std::string &);
    
  private:
    CURL     * m_pcCurl;
    CURLcode   m_cRes;
};
