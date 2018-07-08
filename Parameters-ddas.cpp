/*
  File:  bdecayP.h
  This file creates the parameter tree structure and must be
  edited in parallel with bdecayParameters_standalone.h
  mod 1Sep05 by JPC for 03034 and 05028
*/
#include <iostream>
#include <fstream>
#include <math.h>
#include <vector>
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

//#ifdef HAVE_STD_NAMESPACE
using namespace std;
//#endif

#include <RConfigure.h>
#include <string>

#include "Parameters-ddas.h"

//#if !defined(__CINT__)
ClassImp(bdecayBit);
ClassImp(bdecayClock);
ClassImp(bdecayTrace);
ClassImp(bdecayTac);
ClassImp(bdecayAdc);
ClassImp(bdecayTdc);
ClassImp(bdecayTime);
ClassImp(bdecayPin);
ClassImp(bdecayTotal);
ClassImp(bdecayPid);
ClassImp(bdecayCorr);
ClassImp(bdecaySega);
ClassImp(bdecaySegaTotal);
ClassImp(bdecayDdasDiagnostics);
ClassImp(betadecay);
ClassImp(rootdataout);
ClassImp(bdecayPSPMT);
ClassImp(bdecayI2);
ClassImp(bdecayCLYC);
//#endif

betadecay::betadecay()
{
  //cout << "new  betadecay class " << endl;

  // bit.Reset();
  // clock.Reset();
  // tac.Reset();
  // for(int i=0;i<21;i++){
  //   adc[i].Reset();
  //   tdc[i].Reset();
  //   time[i].Reset();
  // }
  // pin01.Reset();
  // pin02.Reset();
  // pin03.Reset();
  // pin04.Reset();
  // back.Reset();
  // total.Reset();
  // pid.Reset();
  // corr.Reset();
  // sega.Reset();
  // segatotal.Reset();
  // ddasdiagnostics.Reset();
}

betadecay::~betadecay(){

}

rootdataout::rootdataout(){

}
rootdataout::~rootdataout(){

}

void rootdataout::SetOutputValues(betadecay bdecay){
  clock = bdecay.clock;
  tac = bdecay.tac;
  pin01 = bdecay.pin01;
  pin02 = bdecay.pin02;
  pin03 = bdecay.pin03;
  pin04 = bdecay.pin04;
  pid = bdecay.pid;
  corr = bdecay.corr;
  sega = bdecay.sega;
  segatotal = bdecay.segatotal;
  labr3 = bdecay.labr3;
  clyc = bdecay.clyc;
  pspmt = bdecay.pspmt;
  i2 = bdecay.i2;
  ddasdiagnostics = bdecay.ddasdiagnostics;
  
}

void betadecay::Reset()
{
  bit.Reset();
  clock.Reset();
  tac.Reset();
  for(int i=0; i<21; i++){
    adc[i].Reset();
    tdc[i].Reset();
    time[i].Reset();
  }
  pin01.Reset();
  pin02.Reset();
  pin03.Reset();
  pin04.Reset();
  total.Reset();
  pid.Reset();
  corr.Reset();
  //corrv.Reset();
  sega.Reset();
  segatotal.Reset();
  ddasdiagnostics.Reset();
  labr3.Reset();
  clyc.Reset();
  pspmt.Reset();
  i2.Reset();
  
}


void rootdataout::Reset()
{
  clock.Reset();
  tac.Reset();
  pin01.Reset();
  pin02.Reset();
  pin03.Reset();
  pin04.Reset();
  pid.Reset();
  corr.Reset();
  //corrv.Reset();
  sega.Reset();
  ddasdiagnostics.Reset();
  labr3.Reset();
  clyc.Reset();
  pspmt.Reset();
  i2.Reset();
}

void bdecayBit::Reset()
{
  calhitmask = -1;
  corrmask = -1;;
  eventstart = -1;
}

void bdecayClock::Reset()
{
  fast  = 0;
  slow = 0;
  //full = 0;
  cfd = 0;
  current = 0;
  initial = 0;
}

void bdecayTrace::Reset()
{
  trace.clear();
  traceresult.clear();
}


