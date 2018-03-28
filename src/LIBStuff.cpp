#include "LIBStuff.h"

int main(int argc, char* argv[]) {

  TApplication  app("app", &argc, argv);

  std::cout << "LIBStuff LIBS data analyzer" << std::endl;
  std::cout << VERSION_STR << std::endl << std::endl;

  // g_cNIST.RetrieveNISTData();
  // g_cNIST.ConvertNISTData();
  g_cLIBS.LoadData("data/aW_2_6_micro_segundos_vis_57_mJ.txt");
  g_cLIBS.ScanData();

  app.Run();
}