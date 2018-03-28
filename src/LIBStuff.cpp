#include "LIBStuff.h"

int main(int argc, char* argv[]) {

  std::cout << "LIBStuff LIBS data analyzer" << std::endl;
  std::cout << VERSION_STR << std::endl << std::endl;

  // TApplication  app("app", &argc, argv);
  //g_cNIST.RetrieveNISTData();
  //g_cNIST.ConvertNISTData();
  g_cLIBS.LoadData("data/aW_2_6_micro_segundos_vis_57_mJ.txt");
  g_cLIBS.ScanData();

  // app.Run();
}