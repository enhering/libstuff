#include "DataFit.h"

FunctionNames DataFit::m_eSelectedFittingFunction = GAUSSIAN;      

DataFit::DataFit() {
  m_nMaxIndex = 0;
  m_nStartIndex = 0;

  m_nSelectedSize = 0;
  m_bWindowSelected = false;

  m_afX.reserve(50000);
  m_afY.reserve(50000);
  m_afYSD.reserve(50000);

  m_eSelectedFittingFunction = GAUSSIAN;

  m_bParametersIntialized = false;
}

DataFit::~DataFit() {

}

void DataFit::AddDataPoint(double fX, double fY, double fYSD) {
  m_afX.push_back(fX);
  m_afY.push_back(fY);
  m_afYSD.push_back(fYSD);

  m_nMaxIndex++;

  m_nStartIndex = 0;
  m_nEndIndex = m_nMaxIndex; // ATTENTION HERE! ADDING A NEW DATAPOINT RESETS SEARCH WINDOW!
}

// REQUIREMENT: DATA MUST BE SORTED!
void DataFit::SetSearchWindow(double fXStart, double fXEnd) {
  long nIndex = 0;

  while (nIndex < m_nMaxIndex) {
    if (m_afX[nIndex] > fXStart) {
      break;
    }
    nIndex++;
  }
  m_nStartIndex = nIndex;

  while (nIndex < m_nMaxIndex) {
    if (m_afX[nIndex] > fXEnd) {
      break;
    }
    nIndex++;
  }
  m_nEndIndex = nIndex;

  std::cout << m_nMaxIndex << " datapoints added. " << std::endl
            << "Search window between " << fXStart << "nm and " << fXEnd << "nm." << std::endl
            << "Selected data range starts at index " << m_nStartIndex << " and ends at index " << m_nEndIndex << "." << std::endl
            << std::endl;

  m_bWindowSelected = true;
  m_nSelectedSize = 0;

  m_afSelectedX.clear();
  m_afSelectedY.clear();
  m_afSelectedYSD.clear();

  for (long nIndex = m_nStartIndex; nIndex <= m_nEndIndex; nIndex++) {
    m_afSelectedX.push_back(m_afX[nIndex]);
    m_afSelectedY.push_back(m_afY[nIndex]);
    m_afSelectedYSD.push_back(m_afYSD[nIndex]);

    m_nSelectedSize++;

    // std::cout << "Selected lambda: " << m_afSelectedX[m_nSelectedSize - 1] <<
    //              "nm, intensity: "   << m_afSelectedY[m_nSelectedSize - 1] <<
    //              " counts and std dev: "     << m_afSelectedYSD[m_nSelectedSize - 1] << std::endl;
  }
  std::cout << m_nSelectedSize << " datapoints selected." << std::endl << std::endl;
}

void DataFit::ClearSearchWindow() {
  m_bWindowSelected = false;
  m_nSelectedSize = 0;

  m_afSelectedX.clear();
  m_afSelectedY.clear();
  m_afSelectedYSD.clear();
}

void DataFit::SetFittingFunction(FunctionNames eFunction) {
  m_eSelectedFittingFunction = eFunction;
  std::cout << "Gaussian fitting function selected." << std::endl;
}

int DataFit::GetNumberOfFunctionParameters() {
  switch (m_eSelectedFittingFunction) {
    
    // case POWERLAW:                        m_nNumberOfFittingParameters = 3; break;
    // case POWERLAW2:                       m_nNumberOfFittingParameters = 4; break;
    // case POWERLAW_AND_GAUSSIAN:           m_nNumberOfFittingParameters = 6; break;
    // case POWERLAW_AND_SPECIFIC_HEAT_JUMP: m_nNumberOfFittingParameters = 6; break;
    // case SPECIFIC_HEAT_JUMP:              m_nNumberOfFittingParameters = 3; break;
    // case TRANSITION:                      m_nNumberOfFittingParameters = 5; break;
    case GAUSSIAN:                        m_nNumberOfFittingParameters = 3; break;
    // case GAP:                             m_nNumberOfFittingParameters = 4; break;
    default:                              m_nNumberOfFittingParameters = 3; break;
  }
  
  return m_nNumberOfFittingParameters;
}

void DataFit::InitializeParameters(std::vector<double> & afP) {
  int nNumberOfFittingParameters = GetNumberOfFunctionParameters();

  m_afInitialParameters.clear();

  for (int nI = 0; nI < nNumberOfFittingParameters; nI++) {
    m_afInitialParameters.push_back(afP[nI]);
  }

  m_bParametersIntialized = true;
}

double DataFit::SelectedLaw(const gsl_vector * padIndependents, double dX) {
  double dValue;

  switch (m_eSelectedFittingFunction) {
    
    case GAUSSIAN:
      dValue = DataFit::Gaussian(padIndependents, dX);
      return(dValue);
      break;

    default:
      break;
  }
  return (0);
}


