#include "FitFunctions.h"

// GAUSSIAN

double Gaussian(const gsl_vector * padIndependents, double dX, int nOffset) {
  double dA = gsl_vector_get(padIndependents,0 + nOffset);
  double dB = gsl_vector_get(padIndependents,1 + nOffset);
  double dC = gsl_vector_get(padIndependents,2 + nOffset);

  return (dA * exp(-1*((pow((dX-dB),2))/(2*dC*dC))));
}

void WriteGaussianFitEquationOn(char * pchEquation, double * padParameters, int nOffset) {
  double dA = padParameters[0 + nOffset];
  double dB = padParameters[1 + nOffset];
  double dC = padParameters[2 + nOffset];

  sprintf(pchEquation, "$%f * e^{-\\frac{(x-%f)^2}{2*%f^2}}$", dA, dB, dC);
}

// POWER LAW

double PowerLaw(const gsl_vector * padIndependents, double dX, int nOffset) {
  double dRho0 = gsl_vector_get(padIndependents,0 + nOffset);
  double dA = gsl_vector_get(padIndependents,1 + nOffset);
  double dN = gsl_vector_get(padIndependents,2 + nOffset);

  return (dRho0 + dA * pow(dX, dN));
}

void WritePowerLawFitEquationOn(char * pchEquation, double * padParameters, int nOffset) {
  double dRho0 = padParameters[0 + nOffset];
  double dA = padParameters[1 + nOffset];
  double dN = padParameters[2 + nOffset];

  sprintf(pchEquation, "$%f + %f * x^{%f}$", dRho0, dA, dN);
}

// POWER LAW II

double PowerLaw2(const gsl_vector * padIndependents, double dX, int nOffset) {
  double dRho0 = gsl_vector_get(padIndependents,0 + nOffset);
  double dA = gsl_vector_get(padIndependents,1 + nOffset);
  double dB = gsl_vector_get(padIndependents,2 + nOffset);
  double dN = gsl_vector_get(padIndependents,3 + nOffset);

  return (dRho0 + dA * pow(dX, 2) - dB * pow(dX, dN));
}

void WritePowerLaw2FitEquationOn(char * pchEquation, double * padParameters, int nOffset) {
  double dRho0 = padParameters[0 + nOffset];
  double dA = padParameters[1 + nOffset];
  double dB = padParameters[2 + nOffset];
  double dN = padParameters[3 + nOffset];

  sprintf(pchEquation, "$%f + %f * x^2 - %f * x^{%f}$", dRho0, dA, dB, dN);
}


// SPECIFIC HEAT

  // A customization of the alternative form of
  // http://en.wikipedia.org/wiki/ExGaussian_distribution
  // was used. An exponent was inserted on x on the exponential term
  // and a multiplier over t on the same fraction. This enabled
  // me to change the shape of the specific heat jump.
  // This solution was discovered by trial and error.

double SpecificHeatJump(const gsl_vector * padIndependents, double dX, int nOffset) {
  double dA      = gsl_vector_get(padIndependents,0 + nOffset); 
  double dWidth  = gsl_vector_get(padIndependents,1 + nOffset); 
  double dCenter = gsl_vector_get(padIndependents,2 + nOffset); 
  double dD      = 1; //gsl_vector_get(padIndependents,2 + nOffset); 

  return ( dA * pow(dX, dD) * (0.5 + erf((-1 * dX + dCenter) / dWidth) / 2));
}

void WriteSpecificHeatJumpFitEquationOn(char * pchEquation, double * padParameters, int nOffset) {
  double dAmplitude = padParameters[0+nOffset]; 
  double dWidth     = padParameters[1+nOffset]; 
  double dCenter    = padParameters[2+nOffset]; 
  double dD         = 1;//padParameters[2+nOffset]; 
  sprintf(pchEquation, "$%f * x^{%f} * (0.5 + erf(\\frac{-x+%f}{%f})/2)$", dAmplitude, dD, dCenter, dWidth);
}

// TRANSITION

double Transition(const gsl_vector * padIndependents, double dX, int nOffset) {
  double dR0 = gsl_vector_get(padIndependents,0 + nOffset); 
  double dC  = gsl_vector_get(padIndependents,1 + nOffset); 
  double dB  = gsl_vector_get(padIndependents,2 + nOffset); 
  double dA  = gsl_vector_get(padIndependents,3 + nOffset); 
  double dN  = gsl_vector_get(padIndependents,4 + nOffset); 

  return ( (1-(1/(exp((dX-dC)/(dB*dC))+1))) * dA * pow(dX, dN) + dR0 );
}

