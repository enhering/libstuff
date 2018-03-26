#ifndef __FIT_H_
#define __FIT_H_

  #include <stdlib.h>
  #include <stdio.h>

  #include <gsl/gsl_blas.h>
  #include <gsl/gsl_multifit_nlin.h>
  #include <cassert>

  class Fit {
  
    public:
    
      struct data {
        size_t n;
        double * x;
        double * y;
        double * sigma;
      };
    
      Fit() { };
    
      void FitData(mglData *, mglData *,
                  mglData *, double *,
                  double *, int);

    protected:
    
      void PrintState (size_t,
                       gsl_multifit_fdfsolver * );

      virtual void InitGSL(gsl_multifit_function_fdf&) = 0;

  };

#endif