int DataFit::F(const gsl_vector * x,
               void * data,
               gsl_vector * f) {
  
  size_t n = ((struct data *)data)->n;
  double *y = ((struct data *)data)->y;
  double *weights = ((struct data *) data)->weights;

  for (size_t i = 0; i < n; i++) {

    double t = ((struct data *)data)->x[i];
    double Yi = SelectedLaw(x, t);
    gsl_vector_set (f, i, (Yi - y[i])/weights[i]);
  }
  
  return GSL_SUCCESS;
}

int DataFit::DF(const gsl_vector * x,
                void * data,
                gsl_matrix * J) {
  
  size_t n = ((struct data *)data)->n;
  double *sigma = ((struct data *) data)->weights;

  size_t i;
  for (i = 0; i < n; i++) {
    double s = sigma[i];
    double t = ((struct data *)data)->x[i];
    
    switch (m_eSelectedFittingFunction) {
      case GAUSSIAN:
        gsl_matrix_set(J, i, 0, DGaussianDA(gsl_vector_get(x,0), gsl_vector_get(x,1), gsl_vector_get(x,2), t) / s);
        gsl_matrix_set(J, i, 1, DGaussianDB(gsl_vector_get(x,0), gsl_vector_get(x,1), gsl_vector_get(x,2), t) / s);
        gsl_matrix_set(J, i, 2, DGaussianDC(gsl_vector_get(x,0), gsl_vector_get(x,1), gsl_vector_get(x,2), t) / s);
        break;

      default:
        printf("\n *** Unknown function on DataFit::DF. Exiting.\n");
        exit(0);
        break;
    }
  }
  return GSL_SUCCESS;
}

void DataFit::Callback(const size_t iter, void *params, const gsl_multifit_nlinear_workspace *w) {
  gsl_vector *f = gsl_multifit_nlinear_residual(w);
  gsl_vector *x = gsl_multifit_nlinear_position(w);
  double rcond;

  /* compute reciprocal condition number of J(x) */
  gsl_multifit_nlinear_rcond(&rcond, w);

  // fprintf(stderr, "iter %2zu: A = %.4f, lambda = %.4f, b = %.4f, cond(J) = %8.4f, |f(x)| = %.4f\n",
  //         iter,
  //         gsl_vector_get(x, 0),
  //         gsl_vector_get(x, 1),
  //         gsl_vector_get(x, 2),
  //         1.0 / rcond,
  //         gsl_blas_dnrm2(f));
}

