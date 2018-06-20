/*      File: Parameters.h
	Parameter definitions for the beta-decay setup
	Author: Colin Morton
	Date: September 2001
	Altered to C++ class structure from C struct structure
	July09 by HLC
*/

#ifndef __BDECAYPARAMETERS_H
#define __BDECAYPARAMETERS_H

#include "TObject.h"
#include "TMath.h"
#include <cstring>


using namespace std;

// Bit registers 

class bdecayBit : public TObject
{
 private:

 public:
  int calhitmask;
  int corrmask;
  int eventstart;

 public:
  //bdecayBit() {};
  //~bdecayBit() {};
  void Reset();

  ClassDef(bdecayBit,1);
};

/* Clock */
class bdecayClock: public TObject
{
 private:

 public:
  double fast;
  double slow;
  //double full;
  double  cfd;
  double  current;
  double  initial;

 public: 
  //bdecayClock() {};
  //~bdecayClock() {};
  void Reset();

  ClassDef(bdecayClock,1);
};

/* trace capture */
class bdecayTrace : public TObject
{
 private:

 public:
  vector<UShort_t> trace;
  vector<Double_t> traceresult;

 public:
  void Reset();
  int ProcessTrace(int crateid, int slotid, int chanid);
  
  ClassDef(bdecayTrace,1);
};

/* TACs */
class bdecayTac: public TObject
{
 private:
  
 public:
  int pin01i2nE = -1;
  int pin01i2nA = -1;
  int pin01i2nT = 0;
  
  int pin01i2sE = -1;
  int pin01i2sA = -1;
  int pin01i2sT = 0;
  
  int pin02i2nE = -1;
  int pin02i2nA = -1;
  int pin02i2nT = 0;

  int pin02i2sE = -1;
  int pin02i2sA = -1;
  int pin02i2sT = 0;

  int i2ni2sE = -1;
  int i2ni2sA = -1;
  int i2ni2sT = 0;

  int pin01rfE = -1;
  int pin01rfA = -1;
  int pin01rfT = 0;

  int i2pos = -1;
  int i2ncorr = -1;
  int i2scorr = -1;
  
 public:
  void Reset();

  ClassDef(bdecayTac,1);
};

/* ADCs */
class bdecayAdc: public TObject
{
 private:
  
 public:
  int channel[16];
  bdecayTrace chantrace[16];

 public:
  //bdecayAdc() {};
  //~bdecayAdc() {};
  void Reset();
  
  ClassDef(bdecayAdc,1);
};

/* TDCs */
class bdecayTdc: public TObject
{
 private:

 public:
  double time[16];

 public:
  //bdecayTdc() {};
  //~bdecayTdc() {};
  void Reset();

  ClassDef(bdecayTdc,1);
};

/* TIMES */
class bdecayTime: public TObject
{
 private:

 public:
  double timelow[16];
  double timehigh[16];
  double timefull[16];
  double timecfd[16];
  
 public:
  //bdecayTime() {};
  //~bdecayTime() {};
  void Reset();

  ClassDef(bdecayTime,1);
};

/* PIN */
class bdecayPin: public TObject
{
 private:
  
 public:
  int energy;
  int ecorr;
  double time;
  int ecal;
  int eventtdc;
  
 public:
  void Reset();

  ClassDef(bdecayPin,1);
};


/* Total */
class bdecayTotal: public TObject
{
 private:
  
 public:
  int dsum;
  int dmax;
  int isum;
  int imax;
  
 public:
  //bdecayTotal() {};
  //~bdecayTotal() {};
  void Reset();

  ClassDef(bdecayTotal,1);
};

/* PID */
class bdecayPid: public TObject
{
 private:
  
 public:
  int de1;
  int de2;
  int de3;
  int de4;
  int rftof;
  int i2ntof;
  int i2stof;
  int xfptof;
  int isum;
  int imax;
  
 public:
  //bdecayPid() {};
  //~bdecayPid() {};
  void Reset();

  ClassDef(bdecayPid,1);
};

/* Correlator */
class bdecayCorr: public TObject
{
 private:
  
 public:
  double dtimplant;
  double itime;
  int ide1;
  int ide2;
  int ide3;
  float iisum;
  float iimax;
  int itof;
  double gtimecal;
  double gtime;
  int gde1;
  int gde2;
  int gde3;
  float gisum;
  float gimax;
  int gtof;
  double dtimecal;
  double dtime;
  int dfrontch;
  int dbackch;
  int frontch;
  int backch;
  int dde1;
  int dde2;
  int dde3;
  float disum;
  float dimax;
  int dtof;
  int flag;
  int ImpDecayCorrFlag;
  int mult;
  int dmult;
  float dmax;
  float dsum;
  int dnumcorr; //temp SNL
  
