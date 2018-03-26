#ifndef __FITFUNCTIONS_H_
#define __FITFUNCTIONS_H_

  #include <math.h>
  #include <gsl/gsl_blas.h>
  #include <gsl/gsl_multifit_nlin.h>
  #include "ReportWriter.h"

  double Gaussian(const gsl_vector *, double, int);
  void   WriteGaussianFitEquationOn(char *, double *, int);

  double PowerLaw(const gsl_vector *, double, int);
  void   WritePowerLawFitEquationOn(char *, double *, int);

  double PowerLaw2(const gsl_vector *, double, int);
  void   WritePowerLaw2FitEquationOn(char *, double *, int);

  double SpecificHeatJump(const gsl_vector *, double, int);
  void   WriteSpecificHeatJumpFitEquationOn(char *, double *, int);

  double Transition(const gsl_vector *, double, int);
  void   WriteTransitionFitEquationOn(char *, double *, int);
  double TransitionSecondDerivative(const gsl_vector *, double, int);
  double TransitionThirdDerivative(const gsl_vector *, double, int);

  double Gap(const gsl_vector *, double, int);
  void   WriteGapFitEquationOn(char *, double *, int);

  double DGaussianDA(double, double, double, double);
  double DGaussianDB(double, double, double, double);
  double DGaussianDC(double, double, double, double);

  double DPowerLawDA(double, double, double, double);
  double DPowerLawDB(double, double, double, double);
  double DPowerLawDC(double, double, double, double);

#endif