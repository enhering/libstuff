#ifndef DATAFIT_H__
  #define DATAFIT_H__

  #include "Base.h"
  #include <vector>

  enum FunctionList {
    POWERLAW_AND_GAUSSIAN,
    POWERLAW,
    POWERLAW2,
    POWERLAW_AND_SPECIFIC_HEAT_JUMP,
    SPECIFIC_HEAT_JUMP,
    TRANSITION,
    GAUSSIAN,
    GAP
  };

  class DataFit : public Base {
    public:
      DataFit();
      ~DataFit();

      void AddDataPoint(double, double, double);
      void SetSearchWindow(double, double);
      void Fit();


    private:
      std::vector<double> m_afX;
      std::vector<double> m_afY;
      std::vector<double> m_afYSD;

      std::vector<double> m_afSelectedX;
      std::vector<double> m_afSelectedY;
      std::vector<double> m_afSelectedYSD;      

      std::vector<double> m_afParameters;

      double m_fStdDev;

      long m_nStartIndex, m_nEndIndex, m_nMaxIndex;
      bool m_bWindowSelected;
      long m_nSelectedSize;
  };

#endif