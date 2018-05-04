#include "LIBStuff.h"

void Cleanup() {
  while (g_apcDataSets.size() != 0) {
    delete (g_apcDataSets[g_apcDataSets.size()-1]);
  }
}

void my_handler(int s){
  printf("%d\n Cleaning up.\n",s);
  Cleanup();
  exit(1); 
}

int main(int argc, char* argv[]) {

  TApplication  app("app", &argc, argv);

  std::cout << "LIBStuff LIBS data analyzer" << std::endl;
  std::cout << VERSION_STR << std::endl << std::endl;

  struct sigaction sigIntHandler;

  sigIntHandler.sa_handler = my_handler;
  sigemptyset(&sigIntHandler.sa_mask);
  sigIntHandler.sa_flags = 0;
  sigaction(SIGINT, &sigIntHandler, NULL);

  g_pcNIST = new NIST();
  // g_cNIST->RetrieveNISTData();
  g_pcNIST->ConvertAndLoadNISTData();

  g_pcLIBS = new LIBS();

  g_pcSpectrum = new Spectrum();

  g_apcDataSets.push_back(new DataSet);
  g_apcDataSets[0]->SetName("aW-2.6us-vis-57mJ");
  g_pcLIBS->LoadData("data/fW_162mJ_vis_1microsegundo.txt", g_apcDataSets[0]);

  g_apcDataSets.push_back(new DataSet);
  g_apcDataSets[1]->SetName("aW-2.6us-vis-57mJ");
  g_pcLIBS->LoadData("data/fW_162mJ_vis_1_5microsegundo.txt", g_apcDataSets[1]);

  g_apcDataSets.push_back(new DataSet);
  g_apcDataSets[2]->SetName("aW-2.6us-vis-57mJ");
  g_pcLIBS->LoadData("data/fW_162mJ_vis_2microsegundo.txt", g_apcDataSets[2]);

  g_apcDataSets.push_back(new DataSet);
  g_apcDataSets[3]->SetName("aW-2.6us-vis-57mJ");
  g_pcLIBS->LoadData("data/fW_162mJ_vis_2_5microsegundo.txt", g_apcDataSets[3]);

  g_apcDataSets.push_back(new DataSet);
  g_apcDataSets[4]->SetName("aW-2.6us-vis-57mJ");
  g_pcLIBS->LoadData("data/fW_162mJ_vis_3microsegundo.txt", g_apcDataSets[4]);

  g_apcDataSets.push_back(new DataSet);
  g_apcDataSets[5]->SetName("aW-2.6us-vis-57mJ");
  g_pcLIBS->LoadData("data/fW_162mJ_vis_3_5microsegundo.txt", g_apcDataSets[5]);

  g_apcDataSets.push_back(new DataSet);
  g_apcDataSets[6]->SetName("aW-2.6us-vis-57mJ");
  g_pcLIBS->LoadData("data/fW_162mJ_vis_4microsegundo.txt", g_apcDataSets[6]);

  g_pcSpectrum->SetNISTObjectPointer(g_pcNIST);
  g_pcSpectrum->SetDatasetArrayRef(g_apcDataSets);
  g_pcSpectrum->ScanData("W");

  app.Run();
}