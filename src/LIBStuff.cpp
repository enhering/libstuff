#include "LIBStuff.h"

int main(int argc, char* argv[]) {

  TApplication  app("app", &argc, argv);

  std::cout << "LIBStuff LIBS data analyzer" << std::endl;
  std::cout << VERSION_STR << std::endl << std::endl;

  g_pcNIST = new NIST();
  // g_cNIST->RetrieveNISTData();
  g_pcNIST->ConvertAndLoadNISTData();

  g_pcLIBS = new LIBS();
  g_pcLIBS->SetNISTObjectPointer(g_pcNIST);
  g_pcLIBS->LoadData("data/aW_2_6_micro_segundos_vis_57_mJ.txt");
  g_pcLIBS->ScanData("W I");

  app.Run();
}