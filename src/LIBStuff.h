#include "NIST.h"
#include "LIBS.h"
#include "DataSet.h"
#include "Spectrum.h"

#include <vector>

#include "TApplication.h"

NIST * g_pcNIST;
LIBS * g_pcLIBS;
Spectrum * g_pcSpectrum;

std::vector<DataSet *> g_apcDataSets;