  double gdtimecal;
  double gdtime;
  int gdde1;
  int gdde2;
  int gdde3;
  int gdisum;
  int gdimax;
  int gdtof;
  float gdfronthiecal[17];
  float gdbackhiecal[17];

 public:
  //bdecayCorr() {};
  //~bdecayCorr() {};
  void Reset();

  ClassDef(bdecayCorr,2);
};

/* SeGA */
class bdecaySega: public TObject
{
 private:
  
 public:
  double timecfd[17];
  double timelow[17];
  double timehigh[17];
  double time[17];
  double dtdc[17];
  double itdc[17];
  double eventtdc[17];
  double energy[17];
  double goodenergy[17];
  double ecal[17];
  
 public:
  //bdecaySega() {};
  //~bdecaySega() {};
  void Reset();

  ClassDef(bdecaySega,1);
};

/* Gamma Energy Totals */
class bdecaySegaTotal: public TObject
{
 private:
  
 public:
  int mult;
  int total;
  int tot11;
  int xmatrix;
  int ymatrix;
  int xmat11;
  int ymat11;
  
 public:
  //bdecaySegaTotal() {};
  //~bdecaySegaTotal() {};
  void Reset();
  
  ClassDef(bdecaySegaTotal,1);
};

/* PSPMT */
class bdecayPSPMT: public TObject
{
 private:

 public:
  //dynode
  double dyenergy;
  double dyecal;
  double dyamp;
  double dyarea;
  int dyoverflow;
  double dytime;
  double dytdiff;  //time difference between dynode and anode with highest amplitude
  //double pulse
  double dyE1;
  double dyT1;
  double dyE2;
  double dyT2;
  double dytdiffE1E2;

  //anode
  double aenergy[258];
  double aecal[258];
  double asum;
  double atime[258];
  double amax;
  int aoverflow[258];
  int amult;
  int amaxx;
  int amaxy;
  double amaxtime;
  double posxEcent;
  double posyEcent;
  double posxEcent50;
  double posyEcent50;
  double amaxcent;
  double asumcent;
  double asumcent50;
  
  double aamp[258];
  double aampcal[258];
  double aampsum;
  int aampmax;
  int aampmaxx;
  int aampmaxy;
  double aampmaxtime;
  double posxampcent;
  double posyampcent;
  double posxampcent50;
  double posyampcent50;
  double aampmaxcent;
  double aampmaxcentcal;
  double aampsumcent;
  double aampsumcent50;
  
  double aarea[258];
  double aareacal[258];
  double aareasum;
  int aareamax;
  int aareamaxx;
  int aareamaxy;
  double aareamaxtime;
  double posxareacent;
  double posyareacent;
  double posxareacent50;
  double posyareacent50;
  double aareamaxcent;
  double aareamaxcentcal;
  double aareasumcent;
  double aareasumcent50;
  
  double lowpoint[258];
  double baseline[258];
  double ratio[258];

  int pixmult[258];
  
  double loaenergy[258];
  double loaecal[258];
  double loaamp[258];
  double loaampcal[258];
  double loaampsum;
  double loaarea[258];
  double loaareacal[258];
  double loaareasum;
  double loasum;
  double loatime[258];
  double loatdiff[258];
  double loamax;
  double loamaxtime;
  int loamult;
  int loamaxx;
  int loamaxy;
  int loaampmax;
  int loaareamax;
  double loposxEcent;
  double loposyEcent;
  double loposxEcent50;
  double loposyEcent50;
  double loposxampcent;
  double loposyampcent;
  double loposxampcent50;
  double loposyampcent50;
  double loposxareacent;
  double loposyareacent;
  double loposxareacent50;
  double loposyareacent50;

  

 public:
  void Reset();

  ClassDef(bdecayPSPMT,1);

};

/* I2 Scintillator */
class bdecayI2: public TObject
{
 private:
  
 public:
  int i2pos;
  int i2postdc;
  int i2poscorr;
  
  //I2N
  int i2nenergy;
  int i2necal;
  double i2ntime;
  double i2ntof;
  int i2ntdc;
  int i2ncorr;

  //I2S
  int i2senergy;
  int i2secal;
  double i2stime;
  double i2stof;
  int i2stdc;
  int i2scorr;
  
 public:
  //bdecayI2() {};
  //~bdecayI2() {};
  void Reset();

