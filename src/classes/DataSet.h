#ifndef DATASET_H_
  #define DATASET_H_

  class DataSet : public Base {
    public:
      DataSet();
      ~DataSet();

      void AddDataPoint(double, double, double);
      void SetSearchWindow(double, double);
      void ClearSearchWindow();

    private:
      std::vector<double> m_afX;
      std::vector<double> m_afY;
      std::vector<double> m_afYSD;

      std::vector<double> m_afSelectedX;
      std::vector<double> m_afSelectedY;
      std::vector<double> m_afSelectedYSD;      

      long m_nStartIndex, m_nEndIndex, m_nMaxIndex;
      bool m_bWindowSelected;
      long m_nSelectedSize;

      long m_nMaxIndex;
  };


#endif