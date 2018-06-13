/*      File: Parameters.h
	Parameter definitions for the beta-decay setup
	Author: Colin Morton
	Date: September 2001
	Altered to C++ class structure from C struct structure
	July09 by HLC
*/

#ifndef __BDECAYCORRELATOR_H
#define __BDECAYCORREALTOR_H

#include "TObject.h"
#include "TRandom3.h"
#include "TCutG.h"
#include <cstring>


using namespace std;

class IMPLANT
{
 public:
  bool implanted;
  bool decayed;
  double timehigh;
  double timelow;
  double timecfd;
  double time;  /* Time of event, in 30.5 us clockticks */
  long dE1;   /* dE from PIN01 */
  long dE2;   /* dE from PIN02 */
  long dE3;   /* dE from PIN03 */
  long isum;  /* Total implant energy (from sums) */
  long imax;  /* Total implant energy (from max) */
  long tof;   /* Time of flight */
  double dt;    /* Time since last implant */
  int numcorr; /* number of times this implant has been used in a correlation */
};

class DECAY
{
 public:
  double time; /* Time of event, in 30.5 us clockticks */
  double timehigh;
  double timelow;
  double timecfd;
  long dE1;  /* dE from PIN01 */
  long dE2;  /* dE from PIN02 */
  long dE3;  /* dE from PIN03 */
  long isum; /* Total implant energy (from sums) */
  long imax; /* Total implant energy (from max) */
  long tof;  /* Time of flight (for implant event) */
  double dt;   /* Time since implant */
  double decaydt; /* Time between last decay */
  int dnumcorr; /* number of decays correlated to ion*/
};


/* The entire beta-decay set-up */
class Correlator
{
 public:
  
  IMPLANT implant[41][41];
  DECAY decay[41][41];
  
  TCutG *co70i;
  TCutG *goodtof;
  TCutG *misctof;
  TCutG *onetoone;
  TCutG *fe66i1;
  TCutG *fe67i1;
  TCutG* fe67i2;
  TCutG* co69i1;
  TCutG *co70i1;
  TCutG *fe68i1;
  

  //TRandom3 *random3;

 public:

  TCutG *c70Co;

  Correlator();
  ~Correlator();
  void Reset();
  double Correlate(betadecay &bdecay, betadecayvariables &bdecayv, int nstripx, int nstripy, int subseg);
  //int Correlate_snl(betadecay &bdecay, betadecayvariables &bdecayv);
};
#endif
