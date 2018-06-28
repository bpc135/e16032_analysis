#ifdef HAVE_STD_NAMESPACE
using namespace std;
#endif

#include <RConfigure.h>
#include <string>
#include <fstream>
#include <iostream>
#include <istream>
#include <iomanip>
#include <math.h>

#include "Variables-ddas.h"

using namespace std;

bdecayvClockCalibrator::bdecayvClockCalibrator(){

}
bdecayvClockCalibrator::~bdecayvClockCalibrator(){

}

void bdecayvClockCalibrator::Initialize()
{
  calib        = 0.1952;
  scale        = 32768.;
  max          = 10.;
  max_daughter = 10;
}

bdecayvPinCalibrator::bdecayvPinCalibrator(){

}
bdecayvPinCalibrator::~bdecayvPinCalibrator(){

}

void bdecayvPinCalibrator::Initialize()
{
  slope     = 1.;
  intercept = 0.;
  thresh    = 0;
  ithresh   = 0;
  iuld      = 100000;
}

bdecayvTacCalibrator::bdecayvTacCalibrator(){

}
bdecayvTacCalibrator::~bdecayvTacCalibrator(){

}

void bdecayvTacCalibrator::Initialize() 
{
  i2ncorr      = 1.;
  i2ncorrpos   = 0.;
  i2scorr      = 1.;
  i2scorrpos   = 0.;
  pin01corr1    = 1.;
  pin01corr2    = 0.;
  pin01corrpos = 0.;

  i2scorr_offset = 0;
  pin01corr = 0;
  pin01corr_offset = 0;
  pin02corr = 0;
  pin02corr_offset = 0;
}

bdecayvDSSDCalibrator::bdecayvDSSDCalibrator(){

}
bdecayvDSSDCalibrator::~bdecayvDSSDCalibrator(){

}

void bdecayvDSSDCalibrator::Initialize()
{
  for(int i = 0; i<17; i++){
    hioffset[i] = 0.;
    hislope[i]  = 1.;
    loslope[i]  = 1.;

    hipulseareaoffset[i] = 0;
    hipulseareaslope[i] = 1.0;

    hipulsefitoffset[i] = 0;
    hipulsefitslope[i] = 1.0;
  }
}

bdecayvDSSDThresh::bdecayvDSSDThresh(){
}
bdecayvDSSDThresh::~bdecayvDSSDThresh(){
}

void bdecayvDSSDThresh::Initialize()
{
  for(int i = 0; i<17; i++){  
    hithresh[i] = 35000.;
    lothresh[i] = 35000.;
    duld[i]     = 0.;
    loduld[i]   = 0.;
  }
}

bdecayvSSSDCalibrator::bdecayvSSSDCalibrator(){
}
bdecayvSSSDCalibrator::~bdecayvSSSDCalibrator(){
}

void bdecayvSSSDCalibrator::Initialize()
{
  for(int i = 0; i<17; i++){  
    offset[i] = 0.;
    slope[i]  = 1.;
    thresh[i] = 0.;
    uld[i]    = 4095.;
  }
}

bdecayvHitFlag::bdecayvHitFlag(){
}
bdecayvHitFlag::~bdecayvHitFlag(){
}
void bdecayvHitFlag::Initialize()
{
  pin01   = 0;
  pin02   = 0;
  pin03   = 0;
  pin04   = 0;
  fronthi = 0;
  frontlo = 0;
  frontloall=0;
  backhi  = 0;
  backlo  = 0;
  backloall=0;
  sega    = 0;
  labr3   = 0;
  clyc    = 0;
  i2n     = 0;
  i2s     = 0;
  dynode  = 0;
}  

bdecayvCorrelation::bdecayvCorrelation(){
}
bdecayvCorrelation::~bdecayvCorrelation(){
}
void bdecayvCorrelation::Initialize()
{
  reset      = 0;
  resetclock = 0;
  minimplant = 0.;
}

//PSPMT
bdecayvPSPMTCalibrator::bdecayvPSPMTCalibrator(){
}
bdecayvPSPMTCalibrator::~bdecayvPSPMTCalibrator(){
}
void bdecayvPSPMTCalibrator::Initialize()
{
  for(int i = 0; i < 258; i++){
    offset[i] = 0.;
    slope[i] = 1.;
    thresh[i] = 0.;
    ithresh[i] = 0.;
    uld[i] = 4095.;
    time_deviation[i] = 0;
  }
}