int bdecayTrace::ProcessTrace(int crateid, int slotid, int chanid){

  if(trace.size() > 0) {
    
    //Overflows and Underflows
    int ov = -1;
    int un = -1;
    
    //Max trace height
    double max = -1;
    
    double result = -1;
    
    //Baseline and Trace Areas
    double baseline = 0;
    double energy = 0;
    
    Int_t blo = 100;
    Int_t bhi = 250;
    
    Int_t elo = 350;
    Int_t ehi = 500;
    
    if(slotid>=2 && slotid<=5){
      
      for(int i=0; i<trace.size();i++){
	
	//Check for Overflow of the ADC
	if(trace[i] > 4090) ov = 1;
	
	//Check for Underflow of the ADC
	if(trace[i] < 5) un = 1;
	
	//Find the Maximum Heightof the Trace
	if(trace[i] > max) {
	  max = trace[i];
	}
	
	//Calculate Baseline
	if((i>blo) && (i<bhi)) {
	  baseline += trace[i];
	}
	
	//Calculate Trace Area
	if((i>elo) && (i<ehi)) {
	  energy += trace[i];
	}      
      }
      
      baseline = baseline/(1.0*(bhi-blo));
      energy = energy - baseline/(1.0*(ehi-elo));
      max = max-1.0*baseline;

      
     // determine scatter around baseline
      double std = 0;
      for(int i=blo; i<bhi; i++){
	std += TMath::Power((trace[i]-baseline),2);
      }
      std = TMath::Sqrt(std/(bhi-blo));
      
      vector<double> transcheck;
      double_t mintranscheck=0;
      
      //Check for transients
      for(int i=1; i<trace.size();i++){
	transcheck.push_back(trace[i]-trace[i-1]);
	//	cout<<i<<"  "<<trace[i]<<"  "<<trace[i]-trace[i-1]<<endl;
	if(i<=250) {
	  if(transcheck[i-1]<mintranscheck) {
	    mintranscheck = transcheck[i-1];
	  }
	}
      }
      int transient = 0;
      for(int i=250; i<transcheck.size(); i++) {
	if(transcheck[i]<(2*mintranscheck)) {
	  transient = 1;
	}
      }
      if (transient == 1) {
	//	cout<<"Transient Present!!"<<endl;
      }

      traceresult.push_back(energy);
      traceresult.push_back(ov);
      traceresult.push_back(un);
      traceresult.push_back(std);
      traceresult.push_back(max);
      traceresult.push_back(transient);
      
      if(ov) {
	result = -1;
      }
      if(un) {
	result = -2;
      }
      
      //cout << "ov = " << ov << " " << "un = " << un << " " << "std = " << std << endl;
      
      //cout << "energy " << energy << " " << std << " " << ov << " " << un << endl;
    }
  }
}

void bdecayTac::Reset()
{
  pin01i2nE = -1;
  pin01i2nA = -1;
  pin01i2nT = 0;
  pin01i2nTDC = 0;

  pin01i2sE = -1;
  pin01i2sA = -1;
  pin01i2sT = 0;
  pin01i2sTDC = 0;
  
  pin02i2nE = -1;
  pin02i2nA = -1;
  pin02i2nT = 0;
  pin02i2nTDC = 0;

  pin02i2sE = -1;
  pin02i2sA = -1;
  pin02i2sT = 0;
  pin02i2sTDC = 0;

  i2ni2sE = -1;
  i2ni2sA = -1;
  i2ni2sT = 0;
  i2ni2sTDC = 0;

  pin01rfE = -1;
  pin01rfA = -1;
  pin01rfT = 0;
  pin01rfTDC = 0;

  i2pos = -1;
  i2ncorr = -1;
  i2scorr = -1;
}

void bdecayAdc::Reset()
{
  memset(channel,0,16*sizeof(int));
  for(int i=0; i<16; i++){
    chantrace[i].Reset();
  }
}

void bdecayTdc::Reset()
{
  memset(time,0,16*sizeof(double));
}

