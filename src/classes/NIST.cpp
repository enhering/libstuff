#include "NIST.h"

NIST::NIST() {

}

NIST::~NIST() {

}

void NIST::RetrieveNISTData() {
 
  curl_global_init(CURL_GLOBAL_DEFAULT);
 
  m_pcCurl = curl_easy_init();

  if(m_pcCurl) {
    FILE *out;
    char filename[128];
    snprintf(filename, 128, "NIST-ORIGINAL-WEB-FILE");
 
    std::cout << "Downloading NIST data..." << std::endl;

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

void NIST::ConvertNISTData() {
 
  std::ifstream    NISTDataFile;
  std::ofstream    ConvertedDataFile;
  std::string      strLine;
  std::string      strStartPattern = "<pre>";
  std::string      strEndPattern = "</pre>";

  std::cout << "Converting NIST data..." << std::endl;

  NISTDataFile.open("NIST-ORIGINAL-WEB-FILE");
  ConvertedDataFile.open("NIST-CONVERTED-DATA");

  if (! NISTDataFile.is_open()) { 
    std::cerr << "Error opening NIST-ORIGINAL-WEB-FILE for output." << std::endl;
    exit(0);
  }
  if (! ConvertedDataFile.is_open()) { 
    std::cerr << "Error opening NIST-ORIGINAL-WEB-FILE fr input." << std::endl;
    exit(0);
  }

  bool    bFoundStart = false;
  uint8_t nSkipCounter = 0;
  uint8_t nSkip = 7;

  bool    bIsDataLine = false;

  while (getline (NISTDataFile, strLine)) {
    std::size_t nFound = strLine.find(strStartPattern);
    if (nFound != std::string::npos) {
      bFoundStart = true;
    }

    if (bFoundStart) {
      nSkipCounter++;
      if (nSkipCounter > nSkip) {
        bIsDataLine = true;
        bFoundStart = false;
      }
    }

    nFound = strLine.find(strEndPattern);
    if (nFound != std::string::npos) {
      bIsDataLine = false;
    }
 
    if (bIsDataLine) {
      if ((strLine.front() != ' ') &&
          (strLine.front() != '-') ) { 

        std::size_t nPos1, nPos2; 
        std::string strCol01, strCol02, strCol03, strCol04, strCol05, strCol06, strCol07, 
                    strCol08, strCol09, strCol10, strCol11, strCol12, strCol13, strCol14,
                    strCol15, strCol16, strCol17;
        std::string strSeparator = "|";

        nPos1 = 0;
        nPos2 = strLine.find(strSeparator, 0);
        strCol01 = strLine.substr(0, nPos2 - nPos1);
        ltrim(strCol01); rtrim(strCol01);

        nPos1 = nPos2;
        nPos2 = strLine.find(strSeparator, nPos1 + 1);
        strCol02 = strLine.substr(nPos1 + 1, nPos2 - nPos1 - 1);
        ltrim(strCol02); rtrim(strCol02);

        nPos1 = nPos2;
        nPos2 = strLine.find(strSeparator, nPos1 + 1);
        strCol03 = strLine.substr(nPos1 + 1, nPos2 - nPos1 - 1);
        ltrim(strCol03); rtrim(strCol03);

        nPos1 = nPos2;
        nPos2 = strLine.find(strSeparator, nPos1 + 1);
        strCol04 = strLine.substr(nPos1 + 1, nPos2 - nPos1 - 1);
        ltrim(strCol04); rtrim(strCol04);

        nPos1 = nPos2;
        nPos2 = strLine.find(strSeparator, nPos1 + 1);
        strCol05 = strLine.substr(nPos1 + 1, nPos2 - nPos1 - 1);
        ltrim(strCol05); rtrim(strCol05);

        nPos1 = nPos2;
        nPos2 = strLine.find(strSeparator, nPos1 + 1);
        strCol06 = strLine.substr(nPos1 + 1, nPos2 - nPos1 - 1);
        ltrim(strCol06); rtrim(strCol06);

        nPos1 = nPos2;
        nPos2 = strLine.find(strSeparator, nPos1 + 1);
        strCol07 = strLine.substr(nPos1 + 1, nPos2 - nPos1 - 1);
        ltrim(strCol07); rtrim(strCol07);

        nPos1 = nPos2;
        nPos2 = strLine.find(strSeparator, nPos1 + 1);
        strCol08 = strLine.substr(nPos1 + 1, nPos2 - nPos1 - 1);
        ltrim(strCol08); rtrim(strCol08);

        nPos1 = nPos2;
        nPos2 = strLine.find(strSeparator, nPos1 + 1);
        strCol09 = strLine.substr(nPos1 + 1, nPos2 - nPos1 - 1);
        ltrim(strCol09); rtrim(strCol09);

        nPos1 = nPos2;
        nPos2 = strLine.find(strSeparator, nPos1 + 1);
        strCol10 = strLine.substr(nPos1 + 1, nPos2 - nPos1 - 1);
        ltrim(strCol10); rtrim(strCol10);

        nPos1 = nPos2;
        nPos2 = strLine.find(strSeparator, nPos1 + 1);
        strCol11 = strLine.substr(nPos1 + 1, nPos2 - nPos1 - 1);
        ltrim(strCol11); rtrim(strCol11);

        nPos1 = nPos2;
        nPos2 = strLine.find(strSeparator, nPos1 + 1);
        strCol12 = strLine.substr(nPos1 + 1, nPos2 - nPos1 - 1);
        ltrim(strCol12); rtrim(strCol12);

        nPos1 = nPos2;
        nPos2 = strLine.find(strSeparator, nPos1 + 1);
        strCol13 = strLine.substr(nPos1 + 1, nPos2 - nPos1 - 1);
        ltrim(strCol13); rtrim(strCol13);

        nPos1 = nPos2;
        nPos2 = strLine.find(strSeparator, nPos1 + 1);
        strCol14 = strLine.substr(nPos1 + 1, nPos2 - nPos1 - 1);
        ltrim(strCol14); rtrim(strCol14);

        nPos1 = nPos2;
        nPos2 = strLine.find(strSeparator, nPos1 + 1);
        strCol15 = strLine.substr(nPos1 + 1, nPos2 - nPos1 - 1);
        ltrim(strCol15); rtrim(strCol15);

        nPos1 = nPos2;
        nPos2 = strLine.find(strSeparator, nPos1 + 1);
        strCol16 = strLine.substr(nPos1 + 1, nPos2 - nPos1 - 1);
        ltrim(strCol16); rtrim(strCol16);

        nPos1 = nPos2;
        nPos2 = strLine.find(strSeparator, nPos1 + 1);
        strCol17 = strLine.substr(nPos1 + 1, nPos2 - nPos1 - 1);
        ltrim(strCol17); rtrim(strCol17);

        // ConvertedDataFile << strLine << std::endl;        

        ConvertedDataFile << strCol01 << ';'
                          << strCol02 << ';'
                          << strCol03 << ';'
                          << strCol04 << ';'
                          << strCol05 << ';'
                          << strCol06 << ';'
                          << strCol07 << ';'
                          << strCol08 << ';'
                          << strCol09 << ';'
                          << strCol10 << ';'
                          << strCol11 << ';'
                          << strCol12 << ';'
                          << strCol13 << ';'
                          << strCol14 << ';'
                          << strCol15 << ';'
                          << strCol16 << ';'
                          << strCol17 << ';' 
                          << std::endl;
      }
    }
    // Save data to converted file
  }

  ConvertedDataFile.close();
  NISTDataFile.close();
}

// trim from start (in place)
void NIST::ltrim(std::string &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch) {
        return !std::isspace(ch);
    }));
}

// trim from end (in place)
void NIST::rtrim(std::string &s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch) {
        return !std::isspace(ch);
    }).base(), s.end());
}
