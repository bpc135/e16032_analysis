#ifndef UNPACKER_H
#define UNPACKER_H

//DDAS includes
#include "ddaschannel.h"
#include "DDASEvent.h"
#include "DDASRootFitEvent.h"
#include "DDASRootFitHit.h"

//File Includes
#include "Parameters-ddas.h"
#include "Variables-ddas.h"

//ROOT includes
#include "TTree.h"

//C/C++ includes
#include <vector>

using namespace std;

//Function prototypes
int unpack_data(TTree *tree_in, TTree *tree_out, string Run_Number);
void reset_channel_list(vector<DDASRootFitHit*> channellist,  vector<DDASRootFitHit*>::iterator channellist_it);
//int unpack_event(int eventnum, betadecay *bdecay, betadecayvariables *bdecayv);
int unpack_event(int eventnum, betadecay *bdecay, betadecayvariables *bdecayv, vector<DDASRootFitHit*> channellist,vector<DDASRootFitHit*>::iterator channellist_it);

void MapChannels(int crateid, int slotid, int channum, vector<UShort_t> trace, betadecay *bdecay, betadecayvariables *bdecayv, int eventnum);
  
#endif
