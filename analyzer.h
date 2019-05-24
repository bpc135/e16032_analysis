#ifndef ANALYZER_H
#define ANALYZER_H

//File Includes
#include "Parameters-ddas.h"
#include "Variables-ddas.h"
#include "/home/bpc135/Utilities_root6/Pixie16Utilities.h"

//C/C++ includes
#include <vector>


using namespace std;

void analyze_event(int crateid, int slotid, int channum, vector<UShort_t> trace, betadecay *bdecay, betadecayvariables *bdecayv, int eventnum, Pixie16Utilities *utils[]);
void analyze_dynode(int crateid, int slotid, int channum, vector<UShort_t> trace, betadecay *bdecay, betadecayvariables *bdecayv, int eventnum, Pixie16Utilities *utils[]);

#endif