//I2
bdecayvI2Calibrator::bdecayvI2Calibrator(){

}
bdecayvI2Calibrator::~bdecayvI2Calibrator(){

}

void bdecayvI2Calibrator::Initialize()
{
  slope     = 1.;
  intercept = 0.;
  thresh    = 0;
}

//LaBr3
bdecayvLaBr3::bdecayvLaBr3(){
}
bdecayvLaBr3::~bdecayvLaBr3(){
}
void bdecayvLaBr3::Initialize(){
  for(int i = 0; i < 18; i++){
    square[i] = 0;
    slope[i] = 1.;
    intercept[i] = 0.;
    toffset[i] = 0.;
    thresh[i]    = 0.;
    hithresh[i]  = 65536.;
    dynode_toffset[i] = 0.;
    for(int j = 0; j < 17; j++){
      labr_toffset[i][j] = 0.;
    }
  }
  lotime = 0.;
  hitime = 8192.;
  
  
  
}

//CLYC
bdecayvCLYC::bdecayvCLYC(){
}
bdecayvCLYC::~bdecayvCLYC(){
}
void bdecayvCLYC::Initialize(){
  for(int i = 0; i < 18; i++){
    square[i] = 0;
    slope[i] = 1.;
    intercept[i] = 0.;
    toffset[i] = 0.;
    thresh[i]    = 0.;
    hithresh[i]  = 65536.;
    dynode_toffset[i] = 0.;
    for(int j = 0; j < 17; j++){
      clyc_toffset[i][j] = 0.;
    }
  }
  lotime = 0.;
  hitime = 8192.;
  
  
  
}


bdecayvPidCalibrator::bdecayvPidCalibrator(){
}
bdecayvPidCalibrator::~bdecayvPidCalibrator(){
}

void bdecayvPidCalibrator::Initialize()
{
  de1_scale       = 1.;
  de1_offset      = 0.;
  de1_tof_corr    = 0.;
  de1_tof_offset  = 0.;
  de2_scale       = 1.;
  de2_offset      = 0.;
  de2_tof_corr    = 0.;
  de2_tof_offset  = 0.;
  de3_scale       = 1.;
  de3_offset      = 0.;
  de3_tof_corr    = 0.;
  de3_tof_offset  = 0.;
  de4_scale       = 1.;
  de4_offset      = 0.;
  de4_tof_corr    = 0.;
  de4_tof_offset  = 0.;
  isum_scale      = 1.;
  isum_offset     = 0.;
  isum_tof_corr   = 0.;
  isum_tof_offset = 0.;
  imax_scale      = 1.;
  imax_offset     = 0.;
  imax_tof_corr   = 0.;
  imax_tof_offset = 0.;
}

bdecayvSegaCalibrator::bdecayvSegaCalibrator(){
}
bdecayvSegaCalibrator::~bdecayvSegaCalibrator(){
}
void bdecayvSegaCalibrator::Initialize()
{
  for(int i=0; i<17;i++){
    square[i]    = 0.;
    slope[i]     = 1.;
    intercept[i] = 0.;
    thresh[i]    = 0.;
    hithresh[i]  = 65536.;
  }
  lotime = 0.;
  hitime = 8192.;
}

betadecayvariables::betadecayvariables(){

}
betadecayvariables::~betadecayvariables(){

}
void betadecayvariables::Initialize(){

  clock.Initialize();
  clockisomer.Initialize();
  pin01.Initialize();
  pin02.Initialize();
  pin03.Initialize();
  pin04.Initialize();
  tac.Initialize();
  hit.Initialize();
  corr.Initialize();
  pid.Initialize();
  sega.Initialize();
  i2n.Initialize();
  i2s.Initialize();

}

