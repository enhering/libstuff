#include <curl/curl.h>
#include <stdlib.h>
#include <string.h>

#include "Base.h"

class NIST {

  public:
    NIST();
    ~NIST();
    void RetrieveData();

  private:
    CURL     * m_pcCurl;
    CURLcode   m_cRes;
};
