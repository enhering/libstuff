#include "Fit.h"

void Fit::FitData(mglData * cXData, mglData * cYData,
                 mglData * cYStdDevData,
                 double * dStdDev,
                 double * padParameters,
                 int nNumParameters) {

  const gsl_multifit_fdfsolver_type *T;

  gsl_multifit_fdfsolver *s;
  gsl_multifit_function_fdf f;
  
  int status;

  unsigned int i, iter = 0;
  const size_t n = cXData->nx;
  const size_t p = nNumParameters;

  //printf("Inside FitData. %d parameters, %d datapoints.\n", p, n);

  gsl_matrix *covar = gsl_matrix_alloc (p, p);

  // Load data here
  double y[n];
  double x_i[n];
  double sigma[n];
  
  for (long nIndex = 0; nIndex < cXData->nx; nIndex++) {
    x_i[nIndex]   = cXData->a[nIndex];
    y[nIndex]     = cYData->a[nIndex];
    sigma[nIndex] = cYStdDevData->a[nIndex];
  }
  
  struct data d = { n, x_i, y, sigma};

  double x_init[nNumParameters];
  
  for (int nN = 0; nN < nNumParameters; nN++) {
    x_init[nN] = padParameters[nN];
  }

  gsl_vector_view x = gsl_vector_view_array (x_init, p);

  InitGSL(f);
  
  f.n = n;
  f.p = p;
  f.params = &d;

  T = gsl_multifit_fdfsolver_lmsder;

  s = gsl_multifit_fdfsolver_alloc (T, n, p);
  gsl_multifit_fdfsolver_set (s, &f, &x.vector);

  //PrintState (iter, s);

  //printf("      Iterations: ");
  do {
     iter++;
     status = gsl_multifit_fdfsolver_iterate (s);

     //printf ("status = %s\n", gsl_strerror (status));
     //PrintState (iter, s);

     if (status)
       break;

     status = gsl_multifit_test_delta (s->dx, s->x,
                                       1e-4, 1e-4);
  }
  while (status == GSL_CONTINUE && iter < 100);

  if (iter < 10) {
    printf(" G(%ld) ", iter);
  }
  else {
    printf(" B "); 
  }

  gsl_multifit_covar (s->J, 0.0, covar);

  #define FIT(i) gsl_vector_get(s->x, i)
  #define ERR(i) sqrt(gsl_matrix_get(covar,i,i))

  double chi = gsl_blas_dnrm2(s->f);

  for (int nN = 0; nN < nNumParameters; nN++) {
    padParameters[nN] = gsl_vector_get(s->x, nN);
  }

  gsl_multifit_fdfsolver_free (s);
  gsl_matrix_free (covar);
  
  *dStdDev = chi;
}

void Fit::PrintState (size_t iter,
                      gsl_multifit_fdfsolver * s) {
  printf ("iter: %3lu x = % 15.8f % 15.8f % 15.8f % 15.8f"
         "|f(x)| = %g\n",
         iter,
         gsl_vector_get (s->x, 0), 
         gsl_vector_get (s->x, 1),
         gsl_vector_get (s->x, 2), 
         gsl_vector_get (s->x, 3),
         gsl_blas_dnrm2 (s->f));
}