void betadecayvariables::ReadCLYC(char *Name){

  cout<<"Reading clyc"<<endl;
  cout<<Name<<endl;

  ifstream calfile(Name);
  int linenum = 0;

  int location = -1;
  double value = 0;
 
  if(!calfile){
    cout << "Unable to open file " << Name << endl;
  }
  else{
    //read in cal file until end of file
    while(calfile){
      if(calfile && (isdigit(calfile.peek()))){
	linenum++;

	if(linenum < 257) {
	  
	  calfile >> location >> value;
	  
	  if(linenum < 17) clyc.square[location] = value;
	  else if(linenum < 33) { 
	    clyc.slope[location] = value;
	    cout<<clyc.slope[location]<<endl;
	  }
	  else if(linenum < 49) clyc.intercept[location] = value;
	  else if(linenum < 65) clyc.thresh[location] = value;
	  else if(linenum < 81) clyc.hithresh[location] = value;
	  else if(linenum < 97) {
	    clyc.dynode_toffset[location] = value;
	    cout<<clyc.dynode_toffset[location]<<endl;
	  }
	  else if(linenum < 113){
	    clyc.clyc_toffset[1][location] = value;
	    cout<<clyc.clyc_toffset[1][location]<<endl;
	  }
	  else if(linenum < 129){
	    clyc.clyc_toffset[2][location] = value;
	    cout<<clyc.clyc_toffset[2][location]<<endl;
	  }
	  else if(linenum < 145){
	    clyc.clyc_toffset[3][location] = value;
	    cout<<clyc.clyc_toffset[3][location]<<endl;
	  }
	  else if(linenum < 161){
	    clyc.clyc_toffset[4][location] = value;
	    cout<<clyc.clyc_toffset[4][location]<<endl;
	  }
	  else if(linenum < 177){
	    clyc.clyc_toffset[5][location] = value;
	    cout<<clyc.clyc_toffset[5][location]<<endl;
	  }
	  else if(linenum < 193){
	    clyc.clyc_toffset[6][location] = value;
	    cout<<clyc.clyc_toffset[6][location]<<endl;
	  }
	  else if(linenum < 209){
	    clyc.clyc_toffset[7][location] = value;
	    cout<<clyc.clyc_toffset[7][location]<<endl;
	  }
	  else if(linenum < 225){
	    clyc.clyc_toffset[8][location] = value;
	    cout<<clyc.clyc_toffset[8][location]<<endl;
	  }
	  else if(linenum < 241){
	    clyc.clyc_toffset[9][location] = value;
	    cout<<clyc.clyc_toffset[9][location]<<endl;
	  }
	  else if(linenum < 257){
	    clyc.clyc_toffset[10][location] = value;
	    cout<<clyc.clyc_toffset[10][location]<<endl;
	  }
	}
	if(linenum == 257){
	  calfile >> value;
	  clyc.lotime = value;
	}
	if(linenum == 258){
	  calfile >> value;
	  clyc.hitime = value;
	}

      } // line read
      else{
	//ignore line
	calfile.ignore(1000,'\n');
      }
    } // end while read

  }

  //set the offsets to the values for initial analysis
  // clyc.toffset[1] = 0;
  // clyc.toffset[2] = 80.5;
  // clyc.toffset[3] = 81.75;
  // clyc.toffset[4] = 75.5;
  // clyc.toffset[5] = 76.25;
  // clyc.toffset[6] = 82.75;
  // clyc.toffset[7] = 72;
  // clyc.toffset[8] = 75;
  // clyc.toffset[9] = -1.5;
  // clyc.toffset[10] = 80;

  for(int q=0; q<17; q++) {
    cout<<clyc.square[q]<<"  "<<clyc.slope[q]<<"  "<<clyc.intercept[q]<<endl;
  }
  calfile.close();

}

