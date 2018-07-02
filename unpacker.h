#ifndef UNPACKER_H
#define UNPACKER_H

//DDAS includes
#include "ddaschannel.h"
#include "DDASEvent.h"
#include "DDASRootFitEvent.h"
#include "DDASRootFitHit.h"
#include "DDASFitHit.h"
#include "lmfit.h"

//File Includes
#include "Parameters-ddas.h"
#include "Variables-ddas.h"
#include "/user/e16032/Utilities_root6/Pixie16Utilities.h"

//ROOT includes
#include "TTree.h"

//C/C++ includes
#include <vector>

using namespace std;

//Function prototypes
void SetInitialPixie16Utilities(Pixie16Utilities *myUtils, vector<UShort_t> tr);

int unpack_data(TTree *tree_in, TTree *tree_out, string Run_Number);
void reset_channel_list(vector<DDASRootFitHit*> channellist,  vector<DDASRootFitHit*>::iterator channellist_it);
//int unpack_event(int eventnum, betadecay *bdecay, betadecayvariables *bdecayv);
int unpack_event(int eventnum, betadecay *bdecay, betadecayvariables *bdecayv, vector<DDASRootFitHit*> channellist,vector<DDASRootFitHit*>::iterator channellist_it, Pixie16Utilities *utils[]);

void unpack_double_pulse(int eventnum, betadecay *bdecay, vector <RootHitExtension> *fitdata);
  
#endif