void bdecayTime::Reset()
{
  memset(timelow, 0, 16*sizeof(double));
  memset(timehigh, 0, 16*sizeof(double));
  memset(timefull, 0, 16*sizeof(double));
  memset(timecfd, 0, 16*sizeof(double));
}

void bdecayPin::Reset()
{
  //cout << "pin is reset " << endl;
  energy = -1;
  ecorr = -1;
  time = 0;
  ecal = -1;
  eventtdc = -1;
}

void bdecayTotal::Reset()
{
  dsum = -1;
  dmax = -1;
  isum = -1;
  imax = -1;
}

void bdecayPid::Reset()
{
  de1 = -1;
  de2 = -1;
  de3 = -1;
  de4 = -1;
  rftof = -1;
  i2ntof = -1;
  i2stof = -1;
  xfptof = -1;
  isum = -1;
  imax = -1;
}

void bdecayCorr::Reset()
{
  dtimplant = -1;
  //
  itime = -1;
  ide1 = -1;
  ide2 = -1;
  ide3 = -1;
  iisum = -1;
  iimax = -1;
  itof = -1;
  iasum = -1; // yx
  dyecal = -1; // yx
  dyamp = -1; // yx
  idyecal = -1; // yx
  idyamp = -1; // yx
  didyecal = -1; // yx
  didyamp = -1; // yx
  //
  gtimecal = -1;
  gtime = -1;
  gde1 = -1;
  gde2 = -1;
  gde3 = -1;
  gisum = -1;
  gimax = -1;
  gtof = -1;
  // yx
  ltimecal = -1;
  ltime = -1;
  lde1 = -1;
  lde2 = -1;
  lde3 = -1;
  lisum = -1;
  limax = -1;
  ltof = -1;
  // yx
  ctimecal = -1;
  ctime = -1;
  cde1 = -1;
  cde2 = -1;
  cde3 = -1;
  cisum = -1;
  cimax = -1;
  ctof = -1;
  
  dtimecal = -1;
  dtime = -1;
  
  dde1 = -1;
  dde2 = -1;
  dde3 = -1;
  disum = -1;
  dimax = -1;
  dtof = -1;
  flag = -1;
  ImpDecayCorrFlag = 0;
  mult = 0;
  dmult = 0;
  dmax = -1;
  dsum = -1;
  dnumcorr = 0;


  gdtimecal = -1;
  gdtime = -1;
  gdde1 = -1;
  gdde2 = -1;
  gdde3 = -1;
  gdisum = -1;
  gdimax = -1;
  gdtof = -1;
  
}

void bdecaySega::Reset()
{
  memset(timecfd,0,17*sizeof(double));
  memset(time,0,17*sizeof(double));
  memset(timelow,0,17*sizeof(double));
  memset(timehigh, 0,17*sizeof(double));
  memset(dtdc, 0,17*sizeof(double));
  memset(itdc, 0,17*sizeof(double));
  memset(eventtdc,0,17*sizeof(double));
  memset(energy,0,17*sizeof(double));
  memset(goodenergy, 0,17*sizeof(double));
  memset(ecal, 0,17*sizeof(double));
}

void bdecaySegaTotal::Reset()
{
  mult = 0;
  total = -1;
  tot11 = -1;
  xmatrix = -1;
  ymatrix = -1;
  xmat11 = -1;
  ymat11 = -1;
}

void bdecayLaBr3::Reset()
{
  mult = 0;
  memset(timecfd,0,17*sizeof(double));
  memset(timelow,0,17*sizeof(double));
  memset(timehigh, 0,17*sizeof(double));
  memset(dtdc, 0,17*sizeof(double));
  memset(itdc, 0,17*sizeof(double));
  memset(eventtdc,0,17*sizeof(double));
  memset(energy,0,17*sizeof(double));
  memset(goodenergy, 0,17*sizeof(double));
  memset(ecal, 0,17*sizeof(double));
  memset(time, 0, 17*sizeof(double));
  //initialize the differences to -10000000 so we know when it wasn't set
  memset(tdiff, 100000, 17*17*sizeof(double));
  memset(amp,0,17*sizeof(double));
  memset(ampcal,0,17*sizeof(double));
  memset(area,0,17*sizeof(double));
  memset(areacal,0,17*sizeof(double));
  //initialize the differences to -10000000 so we know when it wasn't set
  memset(dynode_tdiff, 100000, 17*sizeof(double));
  //add in parameters that output the CFD filter time + trigger point
  memset(timecfd_filt,0,17*sizeof(double));
  memset(cfdtrigpt_filt,0,17*sizeof(double));
  
}