void betadecayvariables::ReadLaBr3(char *Name){

  cout<<"Reading labr3"<<endl;
  cout<<Name<<endl;

  ifstream calfile(Name);
  int linenum = 0;

  int location = -1;
  double value = 0;
 
  if(!calfile){
    cout << "Unable to open file " << Name << endl;
  }
  else{
    //read in cal file until end of file
    while(calfile){
      if(calfile && (isdigit(calfile.peek()))){
	linenum++;

	if(linenum < 257) {
	  
	  calfile >> location >> value;
	  
	  if(linenum < 17) labr3.square[location] = value;
	  else if(linenum < 33) { 
	    labr3.slope[location] = value;
	    cout<<labr3.slope[location]<<endl;
	  }
	  else if(linenum < 49) labr3.intercept[location] = value;
	  else if(linenum < 65) labr3.thresh[location] = value;
	  else if(linenum < 81) labr3.hithresh[location] = value;
	  else if(linenum < 97) {
	    labr3.dynode_toffset[location] = value;
	    cout<<labr3.dynode_toffset[location]<<endl;
	  }
	  else if(linenum < 113){
	    labr3.labr_toffset[1][location] = value;
	    cout<<labr3.labr_toffset[1][location]<<endl;
	  }
	  else if(linenum < 129){
	    labr3.labr_toffset[2][location] = value;
	    cout<<labr3.labr_toffset[2][location]<<endl;
	  }
	  else if(linenum < 145){
	    labr3.labr_toffset[3][location] = value;
	    cout<<labr3.labr_toffset[3][location]<<endl;
	  }
	  else if(linenum < 161){
	    labr3.labr_toffset[4][location] = value;
	    cout<<labr3.labr_toffset[4][location]<<endl;
	  }
	  else if(linenum < 177){
	    labr3.labr_toffset[5][location] = value;
	    cout<<labr3.labr_toffset[5][location]<<endl;
	  }
	  else if(linenum < 193){
	    labr3.labr_toffset[6][location] = value;
	    cout<<labr3.labr_toffset[6][location]<<endl;
	  }
	  else if(linenum < 209){
	    labr3.labr_toffset[7][location] = value;
	    cout<<labr3.labr_toffset[7][location]<<endl;
	  }
	  else if(linenum < 225){
	    labr3.labr_toffset[8][location] = value;
	    cout<<labr3.labr_toffset[8][location]<<endl;
	  }
	  else if(linenum < 241){
	    labr3.labr_toffset[9][location] = value;
	    cout<<labr3.labr_toffset[9][location]<<endl;
	  }
	  else if(linenum < 257){
	    labr3.labr_toffset[10][location] = value;
	    cout<<labr3.labr_toffset[10][location]<<endl;
	  }
	}
	if(linenum == 257){
	  calfile >> value;
	  labr3.lotime = value;
	}
	if(linenum == 258){
	  calfile >> value;
	  labr3.hitime = value;
	}

      } // line read
      else{
	//ignore line
	calfile.ignore(1000,'\n');
      }
    } // end while read

  }

  //set the offsets to the values for initial analysis
  // labr3.toffset[1] = 0;
  // labr3.toffset[2] = 80.5;
  // labr3.toffset[3] = 81.75;
  // labr3.toffset[4] = 75.5;
  // labr3.toffset[5] = 76.25;
  // labr3.toffset[6] = 82.75;
  // labr3.toffset[7] = 72;
  // labr3.toffset[8] = 75;
  // labr3.toffset[9] = -1.5;
  // labr3.toffset[10] = 80;

  for(int q=0; q<17; q++) {
    cout<<labr3.square[q]<<"  "<<labr3.slope[q]<<"  "<<labr3.intercept[q]<<endl;
  }
  calfile.close();

}

void betadecayvariables::ReadSega(char *Name){

  cout<<"Reading SeGA"<<endl;
  cout<<Name<<endl;

  ifstream calfile(Name);
  int linenum = 0;

  int location = -1;
  double value = 0;
 
  if(!calfile){
    cout << "Unable to open file " << Name << endl;
  }
  else{
    //read in cal file until end of file
    while(calfile){
      if(calfile && (isdigit(calfile.peek()))){
	linenum++;

	if(linenum < 81) {
	  
	  calfile >> location >> value;
	  
	  if(linenum < 17) sega.square[location] = value;
	  else if(linenum < 33) { 
	    sega.slope[location] = value;
	    cout<<sega.slope[location]<<endl;
	  }
	  else if(linenum < 49) sega.intercept[location] = value;
	  else if(linenum < 65) sega.thresh[location] = value;
	  else if(linenum < 81) sega.hithresh[location] = value;
	}
	if(linenum == 81){
	  calfile >> value;
	  sega.lotime = value;
	}
	if(linenum == 82){
	  calfile >> value;
	  sega.hitime = value;
	}

      } // line read
      else{
	//ignore line
	calfile.ignore(1000,'\n');
      }
    } // end while read

  }
  calfile.close();

}

void betadecayvariables::ReadPSPMT_TimeDev(char *Name){

  cout<<"Reading PSPMT Time Dev"<<endl;
  cout<<Name<<endl;

  ifstream calfile(Name);
  // int linenum = 0;

  // int location = -1;
  // double value = 0;
 
  if(!calfile){
    cout << "Unable to open file " << Name << endl;
  }
  else{
   //read in cal file until end of file
    while(calfile){

      int location;
      double value;
      
      calfile >> location >> value;
      
      pspmt.time_deviation[location] = value;

      cout<<"PSPMT TimeDev Anode: "<<location<<" TimeDev: "<<pspmt.time_deviation[location]<<endl;
      if(location == 256) {
	break;
      }
    }
  }
}