void DataFit::Fit() {

  const gsl_multifit_nlinear_type *T = gsl_multifit_nlinear_trust;

  gsl_multifit_nlinear_workspace *w;

  gsl_multifit_nlinear_fdf fdf;

  gsl_multifit_nlinear_parameters fdf_params = gsl_multifit_nlinear_default_parameters();

  std::cout << "Fitting session: " << std::endl;

  // Find number of data points to use
  size_t n;
  if (m_bWindowSelected) {
    n = m_afSelectedX.size();
  }
  else {
    n = m_afX.size();
  }
  std::cout << "  Number of datapoints is " << n << "." << std::endl;

  // Default fitting function is GAUSSIAN. Set with SetFittingFunction above

  // Find number of parameters of fitting function
  const size_t p = GetNumberOfFunctionParameters();
  std::cout << "  Number of fitting function parameters is " << p << "." << std::endl;

  gsl_vector *f;
  gsl_matrix *J;
  gsl_matrix *covar = gsl_matrix_alloc (p, p);
  
  // Load data here
  double x_i[n];
  double y[n];
  double weights[n];
  
  for (long nIndex = 0; nIndex < n; nIndex++) {
    if (m_bWindowSelected) {
      x_i[nIndex]     = m_afSelectedX[nIndex];
      y[nIndex]       = m_afSelectedY[nIndex];
      weights[nIndex] = m_afSelectedYSD[nIndex]; 
    } 
    else {
      x_i[nIndex]     = m_afX[nIndex];
      y[nIndex]       = m_afY[nIndex];
      weights[nIndex] = m_afYSD[nIndex]; 
    }    
  }

  TCanvas canvas("a", "b", 500, 700, 400, 200);

  TMultiGraph *mg = new TMultiGraph();

  TGraph * graph = new TGraph(n, x_i, y);
  graph->SetTitle("LIBS data");
  
  graph->SetMarkerStyle(2);
  graph->SetMarkerColor(4);
  graph->SetMarkerSize(0.3);
  graph->SetLineColor(4);
  graph->SetLineWidth(1);
  graph->GetXaxis()->SetNdivisions(5, kTRUE);

  mg->Add(graph, "APL");

  // graph.Draw("APL");
  // canvas.Update();
  // gSystem->ProcessEvents();

  data d = { n, x_i, y, weights};

  // Parameter initialization;
  std::cout << "  Parameter initialization:" << std::endl;
  double x_init[p]; 
  if (m_bParametersIntialized) {
    for(int nI = 0; nI < p; nI++) {
      x_init[nI] = m_afInitialParameters[nI];
      std::cout << "    p[" << nI << "] is " << x_init[nI] << std::endl;
    }
  } 
  else {
    for(int nI = 0; nI < p; nI++) {
      x_init[nI] = 1.0;
      std::cout << "    p[" << nI << "] is " << x_init[nI] << std::endl;
    }
  }

  gsl_vector_view x   = gsl_vector_view_array(x_init, p);
  gsl_vector_view wts = gsl_vector_view_array(weights, n);

  double chisq, chisq0;
  int status, info;
  size_t i;

  const double xtol = 1e-8;
  const double gtol = 1e-8;
  const double ftol = 0.0;

  // Define the function to be minimized
  fdf.f = F;
  fdf.df = NULL;   /* set to NULL for finite-difference Jacobian */
  fdf.fvv = NULL;     /* not using geodesic acceleration */
  fdf.n = n;
  fdf.p = p;
  fdf.params = &d;

  /* allocate workspace with default parameters */
  w = gsl_multifit_nlinear_alloc (T, &fdf_params, n, p);

  // initialize solver with starting point and weights 
  gsl_multifit_nlinear_winit (&x.vector, &wts.vector, &fdf, w);

  /* compute initial cost function */
  f = gsl_multifit_nlinear_residual(w);
  gsl_blas_ddot(f, f, &chisq0);

  /* solve the system with a maximum of 20 iterations */
  status = gsl_multifit_nlinear_driver(200, xtol, gtol, ftol,
                                       Callback, NULL, &info, w);

  /* compute covariance of best fit parameters */
  J = gsl_multifit_nlinear_jac(w);
  gsl_multifit_nlinear_covar (J, 0.0, covar);

  /* compute final cost */
  gsl_blas_ddot(f, f, &chisq);

#define FIT(i) gsl_vector_get(w->x, i)
#define ERR(i) sqrt(gsl_matrix_get(covar,i,i))

  // fprintf(stderr, "summary from method '%s/%s'\n",
  //         gsl_multifit_nlinear_name(w),
  //         gsl_multifit_nlinear_trs_name(w));
  // fprintf(stderr, "number of iterations: %zu\n",
  //         gsl_multifit_nlinear_niter(w));
  // fprintf(stderr, "function evaluations: %zu\n", fdf.nevalf);
  // fprintf(stderr, "Jacobian evaluations: %zu\n", fdf.nevaldf);
  // fprintf(stderr, "reason for stopping: %s\n",
  //         (info == 1) ? "small step size" : "small gradient");
  // fprintf(stderr, "initial |f(x)| = %f\n", sqrt(chisq0));
  // fprintf(stderr, "final   |f(x)| = %f\n", sqrt(chisq));

  // {
  //   double dof = n - p;
  //   double c = GSL_MAX_DBL(1, sqrt(chisq / dof));

  //   fprintf(stderr, "chisq = %g\n", chisq);    

  //   fprintf(stderr, "chisq/dof = %g\n", chisq / dof);

  //   fprintf (stderr, "A      = %.5f +/- %.5f\n", FIT(0), c*ERR(0));
  //   fprintf (stderr, "lambda = %.5f +/- %.5f\n", FIT(1), c*ERR(1));
  //   fprintf (stderr, "b      = %.5f +/- %.5f\n", FIT(2), c*ERR(2));
  // }

  fprintf (stderr, "status = %s\n", gsl_strerror (status));

  m_fChiSqr = chisq;
  m_fGaussianAmplitude = FIT(0);
  m_fGaussianCenter    = FIT(1);
  m_fGaussianWidth     = FIT(2);
  m_nFitStatus = status;
  
  // double fXStart = x_i[0];
  // double fXEnd   = x_i[n-1];

  for (long nIndex = 0; nIndex < n; nIndex++) {
    y[nIndex] = m_fGaussianAmplitude * exp(-1*((pow((x_i[nIndex]-m_fGaussianCenter),2))/(2*m_fGaussianWidth *m_fGaussianWidth)));
  } 
  TGraph * graph2 = new TGraph(n, x_i, y);

  mg->Add(graph2, "cp");
  mg->Draw("a");

  canvas.Update();
  gSystem->ProcessEvents();

  gsl_multifit_nlinear_free (w);
  gsl_matrix_free (covar);

  sleep(1);
}

double DataFit::Gaussian(const gsl_vector * padIndependents, double dX) {
  double dA = gsl_vector_get(padIndependents,0);
  double dB = gsl_vector_get(padIndependents,1);
  double dC = gsl_vector_get(padIndependents,2);

  return (dA * exp(-1*((pow((dX-dB),2))/(2*dC*dC))));
}

double DataFit::DGaussianDA(double dA, double dB, double dC, double dX) {
  return(-dA * exp(-1*((pow(dX-dB,2))/(2*dC*dC))));
}

double DataFit::DGaussianDB(double dA, double dB, double dC, double dX) {
  return(((dA * (dB - dX)) / (dC*dC)) * exp(-1*((pow((dX-dB),2))/(2*dC*dC))));
}

double DataFit::DGaussianDC(double dA, double dB, double dC, double dX) {
  return(((-1 * dA * pow((dX-dB),2)) / (pow(dC,3))) * exp(-1*((pow((dX-dB),2))/(2*dC*dC))));
}