void bdecayLaBr3Total::Reset()
{
  mult = 0;
  total = -1;
  tot11 = -1;
  xmatrix = -1;
  ymatrix = -1;
  xmat11 = -1;
  ymat11 = -1;}


void bdecayCLYC::Reset()
{
  mult = 0;

  memset(timecfd,0,17*sizeof(double));
  memset(timelow,0,17*sizeof(double));
  memset(timehigh, 0,17*sizeof(double));
  memset(dtdc, 0,17*sizeof(double));
  memset(itdc, 0,17*sizeof(double));
  memset(eventtdc,0,17*sizeof(double));
  memset(energy,0,17*sizeof(double));
  memset(ecal, 0,17*sizeof(double));
  memset(time, 0, 17*sizeof(double));
  //initialize the differences to -10000000 so we know when it wasn't set
  memset(tdiff, 100000, 17*17*sizeof(double));
  memset(amp,0,17*sizeof(double));
  memset(ampcal,0,17*sizeof(double));
  memset(area,0,17*sizeof(double));
  memset(areacal,0,17*sizeof(double));
  //initialize the differences to -10000000 so we know when it wasn't set
  memset(dynode_tdiff, 100000, 17*sizeof(double));
  //add in parameters that output the CFD filter time + trigger point
  memset(timecfd_filt,0,17*sizeof(double));
  memset(cfdtrigpt_filt,0,17*sizeof(double));
  memset(shortsum,0,17*sizeof(double));
  memset(longsum,0,17*sizeof(double));
  memset(fullsum,0,17*sizeof(double));

}

void bdecayCLYCTotal::Reset()
{
  mult = 0;
  total = -1;
  tot11 = -1;
  xmatrix = -1;
  ymatrix = -1;
  xmat11 = -1;
  ymat11 = -1;
}

