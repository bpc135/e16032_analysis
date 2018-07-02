/* Create full events from channel events and process. */

#include <iostream>
#include <ostream>
#include <fstream>
#include <sstream>
#include <math.h>
#include <vector>

#include <string.h>
#include <stdlib.h>
#include <cstdlib>
#include <unistd.h>
#include <ctype.h>
#include <stdio.h>
#include <time.h>
#include <sys/time.h>
#include <malloc.h>
#include <istream>
#include <iomanip>
#include <cmath>
#include <sstream>

#include "TFile.h"
#include "TTree.h"
#include "TH1.h"
#include "TH2.h"
#include "TCutG.h"
#include "TGraph.h"
#include "TMath.h"
#include "TCanvas.h"
#include "TPaveText.h"

#include "TROOT.h"

//#include "Parameters-ddas.h"
#include "ddaschannel.h"
#include "DDASEvent.h"
#include "DDASRootFitEvent.h"
#include "DDASRootFitHit.h"
#include "DDASFitHit.h"
#include "functions.h"
#include "lmfit.h"

//#include "Variables-ddas.h"
#include "Cuts.h"
#include "unpacker.h"

#include "/user/e16032/Utilities_root6/Pixie16Utilities.h"

#ifdef HAVE_STD_NAMESPACE
using namespace std;
#endif

//*******************************************************

//INPUT

//Set the path for the input files. The run numbers are selected by the command
//line when executing the program. A string like "/run-0'RunNum'-00.root"
//will be appended to specify the input files when executing the program like this:
//"./AnalyzeDDAS 'RunNum' char inPath[] = "/mnt/analysis/e11503/rootfiles/rawroot";  
char inPath[] = "/mnt/events/e16032/rootfiles";  
string Run_Number;

//*******************************************************

//OUTPUT

//specify the folder for the output files
char outPath[] = "/mnt/analysis/e16032/rootfiles/";

//specify the name for the output files "run-0'RunNum'" is automatically added
//to the file name before what is specified below
const char outName[] = "_processed.root";

//*******************************************************

int timewindow = 1000;

double front_icent_counter = 0;
double front_dcent_counter = 0;
double back_icent_counter = 0;
double back_dcent_counter = 0;
double decay_counter = 0;
double implant_counter = 0;
double decay_fandb_diff = 0;
double implant_fandb_diff =0;


int main(int argc, char **argv) {

  int run_number;
  
  TFile *rootfile_in;
  TFile *rootfile_out;
  TTree *tree_in;
  TTree *tree_out;

  time_t start, stop;

  char *pFilein = argv[1];
  char *pFileout = argv[2];
  
  cout << "Input file: \t " << pFilein << endl;
  cout << "Output file: \t" << pFileout << endl;
  
  //char *pFileIn = pFilein;
  
  cout<<pFilein[35]<<pFilein[36]<<pFilein[37]<<pFilein[38]<<endl;
  Run_Number = string(&pFilein[35]);
  
  // run_number = atoi(Run_Number.c_str());
  
  time(&start);

  int filenum = atoi(*argv);
    
  cout << "Run number = " << filenum << endl;
      
  int havedata = 0;
  rootfile_in = new TFile(pFilein);
  if(!rootfile_in->IsZombie()){
    havedata = 1;
  }
  else{
    cout << "error opening file" << endl;
    havedata = 0;
  }

  //output file and tree
  rootfile_out = new TFile(pFileout, "RECREATE");
  tree_out = new TTree("data","data");
  
  //go through the data in the file
  if(havedata){
    //Get the data
    //tree_in = (TTree*)rootfile_in->Get("dchan");
    tree_in = (TTree*)rootfile_in->Get("DDASFit");

    //Unpack and Analyze data
    unpack_data(tree_in, tree_out, Run_Number);

    //Close the rootfile input
    rootfile_in->Close();
    delete rootfile_in;
  
  }//end if havedata

  //Write Output
  rootfile_out->cd();
  //cout << "write tree output" << endl;
  tree_out->Write();
  //cout << "tree output written" << endl;
  rootfile_out->Close();

  time(&stop);
  float diff = difftime(stop,start);
  cout << endl << endl;
  cout << "Elapsed time = " << diff << " seconds" << endl;
}
  
