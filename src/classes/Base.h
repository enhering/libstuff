#ifndef BASE_H__
  #define BASE_H__

  #include <stdio.h>
  #include <iostream>
  #include <fstream>
  #include <vector>

  class Base {
    public: 
      Base();
      ~Base();
      void ltrim(std::string &);
      void rtrim(std::string &);

    private:

  };
#endif