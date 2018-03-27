#ifndef __FIT_H_
#define __FIT_H_

  #include <stdlib.h>
  #include <stdio.h>

  #include <gsl/gsl_matrix.h>
  #include <gsl/gsl_vector.h>
  #include <gsl/gsl_blas.h>
  #include <gsl/gsl_multifit_nlinear.h>

  #include <cassert>

  #include "Base.h"

  class Fit : public Base {
  
    public:
    
      struct data {
        size_t n;
        double * x;
        double * y;
        double * sigma;
      };
    
      Fit() { };
    
      void FitData(std::vector<double> *, std::vector<double> *,
                   std::vector<double> *, double *,
                   double *, int);

    protected:
    
      void PrintState (size_t,
                       gsl_multifit_fdfsolver * );

      virtual void InitGSL(gsl_multifit_function_fdf&) = 0;

  };

#endif