void bdecayPSPMT::Reset()
{

  memset(ahit,0,257*sizeof(int));
  memset(aenergy,0,257*sizeof(double));
  memset(aecal,0,257*sizeof(double));
  memset(atime,0,257*sizeof(double));
  memset(aoverflow,0,257*sizeof(double));
  memset(atdiff,0,257*sizeof(double));
  memset(atdiffmin,0,257*sizeof(double));
  memset(aeventtdc,0,257*sizeof(double));

  memset(dyenergies,-1,10*sizeof(double));
  memset(dyamps,-1,10*sizeof(double));
  memset(dytimes,0,10*sizeof(double));

  memset(aamp,0,257*sizeof(double));
  memset(aampcal,0,257*sizeof(double));
  
  memset(aarea,0,257*sizeof(double));
  memset(aareacal,0,257*sizeof(double));
  
  memset(lowpoint,0,257*sizeof(double));
  memset(baseline,0,257*sizeof(double));
  memset(ratio,0,257*sizeof(double));
  
  memset(pixmult,0,257*sizeof(int));
  
  memset(loaenergy,0,257*sizeof(double));
  memset(loaecal,0,257*sizeof(double));
  memset(loaamp,0,257*sizeof(double));
  memset(loaampcal,0,257*sizeof(double));
  memset(loaarea,0,257*sizeof(double));
  memset(loaareacal,0,257*sizeof(double));
  
  memset(loatime,0,257*sizeof(double));
  memset(loatdiff,0,257*sizeof(double));

  
  //dynode
  dyenergy = -1;
  dyecal = -1;
  dyamp = -1;
  dyampcal = -1;
  dyarea = -1;
  dyareacal = -1;
  dyoverflow = -1;
  dytime = 0;
  dytimemin = 0;
  dytdiff = 0;
  dytdiffmin = 0;
  dyeventtdc = -1;
  dymult = 0;

  testsig = -1;
  testamp = -1;
  testarea = -1;
  testtime = -1;
  testoverflow = -1;
  
  //double pulse
  dyE1_double = -1;
  dyT1_double = -1;
  dyE1_steepness_double = -1;
  dyE1_decayTime_double = -1;
  dyE2_double = -1;
  dyT2_double = -1;
  dyE2_steepness_double = -1;
  dyE2_decayTime_double = -1;
  dychisq_double = -1;
  dyoffset_double = -1;
  dytdiffE1E2 = -1;
  dyE1_single = -1;
  dyT1_single = -1;
  dychisq_single = -1;
  dyoffset_single = -1;
  dyE1_steepness_single = -1; 
  dyE1_decayTime_single = -1;
  dyE1real_single = -1;
  dyE1real_double = -1;
  dyE2real_double = -1;
  
  //anode
  asum = -1;
  asumcent = -1;
  asumcent50 = -1;
  amax = 0;
  amin = DBL_MAX;
  aminimp = DBL_MAX;
  amaxcent = 0;
  amult = 0;
  amult_raw = 0;
  amaxx = 0;
  amaxy = 0;
  aminx = 0;
  aminy = 0;
  aminimpx = 0;
  aminimpy = 0;
  amaxtime = 0;
  amintime = 0;
  aoverflowcount = 0;
  posxEcent = -1;
  posyEcent = -1;
  posxEcent50 = -1;
  posyEcent50 = -1;
  amaxcent = 0;
  asumcent = -1;
  asumcent50 = -1;
  posxampcent = -1;

  posyampcent = -1;
  posxampcent50 = -1;
  posyampcent50 = -1;
  aampmaxcent = -1;
  aampmaxcentcal = -1;
  aampsumcent = -1;
  aampsumcent50 = -1;
  aampmax = -1;
  aampmaxcal = -1;
  aampmaxx = -1;
  aampmaxy = -1;
  
  posxareacent = -1;
  posyareacent = -1;
  posxareacent50 = -1;
  posyareacent50 = -1;
  aareamaxcent = -1;
  aareamaxcentcal = -1;
  aareasumcent = -1;
  aareasumcent50 = -1;
  
  loasum = -1;
  loamax = -1;
  loamaxtime = 0;
  loamult = 0;
  loamaxx = 0;
  loamaxy = 0;
  loposxEcent = -1;
  loposyEcent = -1;
  loposxEcent50 = -1;
  loposyEcent50 = -1;
  loposxampcent = -1;
  loposyampcent = -1;
  loposxampcent50 = -1;
  loposyampcent50 = -1;
  loposxareacent = -1;
  loposyareacent = -1;
  loposxareacent50 = -1;
  loposyareacent50 = -1;
  
  aampsum = -1;
  aampsumcent = -1;
  aampsumcent50 = -1;
  aampmax = -1;
  aampmaxcent = -1;
  aampmaxcentcal = -1;
  aampmaxx = -1;
  aampmaxy = -1;
  aampmaxtime = 0.0;
  aareasum = -1;
  aareasumcent = -1;
  aareasumcent50 = -1;
  aareamaxx = -1;
  aareamaxy = -1;
  aareamax = -1;
  aareamaxcent = -1;
  aareamaxcentcal = -1;
  aareamaxtime = 0.0;
  loaampsum = -1;
  loaareasum = -1;
  loaampmax = -1;
  loaareamax = -1;
 
}

void bdecayI2::Reset()
{
  i2pos = -1;
  i2postdc = 0;
  i2poscorr = -1;
  
  //I2N
  i2nenergy = -1;
  i2necal = -1;
  i2ntime = 0;
  i2ntof = 0;
  i2ntdc = 0;
  i2ncorr = -1;

  //I2S
  i2senergy = -1;
  i2secal = -1;
  i2stime = 0;
  i2stof = 0;
  i2stdc = 0;
  i2scorr = -1;
  //cout << "i2 is reset " << endl;
  
}

void bdecayDdasDiagnostics::Reset()
{
  cmult = 0;
  eventlength = -1;
  tdiffevent = -1;
  memset(adchit, 0,20*sizeof(int));

}
