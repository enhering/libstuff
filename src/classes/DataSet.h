#ifndef DATASET_H_
  #define DATASET_H_

  #include "Base.h"

  class DataSet : public Base {
    public:
      DataSet();
      ~DataSet();
      void SetName(std::string strName) { m_strName = strName; }
      void AddDataPoint(double, double, double);
      void SetDomainWindow(double, double, uint8_t);
      void ClearDomainWindow();

    private:
      std::string m_strName;

      std::vector<double> m_afX;
      std::vector<double> m_afY;
      std::vector<double> m_afYSD;

      std::vector<double> m_afSelectedX;
      std::vector<double> m_afSelectedY;
      std::vector<double> m_afSelectedYSD;      

      long m_nStartIndex, m_nEndIndex;
      bool m_bWindowSelected;
      long m_nSelectedSize;

      long m_nMaxIndex;
  };


#endif