  ClassDef(bdecayI2,1);
};


/* LaBr3 */
class bdecayLaBr3: public TObject
{
 private:
  
 public:
  double timecfd[17];
  double timelow[17];
  double timehigh[17];
  double dtdc[17];
  double itdc[17];
  double eventtdc[17];
  double energy[17];
  double goodenergy[17];
  double ecal[17];
  double amp[17];
  double ampcal[17];
  double area[17];
  double areacal[17];
  

  int mult;
  double time[17];
  double tdiff[17][17];

  double dynode_tdiff[17];
  double timecfd_filt[17];
  double cfdtrigpt_filt[17];
  
  
 public:
  //bdecayLaBr3() {};
  //~bdecayLaBr3() {};
  void Reset();

  ClassDef(bdecayLaBr3,1);
};

/* Gamma Energy Totals */
class bdecayLaBr3Total: public TObject
{
 private:
  
 public:
  int mult;
  int total;
  int tot11;
  int xmatrix;
  int ymatrix;
  int xmat11;
  int ymat11;
  
 public:
  //bdecayLaBr3Total() {};
  //~bdecayLaBr3Total() {};
  void Reset();
  
  ClassDef(bdecayLaBr3Total,1);
};

/* CLYC */
class bdecayCLYC: public TObject
{
 private:
  
 public:
  double timecfd[17];
  double timelow[17];
  double timehigh[17];
  double dtdc[17];
  double itdc[17];
  double eventtdc[17];
  double energy[17];
  double goodenergy[17];
  double ecal[17];
  double amp[17];
  double ampcal[17];
  double area[17];
  double areacal[17];
  

  int mult;
  double time[17];
  double tdiff[17][17];

  double dynode_tdiff[17];
  double timecfd_filt[17];
  double cfdtrigpt_filt[17];
  
  
 public:
  //bdecayCLYC() {};
  //~bdecayCLYC() {};
  void Reset();

  ClassDef(bdecayCLYC,1);
};

/* Gamma Energy Totals */
class bdecayCLYCTotal: public TObject
{
 private:
  
 public:
  int mult;
  int total;
  int tot11;
  int xmatrix;
  int ymatrix;
  int xmat11;
  int ymat11;
  
 public:
  //bdecayCLYCTotal() {};
  //~bdecayCLYCTotal() {};
  void Reset();
  
  ClassDef(bdecayCLYCTotal,1);
};

/* DDAS diagnostics */
class bdecayDdasDiagnostics: public TObject
{
 private:

 public:
  int cmult;
  int eventlength;
  int tdiffevent;
  int adchit[20];
  
 public:
  //bdecayDdasDiagnostics() {};
  //~bdecayDdasDiagnostics() {};
  void Reset();
  
  ClassDef(bdecayDdasDiagnostics,1);
};

/* The entire beta-decay set-up */
class betadecay: public TObject
{
 public:
  bdecayBit bit;
  bdecayClock clock;
  bdecayTac tac;
  bdecayAdc adc[21];
  bdecayTdc tdc[21];
  bdecayTime time[21];
  bdecayPin pin01;
  bdecayPin pin02;
  bdecayPin pin03;
  bdecayPin pin04;
  bdecayTotal total;
  bdecayPid pid;
  bdecayCorr corr;
  bdecayCorr corrv;
  bdecaySega sega;
  bdecaySegaTotal segatotal;
  bdecayLaBr3 labr3;
  bdecayCLYC clyc;
  bdecayPSPMT pspmt;
  bdecayI2 i2;
  bdecayLaBr3Total labr3total;
  bdecayCLYCTotal clyctotal;
  bdecayDdasDiagnostics ddasdiagnostics;

 public:
  betadecay();
  ~betadecay();
  void Reset();

  ClassDef(betadecay,1);
};

class rootdataout : public TObject
{
 public:
  bdecayClock clock;
  bdecayTac tac;
  bdecayPin pin01;
  bdecayPin pin02;
  bdecayPin pin03;
  bdecayPin pin04;
  bdecayPid pid;
  bdecayCorr corr;
  //bdecayCorr corrv;
  bdecaySega sega;
  bdecaySegaTotal segatotal;
  bdecayLaBr3 labr3;
  bdecayCLYC clyc;
  bdecayI2 i2;
  bdecayPSPMT pspmt;
  
  bdecayDdasDiagnostics ddasdiagnostics;
 public:
  rootdataout();
  ~rootdataout();
  void Reset();
  void SetOutputValues(betadecay bdecay);

  ClassDef(rootdataout,1);
};
#endif
