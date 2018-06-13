#ifndef ANALYZER_H
#define ANALYZER_H

//File Includes
#include "Parameters-ddas.h"
#include "Variables-ddas.h"
#include "/user/e16032/Utilities_root6/Pixie16Utilities.h"

//C/C++ includes
#include <vector>


using namespace std;

void SetInitialPixie16Utilities(Pixie16Utilities *myUtils, vector<UShort_t> tr);

void analyze_event(int crateid, int slotid, int channum, vector<UShort_t> trace, betadecay *bdecay, betadecayvariables *bdecayv, int eventnum);



#endif
