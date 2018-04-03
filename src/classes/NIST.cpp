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
 
    std::cout << "Downloading NIST data...";

    out = fopen(filename, "wb");
 
  /* write to this file */ 
    curl_easy_setopt(m_pcCurl, CURLOPT_WRITEDATA, out);
    curl_easy_setopt(m_pcCurl, CURLOPT_URL, "https://physics.nist.gov/cgi-bin/ASD/lines1.pl?spectra=w+I%3B+W+II&limits_type=0&low_w=&upp_w=&unit=1&de=0&format=1&line_out=0&remove_js=on&en_unit=1&output=0&bibrefs=1&page_size=1500&show_obs_wl=1&show_calc_wl=1&unc_out=1&order_out=0&max_low_enrg=&show_av=2&max_upp_enrg=&tsb_value=0&min_str=&A_out=0&intens_out=on&max_str=&allowed_out=1&forbid_out=1&min_accur=&min_intens=&conf_out=on&term_out=on&enrg_out=on&J_out=on&submit=Retrieve+Data");
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
  std::cout << "Ok." << std::endl;
}

void NIST::ConvertAndLoadNISTData() {
 
  std::ifstream    NISTDataFile;
  std::ofstream    ConvertedDataFile;
  std::string      strLine;
  std::string      strStartPattern = "<pre>";
  std::string      strEndPattern = "</pre>";

  std::cout << "Converting NIST data...";

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

  NISTData cNISTData;
  long     nLineCounter = 0;

  double fValue;
  std::string strElement;

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
          (strLine.front() != '-') &&
          (strLine.front() != 'S') &&
          (strLine[1]      != 'p')) { 

        std::size_t nPos1, nPos2; 
        std::string strCol01A, strCol01B, strCol02,  strCol03, strCol04, strCol05, strCol06, strCol07, 
                    strCol08A, strCol08B, strCol09, strCol10, strCol11, strCol12, strCol13, 
                    strCol14,  strCol15,  strCol16, strCol17;
        std::string strSeparator = "|";

        nPos1 = 0;
        nPos2 = strLine.find(" ", 0);
        strCol01A = strLine.substr(0, nPos2 - nPos1);
        ltrim(strCol01A); rtrim(strCol01A);
        strElement = strCol01A;

        nPos1 = nPos2;
        nPos2 = strLine.find(strSeparator, nPos1 + 1);
        strCol01B = strLine.substr(0, nPos2 - nPos1);
        ltrim(strCol01B); rtrim(strCol01B);
        
        cNISTData.m_astrIonizationLevel.push_back(strCol01B);
        // std::cout << nLineCounter << ": ";

        // std::cout << " 01: " << strCol01;

        nPos1 = nPos2;
        nPos2 = strLine.find(strSeparator, nPos1 + 1);
        strCol02 = strLine.substr(nPos1 + 1, nPos2 - nPos1 - 1);
        ltrim(strCol02); rtrim(strCol02);

        // std::cout << " 02: " << strCol02;
        fValue = 0.0;
        if (! strCol02.empty()) {
          fValue = stof(strCol02);
        }
        cNISTData.m_afObservedWavelength_nm.push_back(fValue);

        nPos1 = nPos2;
        nPos2 = strLine.find(strSeparator, nPos1 + 1);
        strCol03 = strLine.substr(nPos1 + 1, nPos2 - nPos1 - 1);
        ltrim(strCol03); rtrim(strCol03);

        // std::cout << " 03: " << strCol03;
        fValue = 0.0;
        if (! strCol03.empty()) {
          fValue = stof(strCol03);
        }
        cNISTData.m_afRitzWavelength_nm.push_back(fValue);
        
        nPos1 = nPos2;
        nPos2 = strLine.find(strSeparator, nPos1 + 1);
        strCol04 = strLine.substr(nPos1 + 1, nPos2 - nPos1 - 1);
        ltrim(strCol04); rtrim(strCol04);

        // std::cout << " 04: " << strCol04;
        fValue = 0.0;
        if (! strCol04.empty()) {
          fValue = stof(strCol04);
        }
        cNISTData.m_afUncertainty_nm.push_back(fValue);

        nPos1 = nPos2;
        nPos2 = strLine.find(strSeparator, nPos1 + 1);
        strCol05 = strLine.substr(nPos1 + 1, nPos2 - nPos1 - 1);
        ltrim(strCol05); rtrim(strCol05);

        // std::cout << " 05: " << strCol05;
        cNISTData.m_astrRelInt.push_back(strCol05);

        nPos1 = nPos2;
        nPos2 = strLine.find(strSeparator, nPos1 + 1);
        strCol06 = strLine.substr(nPos1 + 1, nPos2 - nPos1 - 1);
        ltrim(strCol06); rtrim(strCol06);

        // std::cout << " 06: " << strCol06;
        fValue = 0.0;
        if (! strCol06.empty()) {
          fValue = stof(strCol06);
        }
        cNISTData.m_afAki_sec_minus_one.push_back(fValue);

        nPos1 = nPos2;
        nPos2 = strLine.find(strSeparator, nPos1 + 1);
        strCol07 = strLine.substr(nPos1 + 1, nPos2 - nPos1 - 1);
        ltrim(strCol07); rtrim(strCol07);

        // std::cout << " 07: " << strCol07;
        cNISTData.m_astrAcc.push_back(strCol07);

        nPos1 = nPos2;
        nPos2 = strLine.find("-", nPos1 + 1);
        if (nPos2 != std::string::npos) {
          strCol08A = strLine.substr(nPos1 + 1, nPos2 - nPos1 - 1);
          ltrim(strCol08A); rtrim(strCol08A);
        }
        else {
          strCol08A = "0";
          nPos2 = nPos1;
        }

        // std::cout << " 08A: " << strCol08A;
        fValue = 0.0;
        if (! strCol08A.empty()) {
          fValue = stof(strCol08A);          
        }
        cNISTData.m_afEi_eV.push_back(fValue);

        nPos1 = nPos2;
        nPos2 = strLine.find(strSeparator, nPos1 + 1);    
        strCol08B = strLine.substr(nPos1 + 1, nPos2 - nPos1 - 1);
        ltrim(strCol08B); rtrim(strCol08B);

        // std::cout << " 08B: " << strCol08B;
        fValue = 0.0;
        if (! strCol08B.empty()) {
          fValue = stof(strCol08B);
        }
        cNISTData.m_afEf_eV.push_back(fValue);

        nPos1 = nPos2;
        nPos2 = strLine.find(strSeparator, nPos1 + 1);
        strCol09 = strLine.substr(nPos1 + 1, nPos2 - nPos1 - 1);
        ltrim(strCol09); rtrim(strCol09);

        // std::cout << " 09: " << strCol09;
        cNISTData.m_astrLowerLevelConf.push_back(strCol09);

        nPos1 = nPos2;
        nPos2 = strLine.find(strSeparator, nPos1 + 1);
        strCol10 = strLine.substr(nPos1 + 1, nPos2 - nPos1 - 1);
        ltrim(strCol10); rtrim(strCol10);

        // std::cout << " 10: " << strCol10;
        cNISTData.m_astrLowerLevelTerm.push_back(strCol10);

        nPos1 = nPos2;
        nPos2 = strLine.find(strSeparator, nPos1 + 1);
        strCol11 = strLine.substr(nPos1 + 1, nPos2 - nPos1 - 1);
        ltrim(strCol11); rtrim(strCol11);

        // std::cout << " 11: " << strCol11;
        cNISTData.m_astrLowerLevelJ.push_back(strCol11);

        nPos1 = nPos2;
        nPos2 = strLine.find(strSeparator, nPos1 + 1);
        strCol12 = strLine.substr(nPos1 + 1, nPos2 - nPos1 - 1);
        ltrim(strCol12); rtrim(strCol12);

        // std::cout << " 12: " << strCol12;
        cNISTData.m_astrUpperLevelConf.push_back(strCol12);

        nPos1 = nPos2;
        nPos2 = strLine.find(strSeparator, nPos1 + 1);
        strCol13 = strLine.substr(nPos1 + 1, nPos2 - nPos1 - 1);
        ltrim(strCol13); rtrim(strCol13);

        // std::cout << " 13: " << strCol13;
        cNISTData.m_astrUpperLevelTerm.push_back(strCol13);

        nPos1 = nPos2;
        nPos2 = strLine.find(strSeparator, nPos1 + 1);
        strCol14 = strLine.substr(nPos1 + 1, nPos2 - nPos1 - 1);
        ltrim(strCol14); rtrim(strCol14);

        // std::cout << " 14: " << strCol14;
        cNISTData.m_astrUpperLevelJ.push_back(strCol14);

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
        // std::cout << std::endl;

        cNISTData.m_nNumLines++;
        nLineCounter++; 

        ConvertedDataFile << nLineCounter << " "
                          << strCol01A << ';'
                          << strCol01B << ';'
                          << strCol02 << ';'
                          << strCol03 << ';'
                          << strCol04 << ';'
                          << strCol05 << ';'
                          << strCol06 << ';'
                          << strCol07 << ';'
                          << strCol08A << ';'
                          << strCol08B << ';'
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

  m_acNISTDataByElement[strElement] = cNISTData;

  ConvertedDataFile.close();
  NISTDataFile.close();

  std::cout << "Ok." << std::endl << std::endl;

}
