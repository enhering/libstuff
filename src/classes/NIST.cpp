#include "NIST.h"

NIST::NIST() {

}

NIST::~NIST() {

}

void NIST::RetrieveData() {
 
  curl_global_init(CURL_GLOBAL_DEFAULT);
 
  m_pcCurl = curl_easy_init();

  if(m_pcCurl) {
    FILE *out;
    char filename[128];
    snprintf(filename, 128, "NIST-DATA");
 
    out = fopen(filename, "wb");
 
  /* write to this file */ 
    curl_easy_setopt(m_pcCurl, CURLOPT_WRITEDATA, out);
    curl_easy_setopt(m_pcCurl, CURLOPT_URL, "https://physics.nist.gov/cgi-bin/ASD/lines1.pl?spectra=W+I%3B+W+II&limits_type=0&low_w=&upp_w=&unit=1&submit=Retrieve+Data&de=0&format=1&line_out=0&remove_js=on&en_unit=0&output=0&bibrefs=1&page_size=15&show_obs_wl=1&show_calc_wl=1&unc_out=1&order_out=0&max_low_enrg=&show_av=2&max_upp_enrg=&tsb_value=0&min_str=&A_out=0&intens_out=on&max_str=&allowed_out=1&forbid_out=1&min_accur=&min_intens=&conf_out=on&term_out=on&enrg_out=on&J_out=on");
    curl_easy_setopt(m_pcCurl, CURLOPT_SSL_VERIFYPEER, 0L);
    curl_easy_setopt(m_pcCurl, CURLOPT_SSL_VERIFYHOST, 0L);

    /* Perform the request, m_cRes will get the return code */ 
    m_cRes = curl_easy_perform(m_pcCurl);
    /* Check for errors */ 
    if(m_cRes != CURLE_OK)
      fprintf(stderr, "curl_easy_perform() failed: %s\n",
              curl_easy_strerror(m_cRes));
 
    /* always cleanup */ 
    curl_easy_cleanup(m_pcCurl);
  }
 
  curl_global_cleanup();
}