void WriteTransitionFitEquationOn(char * pchEquation, double * padParameters, int nOffset) {
  double dR0 = padParameters[0 + nOffset]; 
  double dC  = padParameters[1 + nOffset];
  double dB  = padParameters[2 + nOffset];
  double dA  = padParameters[3 + nOffset];
  double dN  = padParameters[4 + nOffset];

  sprintf(pchEquation, "$(1-(\\frac{1}{exp(\\frac{(x-%f)}{(%f*%f)})+1})) * %f * x^{%f} + %f$", dC, dB, dC, dA, dN, dR0);
}

double TransitionThirdDerivative(const gsl_vector * padIndependents, double dX, int nOffset) {
  double dR0 = gsl_vector_get(padIndependents,0 + nOffset); 
  double dC  = gsl_vector_get(padIndependents,1 + nOffset); 
  double dB  = gsl_vector_get(padIndependents,2 + nOffset); 
  double dA  = gsl_vector_get(padIndependents,3 + nOffset); 
  double dN  = gsl_vector_get(padIndependents,4 + nOffset); 

  double dF1 = exp(dX/(dB*dC));
  double dF2 = exp(1/dB);
  double dResult =  (1 / ( pow(dB,3) * pow(dC,3) * pow((dF2+dF1),4) )) * dA * dF1 * pow(dX,(-3+dN)) * ( pow(dB,3) * pow(dC,3) * pow((dF2 + dF1),3) * dN * (2 - 3 * dN + pow(dN,2) ) + 3 * pow(dB,2) * pow(dC,2) * dF2 * pow((dF2+dF1),2) * (-1+dN) * dN * dX + 3 * dB * dC * dF2 * ( exp(2/dB) - exp((2*dX)/(dB*dC)) ) * dN * pow(dX,2) + dF2 * (exp(2/dB) + exp((2*dX)/(dB*dC)) - 4 * exp((dC+dX)/(dB*dC))) * pow(dX,3));
  return(dResult);
}

double TransitionSecondDerivative(const gsl_vector * padIndependents, double dX, int nOffset) {
  double dR0 = gsl_vector_get(padIndependents,0 + nOffset); 
  double dC  = gsl_vector_get(padIndependents,1 + nOffset); 
  double dB  = gsl_vector_get(padIndependents,2 + nOffset); 
  double dA  = gsl_vector_get(padIndependents,3 + nOffset); 
  double dN  = gsl_vector_get(padIndependents,4 + nOffset); 

  double dF1 = exp(dX/(dB*dC));
  double dF2 = exp(1/dB);
  double dResult =  (dA * dF1 * pow(dX,(-2+dN)) * ( pow(dB,2) * pow(dC,2) * pow((dF2+dF1),2) * (-1+dN) * dN + 2 * dB * dC * dF2 * (dF2 + dF1) * dN * dX + dF2 * (dF2 - dF1) * pow(dX,2) )) / ( pow(dB,2) *  pow(dC,2) *  pow((dF2+dF1),3) );
  return(dResult);
}

// GAP

double Gap(const gsl_vector * padIndependents, double dX, int nOffset){
  double dRho0  = gsl_vector_get(padIndependents,0 + nOffset); 
  double dA     = gsl_vector_get(padIndependents,1 + nOffset); 
  double dN     = gsl_vector_get(padIndependents,2 + nOffset); 
  double dDelta = gsl_vector_get(padIndependents,3 + nOffset); 

  return ( dRho0 + dA * pow(dX, dN) * exp(-1 * (dDelta / dX) ));
}

void WriteGapFitEquationOn(char * pchEquation, double * padParameters, int nOffset) {
  double dRho0  = padParameters[0 + nOffset];
  double dA     = padParameters[1 + nOffset]; 
  double dN     = padParameters[2 + nOffset]; 
  double dDelta = padParameters[3 + nOffset];

  sprintf(pchEquation, "  Fit equation is $%f * %f * x^{%f} * e^{-\\frac{%f}{x}}$", dRho0, dA, dN, dDelta);
}

// DERIVATIVES _____________________________________________________










double DGaussianDA(double dA, double dB, double dC, double dX) {
  return(-dA * exp(-1*((pow(dX-dB,2))/(2*dC*dC))));
}

double DGaussianDB(double dA, double dB, double dC, double dX) {
  return(((dA * (dB - dX)) / (dC*dC)) * exp(-1*((pow((dX-dB),2))/(2*dC*dC))));
}

double DGaussianDC(double dA, double dB, double dC, double dX) {
  return(((-1 * dA * pow((dX-dB),2)) / (pow(dC,3))) * exp(-1*((pow((dX-dB),2))/(2*dC*dC))));
}


double DPowerLawDA(double dA, double dB, double dC, double dX) {
  return(1);
}

double DPowerLawDB(double dA, double dB, double dC, double dX) {
  return(pow(dX, dC));
}

double DPowerLawDC(double dA, double dB, double dC, double dX) {
  return(pow(dX, dC) * log(dX));
}