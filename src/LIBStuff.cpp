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

  g_acDataSets.push_back(new DataSet);
  g_pcLIBS->LoadData("data/aW_2_6_micro_segundos_vis_57_mJ.txt", g_apcDataSets[0]);
  g_acDataSets.SetName("aW-2.6us-vis-57mJ");

  g_acDataSets.push_back(new DataSet);
  g_pcLIBS->LoadData("data/aW_2_6_micro_segundos_vis_57_mJ.txt", g_apcDataSets[0]);
  g_acDataSets.SetName("aW-2.6us-vis-57mJ");

  g_acDataSets.push_back(new DataSet);
  g_pcLIBS->LoadData("data/aW_2_6_micro_segundos_vis_57_mJ.txt", g_apcDataSets[0]);
  g_acDataSets.SetName("aW-2.6us-vis-57mJ");

  g_acDataSets.push_back(new DataSet);
  g_pcLIBS->LoadData("data/aW_2_6_micro_segundos_vis_57_mJ.txt", g_apcDataSets[0]);
  g_acDataSets.SetName("aW-2.6us-vis-57mJ");

  g_acDataSets.push_back(new DataSet);
  g_pcLIBS->LoadData("data/aW_2_6_micro_segundos_vis_57_mJ.txt", g_apcDataSets[0]);
  g_acDataSets.SetName("aW-2.6us-vis-57mJ");

  g_pcLIBS->ScanData("W");

  app.Run();
}