void betadecayvariables::ReadPSPMT(char *Name){

  cout<<"Reading PSPMT"<<endl;
  cout<<Name<<endl;

  ifstream calfile(Name);
  // int linenum = 0;

  // int location = -1;
  // double value = 0;
 
  if(!calfile){
    cout << "Unable to open file " << Name << endl;
  }
  else{
   //read in cal file until end of file
    while(calfile){

      int location;
      double value;
      
      calfile >> location >> value;
      
      pspmt.slope[location] = value;

      cout<<"PSPMT Anode: "<<location<<" gain: "<<value<<endl;
      if(location == 256) {
	break;
      }
    }
  }


  //ULD on anodes
  for(int eye=1; eye<257; eye++) {
    if(eye >=1 && eye <= 48) {
      pspmt.uld[eye] = 0.95*pow(2,15.0);
    }
    else if(eye >48 && eye <= 208) {
      pspmt.uld[eye] = 0.95*pow(2,16.0);
    }
    else if(eye >208 && eye <= 256) {
      pspmt.uld[eye] = 0.95*pow(2,15.0);
    }
    else {
      cout<<"Something is very wrong"<<endl;
    }
  }

  
      //  if(calfile && (isdigit(calfile.peek()))){
  // 	linenum++;

  // 	if(linenum < 81) {
	  
  // 	  calfile >> location >> value;
	  
  // 	  if(linenum < 17) pspmt.square[location] = value;
  // 	  else if(linenum < 33) { 
  // 	    pspmt.slope[location] = value;
  // 	    cout<<pspmt.slope[location]<<endl;
  // 	  }
  // 	  else if(linenum < 49) pspmt.intercept[location] = value;
  // 	  else if(linenum < 65) pspmt.thresh[location] = value;
  // 	  else if(linenum < 81) pspmt.hithresh[location] = value;
  // 	}
  // 	if(linenum == 81){
  // 	  calfile >> value;
  // 	  pspmt.lotime = value;
  // 	}
  // 	if(linenum == 82){
  // 	  calfile >> value;
  // 	  pspmt.hitime = value;
  // 	}

  //     } // line read
  //     else{
  // 	//ignore line
  // 	calfile.ignore(1000,'\n');
  //     }
  //   } // end while read

  // }
   calfile.close();

}


void betadecayvariables::ReadOther(char *Name){
 
  //ifstream calfile("cal/OtherInit.txt");
  ifstream calfile(Name);
  int linenum = 0;

  double value = 0;
 
  if(!calfile){
    //cout << "Unable to open file cal/OtherInit.txt" << endl;
    cout << "Unable to open file " << Name << endl;
  }
  else{
    //read in cal file until end of file
    while(calfile){
      if(calfile && (isdigit(calfile.peek()))){
	linenum++;

	calfile >> value;

	if(linenum == 1) clock.scale = value;
	if(linenum == 2) clock.calib = value;
	if(linenum == 3) clock.max = value;
	if(linenum == 4) corr.minimplant = value;
	if(linenum == 5) clockisomer.max = value;
	if(linenum == 6) clockisomer.calib = value;
	if(linenum == 7) pin01.slope = value;
	if(linenum == 8) pin01.intercept = value;
	if(linenum == 9) pin01.thresh = value;
	if(linenum == 10) pin01.ithresh = value;
	if(linenum == 11) pin01.iuld = value;
	if(linenum == 12) pin02.slope = value;
	if(linenum == 13) pin02.intercept = value;
	if(linenum == 14) pin02.thresh = value;
	if(linenum == 15) pin03.slope = value;
	if(linenum == 16) pin03.intercept = value;
	if(linenum == 17) pin03.thresh = value;
	if(linenum == 18) tac.i2ncorr = value;
	if(linenum == 19) tac.i2ncorrpos = value;
	if(linenum == 20) tac.pin01corr1 = value;
	if(linenum == 21) tac.pin01corr2 = value;
	if(linenum == 22) tac.i2scorr = value;
	if(linenum == 23) tac.i2scorr_offset = value;
	if(linenum == 24) tac.pin01corr = value;
	if(linenum == 25) tac.pin01corr_offset = value;
	if(linenum == 26) tac.pin02corr = value;
	if(linenum == 27) tac.pin02corr_offset = value;
	if(linenum == 28) i2n.slope = value;
	if(linenum == 29) i2n.intercept = value;
	if(linenum == 30) i2n.thresh = value;
	if(linenum == 31) i2s.slope = value;
	if(linenum == 32) i2s.intercept = value;
	if(linenum == 33) i2s.thresh = value;


      } // line read
      else{
	//ignore line
	calfile.ignore(1000,'\n');
      }
    } // end while read

  }
  calfile.close();
}
