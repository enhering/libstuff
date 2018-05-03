#ifndef NISTDATA_H__
  #define NISTDATA_H__

  #include <vector>
  #include "Base.h"

  class NISTData : public Base {

    public: 
      NISTData();
      ~NISTData();

      long                     m_nNumLines;
      std::vector<std::string> m_astrIonizationLevel;
      std::vector<float>       m_afObservedWavelength_nm;
      std::vector<float>       m_afRitzWavelength_nm;
      std::vector<float>       m_afUncertainty_nm;
      std::vector<std::string> m_astrRelInt;
      std::vector<float>       m_afAki_sec_minus_one;
      std::vector<std::string> m_astrAcc;
      std::vector<float>       m_afEi_eV;
      std::vector<float>       m_afEf_eV;
      std::vector<std::string> m_astrLowerLevelConf;
      std::vector<std::string> m_astrLowerLevelTerm;
      std::vector<std::string> m_astrLowerLevelJ;
      std::vector<std::string> m_astrUpperLevelConf;
      std::vector<std::string> m_astrUpperLevelTerm;
      std::vector<std::string> m_astrUpperLevelJ;
  };

#endif
