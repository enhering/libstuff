#include "LIBStuff.h"

int main(int argc, char* argv[]) {

  // TApplication  app("app", &argc, argv);
  //g_cNIST.RetrieveNISTData();
  g_cNIST.ConvertNISTData();
  g_cLIBS.LoadData("data/aW_2_6_micro_segundos_vis_57_mJ.txt");

  // app.Run();
}