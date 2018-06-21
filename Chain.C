//B.P. Crider - 05/17/18
//Sum histograms from CeBr3 Implantation scintillator

#include <iostream>
#include <fstream>

#include "TH1.h"
#include "TH2.h"
#include "TCanvas.h"
#include "TFile.h"
#include "TList.h"

#include "dirent.h"

using namespace std;

void Chain(TChain *chain, int run_num) {

  //modify these directories if the input/output directories change
  string infile_dir = "/mnt/analysis/e16032/analyzed_rootfiles/";
  vector<string> files;

  //find all segments for the given run number in the histo_segments directory
  string run_num_str = Form("%04d",run_num);
  string filemodifier = "-e16032_analysis.root";
  
  DIR *dir;
  struct dirent *ent;
  if ((dir = opendir (infile_dir.c_str())) != NULL) {
    /* print all the files and directories within directory */
    while ((ent = readdir (dir)) != NULL) {
      //printf ("%s\n", ent->d_name);
      string s1 = ent->d_name;
      if (s1.find(run_num_str) != std::string::npos) {
	if(s1.find(filemodifier) != std::string::npos) {
	  //std::cout << "found!" << '\n';
	  files.push_back(s1);
	}
      }
    }
    closedir (dir);
  } else {
    /* could not open directory */
    perror ("");
    return EXIT_FAILURE;
  }

  //chain = new TChain("data");
  
  for(int i = 0; i < files.size(); i++){
    
    //add tree
    chain->Add(Form("%s%s",infile_dir.c_str(),files[i].c_str()));
    
    cout << files[i] << " Done!" << endl;
  }
  
  //mergin the tree
  //cout << "Merging data tree" << endl;
  //fout_t->mkdir("data")->cd();
  //chain->Merge(fout_t);
  //tree1_Total->SetName("data");
  //tree1_Total->Write();
  //fout_t->cd();
  //fout_t->Write();
  //fout_t->Close();

  //return *chain;
}
