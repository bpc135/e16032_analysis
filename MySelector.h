//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Thu Jun 28 17:48:38 2018 by ROOT version 6.10/02
// from TChain data/
//////////////////////////////////////////////////////////

#ifndef MySelector_h
#define MySelector_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TSelector.h>
#include <TTreeReader.h>
#include <TTreeReaderValue.h>
#include <TTreeReaderArray.h>
#include <TMath.h>
#include "Parameters-ddas.h"

// Headers needed by this particular selector


class MySelector : public TSelector {
public :
   TTreeReader     fReader;  //!the tree reader
   TTree          *fChain = 0;   //!pointer to the analyzed TTree or TChain
   int             counter;
   
   // Readers to access the data (delete the ones you do not need).
   TTreeReaderValue<unsigned int> fUniqueID = {fReader, "fUniqueID"};
   TTreeReaderValue<unsigned int> fBits = {fReader, "fBits"};
   TTreeReaderValue<unsigned int> clock_fUniqueID = {fReader, "clock.fUniqueID"};
   TTreeReaderValue<unsigned int> clock_fBits = {fReader, "clock.fBits"};
   TTreeReaderValue<Double_t> clock_fast = {fReader, "clock.fast"};
   TTreeReaderValue<Double_t> clock_slow = {fReader, "clock.slow"};
   TTreeReaderValue<Double_t> clock_cfd = {fReader, "clock.cfd"};
   TTreeReaderValue<Double_t> clock_current = {fReader, "clock.current"};
   TTreeReaderValue<Double_t> clock_initial = {fReader, "clock.initial"};
   TTreeReaderValue<unsigned int> tac_fUniqueID = {fReader, "tac.fUniqueID"};
   TTreeReaderValue<unsigned int> tac_fBits = {fReader, "tac.fBits"};
   TTreeReaderValue<Int_t> tac_pin01i2nE = {fReader, "tac.pin01i2nE"};
   TTreeReaderValue<Int_t> tac_pin01i2nA = {fReader, "tac.pin01i2nA"};
   TTreeReaderValue<Int_t> tac_pin01i2nT = {fReader, "tac.pin01i2nT"};
   TTreeReaderValue<Double_t> tac_pin01i2nTDC = {fReader, "tac.pin01i2nTDC"};
   TTreeReaderValue<Int_t> tac_pin01i2sE = {fReader, "tac.pin01i2sE"};
   TTreeReaderValue<Int_t> tac_pin01i2sA = {fReader, "tac.pin01i2sA"};
   TTreeReaderValue<Int_t> tac_pin01i2sT = {fReader, "tac.pin01i2sT"};
   TTreeReaderValue<Double_t> tac_pin01i2sTDC = {fReader, "tac.pin01i2sTDC"};
   TTreeReaderValue<Int_t> tac_pin02i2nE = {fReader, "tac.pin02i2nE"};
   TTreeReaderValue<Int_t> tac_pin02i2nA = {fReader, "tac.pin02i2nA"};
   TTreeReaderValue<Int_t> tac_pin02i2nT = {fReader, "tac.pin02i2nT"};
   TTreeReaderValue<Double_t> tac_pin02i2nTDC = {fReader, "tac.pin02i2nTDC"};
   TTreeReaderValue<Int_t> tac_pin02i2sE = {fReader, "tac.pin02i2sE"};
   TTreeReaderValue<Int_t> tac_pin02i2sA = {fReader, "tac.pin02i2sA"};
   TTreeReaderValue<Int_t> tac_pin02i2sT = {fReader, "tac.pin02i2sT"};
   TTreeReaderValue<Double_t> tac_pin02i2sTDC = {fReader, "tac.pin02i2sTDC"};
   TTreeReaderValue<Int_t> tac_i2ni2sE = {fReader, "tac.i2ni2sE"};
   TTreeReaderValue<Int_t> tac_i2ni2sA = {fReader, "tac.i2ni2sA"};
   TTreeReaderValue<Int_t> tac_i2ni2sT = {fReader, "tac.i2ni2sT"};
   TTreeReaderValue<Double_t> tac_i2ni2sTDC = {fReader, "tac.i2ni2sTDC"};
   TTreeReaderValue<Int_t> tac_pin01rfE = {fReader, "tac.pin01rfE"};
   TTreeReaderValue<Int_t> tac_pin01rfA = {fReader, "tac.pin01rfA"};
   TTreeReaderValue<Int_t> tac_pin01rfT = {fReader, "tac.pin01rfT"};
   TTreeReaderValue<Double_t> tac_pin01rfTDC = {fReader, "tac.pin01rfTDC"};
   TTreeReaderValue<Int_t> tac_i2pos = {fReader, "tac.i2pos"};
   TTreeReaderValue<Int_t> tac_i2ncorr = {fReader, "tac.i2ncorr"};
   TTreeReaderValue<Int_t> tac_i2scorr = {fReader, "tac.i2scorr"};
   TTreeReaderValue<unsigned int> pin01_fUniqueID = {fReader, "pin01.fUniqueID"};
   TTreeReaderValue<unsigned int> pin01_fBits = {fReader, "pin01.fBits"};
   TTreeReaderValue<Int_t> pin01_energy = {fReader, "pin01.energy"};
   TTreeReaderValue<Int_t> pin01_ecorr = {fReader, "pin01.ecorr"};
   TTreeReaderValue<Double_t> pin01_time = {fReader, "pin01.time"};
   TTreeReaderValue<Int_t> pin01_ecal = {fReader, "pin01.ecal"};
   TTreeReaderValue<Int_t> pin01_eventtdc = {fReader, "pin01.eventtdc"};
   TTreeReaderValue<unsigned int> pin02_fUniqueID = {fReader, "pin02.fUniqueID"};
   TTreeReaderValue<unsigned int> pin02_fBits = {fReader, "pin02.fBits"};
   TTreeReaderValue<Int_t> pin02_energy = {fReader, "pin02.energy"};
   TTreeReaderValue<Int_t> pin02_ecorr = {fReader, "pin02.ecorr"};
   TTreeReaderValue<Double_t> pin02_time = {fReader, "pin02.time"};
   TTreeReaderValue<Int_t> pin02_ecal = {fReader, "pin02.ecal"};
   TTreeReaderValue<Int_t> pin02_eventtdc = {fReader, "pin02.eventtdc"};
   TTreeReaderValue<unsigned int> pin03_fUniqueID = {fReader, "pin03.fUniqueID"};
   TTreeReaderValue<unsigned int> pin03_fBits = {fReader, "pin03.fBits"};
   TTreeReaderValue<Int_t> pin03_energy = {fReader, "pin03.energy"};
   TTreeReaderValue<Int_t> pin03_ecorr = {fReader, "pin03.ecorr"};
   TTreeReaderValue<Double_t> pin03_time = {fReader, "pin03.time"};
   TTreeReaderValue<Int_t> pin03_ecal = {fReader, "pin03.ecal"};
   TTreeReaderValue<Int_t> pin03_eventtdc = {fReader, "pin03.eventtdc"};
   TTreeReaderValue<unsigned int> pin04_fUniqueID = {fReader, "pin04.fUniqueID"};
   TTreeReaderValue<unsigned int> pin04_fBits = {fReader, "pin04.fBits"};
   TTreeReaderValue<Int_t> pin04_energy = {fReader, "pin04.energy"};
   TTreeReaderValue<Int_t> pin04_ecorr = {fReader, "pin04.ecorr"};
   TTreeReaderValue<Double_t> pin04_time = {fReader, "pin04.time"};
   TTreeReaderValue<Int_t> pin04_ecal = {fReader, "pin04.ecal"};
   TTreeReaderValue<Int_t> pin04_eventtdc = {fReader, "pin04.eventtdc"};
   TTreeReaderValue<unsigned int> pid_fUniqueID = {fReader, "pid.fUniqueID"};
   TTreeReaderValue<unsigned int> pid_fBits = {fReader, "pid.fBits"};
   TTreeReaderValue<Int_t> pid_de1 = {fReader, "pid.de1"};
   TTreeReaderValue<Int_t> pid_de2 = {fReader, "pid.de2"};
   TTreeReaderValue<Int_t> pid_de3 = {fReader, "pid.de3"};
   TTreeReaderValue<Int_t> pid_de4 = {fReader, "pid.de4"};
   TTreeReaderValue<Int_t> pid_rftof = {fReader, "pid.rftof"};
   TTreeReaderValue<Int_t> pid_i2ntof = {fReader, "pid.i2ntof"};
   TTreeReaderValue<Int_t> pid_i2stof = {fReader, "pid.i2stof"};
   TTreeReaderValue<Int_t> pid_xfptof = {fReader, "pid.xfptof"};
   TTreeReaderValue<Int_t> pid_isum = {fReader, "pid.isum"};
   TTreeReaderValue<Int_t> pid_imax = {fReader, "pid.imax"};
   TTreeReaderValue<unsigned int> corr_fUniqueID = {fReader, "corr.fUniqueID"};
   TTreeReaderValue<unsigned int> corr_fBits = {fReader, "corr.fBits"};
   TTreeReaderValue<Double_t> corr_dtimplant = {fReader, "corr.dtimplant"};
   TTreeReaderValue<Double_t> corr_itime = {fReader, "corr.itime"};
   TTreeReaderValue<Int_t> corr_ide1 = {fReader, "corr.ide1"};
   TTreeReaderValue<Int_t> corr_ide2 = {fReader, "corr.ide2"};
   TTreeReaderValue<Int_t> corr_ide3 = {fReader, "corr.ide3"};
   TTreeReaderValue<Float_t> corr_iisum = {fReader, "corr.iisum"};
   TTreeReaderValue<Float_t> corr_iimax = {fReader, "corr.iimax"};
   TTreeReaderValue<Int_t> corr_itof = {fReader, "corr.itof"};
   TTreeReaderValue<Double_t> corr_iasum = {fReader, "corr.iasum"};
   TTreeReaderValue<Double_t> corr_dyecal = {fReader, "corr.dyecal"};
   TTreeReaderValue<Double_t> corr_dyamp = {fReader, "corr.dyamp"};
   TTreeReaderValue<Double_t> corr_idyecal = {fReader, "corr.idyecal"};
   TTreeReaderValue<Double_t> corr_idyamp = {fReader, "corr.idyamp"};
   TTreeReaderValue<Double_t> corr_didyecal = {fReader, "corr.didyecal"};
   TTreeReaderValue<Double_t> corr_didyamp = {fReader, "corr.didyamp"};
   TTreeReaderValue<Double_t> corr_gtimecal = {fReader, "corr.gtimecal"};
   TTreeReaderValue<Double_t> corr_gtime = {fReader, "corr.gtime"};
   TTreeReaderValue<Int_t> corr_gde1 = {fReader, "corr.gde1"};
   TTreeReaderValue<Int_t> corr_gde2 = {fReader, "corr.gde2"};
   TTreeReaderValue<Int_t> corr_gde3 = {fReader, "corr.gde3"};
   TTreeReaderValue<Float_t> corr_gisum = {fReader, "corr.gisum"};
   TTreeReaderValue<Float_t> corr_gimax = {fReader, "corr.gimax"};
   TTreeReaderValue<Int_t> corr_gtof = {fReader, "corr.gtof"};
   TTreeReaderValue<Double_t> corr_ltimecal = {fReader, "corr.ltimecal"};
   TTreeReaderValue<Double_t> corr_ltime = {fReader, "corr.ltime"};
   TTreeReaderValue<Int_t> corr_lde1 = {fReader, "corr.lde1"};
   TTreeReaderValue<Int_t> corr_lde2 = {fReader, "corr.lde2"};
   TTreeReaderValue<Int_t> corr_lde3 = {fReader, "corr.lde3"};
   TTreeReaderValue<Float_t> corr_lisum = {fReader, "corr.lisum"};
   TTreeReaderValue<Float_t> corr_limax = {fReader, "corr.limax"};
   TTreeReaderValue<Int_t> corr_ltof = {fReader, "corr.ltof"};
   TTreeReaderValue<Double_t> corr_ctimecal = {fReader, "corr.ctimecal"};
   TTreeReaderValue<Double_t> corr_ctime = {fReader, "corr.ctime"};
   TTreeReaderValue<Int_t> corr_cde1 = {fReader, "corr.cde1"};
   TTreeReaderValue<Int_t> corr_cde2 = {fReader, "corr.cde2"};
   TTreeReaderValue<Int_t> corr_cde3 = {fReader, "corr.cde3"};
   TTreeReaderValue<Float_t> corr_cisum = {fReader, "corr.cisum"};
   TTreeReaderValue<Float_t> corr_cimax = {fReader, "corr.cimax"};
   TTreeReaderValue<Int_t> corr_ctof = {fReader, "corr.ctof"};
   TTreeReaderValue<Double_t> corr_dtimecal = {fReader, "corr.dtimecal"};
   TTreeReaderValue<Double_t> corr_dtime = {fReader, "corr.dtime"};
   TTreeReaderValue<Double_t> corr_xPos = {fReader, "corr.xPos"};
   TTreeReaderValue<Double_t> corr_yPos = {fReader, "corr.yPos"};
   TTreeReaderValue<Int_t> corr_implantX = {fReader, "corr.implantX"};
   TTreeReaderValue<Int_t> corr_implantY = {fReader, "corr.implantY"};
   TTreeReaderValue<Int_t> corr_decayX = {fReader, "corr.decayX"};
   TTreeReaderValue<Int_t> corr_decayY = {fReader, "corr.decayY"};
   TTreeReaderValue<Int_t> corr_dde1 = {fReader, "corr.dde1"};
   TTreeReaderValue<Int_t> corr_dde2 = {fReader, "corr.dde2"};
   TTreeReaderValue<Int_t> corr_dde3 = {fReader, "corr.dde3"};
   TTreeReaderValue<Float_t> corr_disum = {fReader, "corr.disum"};
   TTreeReaderValue<Float_t> corr_dimax = {fReader, "corr.dimax"};
   TTreeReaderValue<Int_t> corr_dtof = {fReader, "corr.dtof"};
   TTreeReaderValue<Int_t> corr_flag = {fReader, "corr.flag"};
   TTreeReaderValue<Int_t> corr_ImpDecayCorrFlag = {fReader, "corr.ImpDecayCorrFlag"};
   TTreeReaderValue<Int_t> corr_mult = {fReader, "corr.mult"};
   TTreeReaderValue<Int_t> corr_dmult = {fReader, "corr.dmult"};
   TTreeReaderValue<Float_t> corr_dmax = {fReader, "corr.dmax"};
   TTreeReaderValue<Float_t> corr_dsum = {fReader, "corr.dsum"};
   TTreeReaderValue<Int_t> corr_dnumcorr = {fReader, "corr.dnumcorr"};
   TTreeReaderValue<Double_t> corr_gdtimecal = {fReader, "corr.gdtimecal"};
   TTreeReaderValue<Double_t> corr_gdtime = {fReader, "corr.gdtime"};
   TTreeReaderValue<Int_t> corr_gdde1 = {fReader, "corr.gdde1"};
   TTreeReaderValue<Int_t> corr_gdde2 = {fReader, "corr.gdde2"};
   TTreeReaderValue<Int_t> corr_gdde3 = {fReader, "corr.gdde3"};
   TTreeReaderValue<Int_t> corr_gdisum = {fReader, "corr.gdisum"};
   TTreeReaderValue<Int_t> corr_gdimax = {fReader, "corr.gdimax"};
   TTreeReaderValue<Int_t> corr_gdtof = {fReader, "corr.gdtof"};
   TTreeReaderArray<Float_t> corr_gdfronthiecal = {fReader, "corr.gdfronthiecal[17]"};
   TTreeReaderArray<Float_t> corr_gdbackhiecal = {fReader, "corr.gdbackhiecal[17]"};
   TTreeReaderValue<unsigned int> sega_fUniqueID = {fReader, "sega.fUniqueID"};
   TTreeReaderValue<unsigned int> sega_fBits = {fReader, "sega.fBits"};
   TTreeReaderArray<Double_t> sega_timecfd = {fReader, "sega.timecfd[17]"};
   TTreeReaderArray<Double_t> sega_timelow = {fReader, "sega.timelow[17]"};
   TTreeReaderArray<Double_t> sega_timehigh = {fReader, "sega.timehigh[17]"};
   TTreeReaderArray<Double_t> sega_time = {fReader, "sega.time[17]"};
   TTreeReaderArray<Double_t> sega_dtdc = {fReader, "sega.dtdc[17]"};
   TTreeReaderArray<Double_t> sega_itdc = {fReader, "sega.itdc[17]"};
   TTreeReaderArray<Double_t> sega_eventtdc = {fReader, "sega.eventtdc[17]"};
   TTreeReaderArray<Double_t> sega_energy = {fReader, "sega.energy[17]"};
   TTreeReaderArray<Double_t> sega_goodenergy = {fReader, "sega.goodenergy[17]"};
   TTreeReaderArray<Double_t> sega_ecal = {fReader, "sega.ecal[17]"};
   TTreeReaderValue<unsigned int> segatotal_fUniqueID = {fReader, "segatotal.fUniqueID"};
   TTreeReaderValue<unsigned int> segatotal_fBits = {fReader, "segatotal.fBits"};
   TTreeReaderValue<Int_t> segatotal_mult = {fReader, "segatotal.mult"};
   TTreeReaderValue<Int_t> segatotal_total = {fReader, "segatotal.total"};
   TTreeReaderValue<Int_t> segatotal_tot11 = {fReader, "segatotal.tot11"};
   TTreeReaderValue<Int_t> segatotal_xmatrix = {fReader, "segatotal.xmatrix"};
   TTreeReaderValue<Int_t> segatotal_ymatrix = {fReader, "segatotal.ymatrix"};
   TTreeReaderValue<Int_t> segatotal_xmat11 = {fReader, "segatotal.xmat11"};
   TTreeReaderValue<Int_t> segatotal_ymat11 = {fReader, "segatotal.ymat11"};
   TTreeReaderValue<unsigned int> labr3_fUniqueID = {fReader, "labr3.fUniqueID"};
   TTreeReaderValue<unsigned int> labr3_fBits = {fReader, "labr3.fBits"};
   TTreeReaderArray<Double_t> labr3_timecfd = {fReader, "labr3.timecfd[17]"};
   TTreeReaderArray<Double_t> labr3_timelow = {fReader, "labr3.timelow[17]"};
   TTreeReaderArray<Double_t> labr3_timehigh = {fReader, "labr3.timehigh[17]"};
   TTreeReaderArray<Double_t> labr3_dtdc = {fReader, "labr3.dtdc[17]"};
   TTreeReaderArray<Double_t> labr3_itdc = {fReader, "labr3.itdc[17]"};
   TTreeReaderArray<Double_t> labr3_eventtdc = {fReader, "labr3.eventtdc[17]"};
   TTreeReaderArray<Double_t> labr3_energy = {fReader, "labr3.energy[17]"};
   TTreeReaderArray<Double_t> labr3_goodenergy = {fReader, "labr3.goodenergy[17]"};
   TTreeReaderArray<Double_t> labr3_ecal = {fReader, "labr3.ecal[17]"};
   TTreeReaderArray<Double_t> labr3_amp = {fReader, "labr3.amp[17]"};
   TTreeReaderArray<Double_t> labr3_ampcal = {fReader, "labr3.ampcal[17]"};
   TTreeReaderArray<Double_t> labr3_area = {fReader, "labr3.area[17]"};
   TTreeReaderArray<Double_t> labr3_areacal = {fReader, "labr3.areacal[17]"};
   TTreeReaderValue<Int_t> labr3_mult = {fReader, "labr3.mult"};
   TTreeReaderArray<Double_t> labr3_time = {fReader, "labr3.time[17]"};
   TTreeReaderArray<Double_t> labr3_tdiff = {fReader, "labr3.tdiff[17][17]"};
   TTreeReaderArray<Double_t> labr3_dynode_tdiff = {fReader, "labr3.dynode_tdiff[17]"};
   TTreeReaderArray<Double_t> labr3_timecfd_filt = {fReader, "labr3.timecfd_filt[17]"};
   TTreeReaderArray<Double_t> labr3_cfdtrigpt_filt = {fReader, "labr3.cfdtrigpt_filt[17]"};
   TTreeReaderValue<unsigned int> clyc_fUniqueID = {fReader, "clyc.fUniqueID"};
   TTreeReaderValue<unsigned int> clyc_fBits = {fReader, "clyc.fBits"};
   TTreeReaderArray<Double_t> clyc_timecfd = {fReader, "clyc.timecfd[17]"};
   TTreeReaderArray<Double_t> clyc_timelow = {fReader, "clyc.timelow[17]"};
   TTreeReaderArray<Double_t> clyc_timehigh = {fReader, "clyc.timehigh[17]"};
   TTreeReaderArray<Double_t> clyc_dtdc = {fReader, "clyc.dtdc[17]"};
   TTreeReaderArray<Double_t> clyc_itdc = {fReader, "clyc.itdc[17]"};
   TTreeReaderArray<Double_t> clyc_eventtdc = {fReader, "clyc.eventtdc[17]"};
   TTreeReaderArray<Double_t> clyc_energy = {fReader, "clyc.energy[17]"};
   TTreeReaderArray<Double_t> clyc_goodenergy = {fReader, "clyc.goodenergy[17]"};
   TTreeReaderArray<Double_t> clyc_ecal = {fReader, "clyc.ecal[17]"};
   TTreeReaderArray<Double_t> clyc_amp = {fReader, "clyc.amp[17]"};
   TTreeReaderArray<Double_t> clyc_ampcal = {fReader, "clyc.ampcal[17]"};
   TTreeReaderArray<Double_t> clyc_area = {fReader, "clyc.area[17]"};
   TTreeReaderArray<Double_t> clyc_areacal = {fReader, "clyc.areacal[17]"};
   TTreeReaderArray<Int_t> clyc_shortsum = {fReader, "clyc.shortsum[17]"};
   TTreeReaderArray<Int_t> clyc_longsum = {fReader, "clyc.longsum[17]"};
   TTreeReaderArray<Int_t> clyc_fullsum = {fReader, "clyc.fullsum[17]"};
   TTreeReaderValue<Int_t> clyc_mult = {fReader, "clyc.mult"};
   TTreeReaderArray<Double_t> clyc_time = {fReader, "clyc.time[17]"};
   TTreeReaderArray<Double_t> clyc_tdiff = {fReader, "clyc.tdiff[17][17]"};
   TTreeReaderArray<Double_t> clyc_dynode_tdiff = {fReader, "clyc.dynode_tdiff[17]"};
   TTreeReaderArray<Double_t> clyc_timecfd_filt = {fReader, "clyc.timecfd_filt[17]"};
   TTreeReaderArray<Double_t> clyc_cfdtrigpt_filt = {fReader, "clyc.cfdtrigpt_filt[17]"};
   TTreeReaderValue<unsigned int> i2_fUniqueID = {fReader, "i2.fUniqueID"};
   TTreeReaderValue<unsigned int> i2_fBits = {fReader, "i2.fBits"};
   TTreeReaderValue<Int_t> i2_i2pos = {fReader, "i2.i2pos"};
   TTreeReaderValue<Int_t> i2_i2postdc = {fReader, "i2.i2postdc"};
   TTreeReaderValue<Int_t> i2_i2poscorr = {fReader, "i2.i2poscorr"};
   TTreeReaderValue<Int_t> i2_i2nenergy = {fReader, "i2.i2nenergy"};
   TTreeReaderValue<Int_t> i2_i2necal = {fReader, "i2.i2necal"};
   TTreeReaderValue<Double_t> i2_i2ntime = {fReader, "i2.i2ntime"};
   TTreeReaderValue<Double_t> i2_i2ntof = {fReader, "i2.i2ntof"};
   TTreeReaderValue<Int_t> i2_i2ntdc = {fReader, "i2.i2ntdc"};
   TTreeReaderValue<Int_t> i2_i2ncorr = {fReader, "i2.i2ncorr"};
   TTreeReaderValue<Int_t> i2_i2senergy = {fReader, "i2.i2senergy"};
   TTreeReaderValue<Int_t> i2_i2secal = {fReader, "i2.i2secal"};
   TTreeReaderValue<Double_t> i2_i2stime = {fReader, "i2.i2stime"};
   TTreeReaderValue<Double_t> i2_i2stof = {fReader, "i2.i2stof"};
   TTreeReaderValue<Int_t> i2_i2stdc = {fReader, "i2.i2stdc"};
   TTreeReaderValue<Int_t> i2_i2scorr = {fReader, "i2.i2scorr"};
   TTreeReaderValue<unsigned int> pspmt_fUniqueID = {fReader, "pspmt.fUniqueID"};
   TTreeReaderValue<unsigned int> pspmt_fBits = {fReader, "pspmt.fBits"};
   TTreeReaderValue<Double_t> pspmt_dyenergy = {fReader, "pspmt.dyenergy"};
   TTreeReaderValue<Double_t> pspmt_dyecal = {fReader, "pspmt.dyecal"};
   TTreeReaderValue<Double_t> pspmt_dyamp = {fReader, "pspmt.dyamp"};
   TTreeReaderValue<Double_t> pspmt_dyampcal = {fReader, "pspmt.dyampcal"};
   TTreeReaderValue<Double_t> pspmt_dyarea = {fReader, "pspmt.dyarea"};
   TTreeReaderValue<Double_t> pspmt_dyareacal = {fReader, "pspmt.dyareacal"};
   TTreeReaderValue<Int_t> pspmt_dyoverflow = {fReader, "pspmt.dyoverflow"};
   TTreeReaderValue<Double_t> pspmt_dytime = {fReader, "pspmt.dytime"};
   TTreeReaderValue<Double_t> pspmt_dytimemin = {fReader, "pspmt.dytimemin"};
   TTreeReaderValue<Double_t> pspmt_dytdiff = {fReader, "pspmt.dytdiff"};
   TTreeReaderValue<Double_t> pspmt_dytdiffmin = {fReader, "pspmt.dytdiffmin"};
   TTreeReaderValue<Double_t> pspmt_dyeventtdc = {fReader, "pspmt.dyeventtdc"};
   TTreeReaderValue<Double_t> pspmt_dyE1_double = {fReader, "pspmt.dyE1_double"};
   TTreeReaderValue<Double_t> pspmt_dyT1_double = {fReader, "pspmt.dyT1_double"};
   TTreeReaderValue<Double_t> pspmt_dyE1_steepness_double = {fReader, "pspmt.dyE1_steepness_double"};
   TTreeReaderValue<Double_t> pspmt_dyE1_decayTime_double = {fReader, "pspmt.dyE1_decayTime_double"};
   TTreeReaderValue<Double_t> pspmt_dyE2_double = {fReader, "pspmt.dyE2_double"};
   TTreeReaderValue<Double_t> pspmt_dyT2_double = {fReader, "pspmt.dyT2_double"};
   TTreeReaderValue<Double_t> pspmt_dyE2_steepness_double = {fReader, "pspmt.dyE2_steepness_double"};
   TTreeReaderValue<Double_t> pspmt_dyE2_decayTime_double = {fReader, "pspmt.dyE2_decayTime_double"};
   TTreeReaderValue<Double_t> pspmt_dychisq_double = {fReader, "pspmt.dychisq_double"};
   TTreeReaderValue<Double_t> pspmt_dyoffset_double = {fReader, "pspmt.dyoffset_double"};
   TTreeReaderValue<Double_t> pspmt_dytdiffE1E2 = {fReader, "pspmt.dytdiffE1E2"};
   TTreeReaderValue<Double_t> pspmt_dyE1_single = {fReader, "pspmt.dyE1_single"};
   TTreeReaderValue<Double_t> pspmt_dyT1_single = {fReader, "pspmt.dyT1_single"};
   TTreeReaderValue<Double_t> pspmt_dychisq_single = {fReader, "pspmt.dychisq_single"};
   TTreeReaderValue<Double_t> pspmt_dyoffset_single = {fReader, "pspmt.dyoffset_single"};
   TTreeReaderValue<Double_t> pspmt_dyE1_steepness_single = {fReader, "pspmt.dyE1_steepness_single"};
   TTreeReaderValue<Double_t> pspmt_dyE1_decayTime_single = {fReader, "pspmt.dyE1_decayTime_single"};
   TTreeReaderValue<Double_t> pspmt_testsig = {fReader, "pspmt.testsig"};
   TTreeReaderValue<Double_t> pspmt_testamp = {fReader, "pspmt.testamp"};
   TTreeReaderValue<Double_t> pspmt_testarea = {fReader, "pspmt.testarea"};
   TTreeReaderValue<Double_t> pspmt_testtime = {fReader, "pspmt.testtime"};
   TTreeReaderValue<Double_t> pspmt_testoverflow = {fReader, "pspmt.testoverflow"};
   TTreeReaderArray<Int_t> pspmt_ahit = {fReader, "pspmt.ahit[257]"};
   TTreeReaderArray<Double_t> pspmt_aenergy = {fReader, "pspmt.aenergy[257]"};
   TTreeReaderArray<Double_t> pspmt_aecal = {fReader, "pspmt.aecal[257]"};
   TTreeReaderValue<Double_t> pspmt_asum = {fReader, "pspmt.asum"};
   TTreeReaderArray<Double_t> pspmt_atime = {fReader, "pspmt.atime[257]"};
   TTreeReaderValue<Double_t> pspmt_amax = {fReader, "pspmt.amax"};
   TTreeReaderValue<Double_t> pspmt_amin = {fReader, "pspmt.amin"};
   TTreeReaderValue<Double_t> pspmt_amaxcal = {fReader, "pspmt.amaxcal"};
   TTreeReaderValue<Double_t> pspmt_amincal = {fReader, "pspmt.amincal"};
   TTreeReaderArray<Double_t> pspmt_atdiff = {fReader, "pspmt.atdiff[257]"};
   TTreeReaderArray<Double_t> pspmt_atdiffmin = {fReader, "pspmt.atdiffmin[257]"};
   TTreeReaderArray<Int_t> pspmt_aoverflow = {fReader, "pspmt.aoverflow[257]"};
   TTreeReaderValue<Int_t> pspmt_amult = {fReader, "pspmt.amult"};
   TTreeReaderValue<Int_t> pspmt_amult_raw = {fReader, "pspmt.amult_raw"};
   TTreeReaderValue<Int_t> pspmt_amaxx = {fReader, "pspmt.amaxx"};
   TTreeReaderValue<Int_t> pspmt_amaxy = {fReader, "pspmt.amaxy"};
   TTreeReaderValue<Int_t> pspmt_aminx = {fReader, "pspmt.aminx"};
   TTreeReaderValue<Int_t> pspmt_aminy = {fReader, "pspmt.aminy"};
   TTreeReaderValue<Double_t> pspmt_amaxtime = {fReader, "pspmt.amaxtime"};
   TTreeReaderValue<Double_t> pspmt_amintime = {fReader, "pspmt.amintime"};
   TTreeReaderValue<Double_t> pspmt_posxEcent = {fReader, "pspmt.posxEcent"};
   TTreeReaderValue<Double_t> pspmt_posyEcent = {fReader, "pspmt.posyEcent"};
   TTreeReaderValue<Double_t> pspmt_posxEcent50 = {fReader, "pspmt.posxEcent50"};
   TTreeReaderValue<Double_t> pspmt_posyEcent50 = {fReader, "pspmt.posyEcent50"};
   TTreeReaderValue<Double_t> pspmt_amaxcent = {fReader, "pspmt.amaxcent"};
   TTreeReaderValue<Double_t> pspmt_asumcent = {fReader, "pspmt.asumcent"};
   TTreeReaderValue<Double_t> pspmt_asumcent50 = {fReader, "pspmt.asumcent50"};
   TTreeReaderArray<Double_t> pspmt_aamp = {fReader, "pspmt.aamp[257]"};
   TTreeReaderArray<Double_t> pspmt_aampcal = {fReader, "pspmt.aampcal[257]"};
   TTreeReaderValue<Double_t> pspmt_aampsum = {fReader, "pspmt.aampsum"};
   TTreeReaderValue<Int_t> pspmt_aampmax = {fReader, "pspmt.aampmax"};
   TTreeReaderValue<Double_t> pspmt_aampmaxcal = {fReader, "pspmt.aampmaxcal"};
   TTreeReaderValue<Int_t> pspmt_aampmaxx = {fReader, "pspmt.aampmaxx"};
   TTreeReaderValue<Int_t> pspmt_aampmaxy = {fReader, "pspmt.aampmaxy"};
   TTreeReaderValue<Double_t> pspmt_aampmaxtime = {fReader, "pspmt.aampmaxtime"};
   TTreeReaderValue<Double_t> pspmt_posxampcent = {fReader, "pspmt.posxampcent"};
   TTreeReaderValue<Double_t> pspmt_posyampcent = {fReader, "pspmt.posyampcent"};
   TTreeReaderValue<Double_t> pspmt_posxampcent50 = {fReader, "pspmt.posxampcent50"};
   TTreeReaderValue<Double_t> pspmt_posyampcent50 = {fReader, "pspmt.posyampcent50"};
   TTreeReaderValue<Double_t> pspmt_aampmaxcent = {fReader, "pspmt.aampmaxcent"};
   TTreeReaderValue<Double_t> pspmt_aampmaxcentcal = {fReader, "pspmt.aampmaxcentcal"};
   TTreeReaderValue<Double_t> pspmt_aampsumcent = {fReader, "pspmt.aampsumcent"};
   TTreeReaderValue<Double_t> pspmt_aampsumcent50 = {fReader, "pspmt.aampsumcent50"};
   TTreeReaderValue<Int_t> pspmt_aoverflowcount = {fReader, "pspmt.aoverflowcount"};
   TTreeReaderArray<Double_t> pspmt_aarea = {fReader, "pspmt.aarea[257]"};
   TTreeReaderArray<Double_t> pspmt_aareacal = {fReader, "pspmt.aareacal[257]"};
   TTreeReaderValue<Double_t> pspmt_aareasum = {fReader, "pspmt.aareasum"};
   TTreeReaderValue<Int_t> pspmt_aareamax = {fReader, "pspmt.aareamax"};
   TTreeReaderValue<Int_t> pspmt_aareamaxx = {fReader, "pspmt.aareamaxx"};
   TTreeReaderValue<Int_t> pspmt_aareamaxy = {fReader, "pspmt.aareamaxy"};
   TTreeReaderValue<Double_t> pspmt_aareamaxtime = {fReader, "pspmt.aareamaxtime"};
   TTreeReaderValue<Double_t> pspmt_posxareacent = {fReader, "pspmt.posxareacent"};
   TTreeReaderValue<Double_t> pspmt_posyareacent = {fReader, "pspmt.posyareacent"};
   TTreeReaderValue<Double_t> pspmt_posxareacent50 = {fReader, "pspmt.posxareacent50"};
   TTreeReaderValue<Double_t> pspmt_posyareacent50 = {fReader, "pspmt.posyareacent50"};
   TTreeReaderValue<Double_t> pspmt_aareamaxcent = {fReader, "pspmt.aareamaxcent"};
   TTreeReaderValue<Double_t> pspmt_aareamaxcentcal = {fReader, "pspmt.aareamaxcentcal"};
   TTreeReaderValue<Double_t> pspmt_aareasumcent = {fReader, "pspmt.aareasumcent"};
   TTreeReaderValue<Double_t> pspmt_aareasumcent50 = {fReader, "pspmt.aareasumcent50"};
   TTreeReaderArray<Double_t> pspmt_lowpoint = {fReader, "pspmt.lowpoint[257]"};
   TTreeReaderArray<Double_t> pspmt_baseline = {fReader, "pspmt.baseline[257]"};
   TTreeReaderArray<Double_t> pspmt_ratio = {fReader, "pspmt.ratio[257]"};
   TTreeReaderArray<Double_t> pspmt_aeventtdc = {fReader, "pspmt.aeventtdc[257]"};
   TTreeReaderArray<Int_t> pspmt_pixmult = {fReader, "pspmt.pixmult[257]"};
   TTreeReaderArray<Double_t> pspmt_loaenergy = {fReader, "pspmt.loaenergy[257]"};
   TTreeReaderArray<Double_t> pspmt_loaecal = {fReader, "pspmt.loaecal[257]"};
   TTreeReaderArray<Double_t> pspmt_loaamp = {fReader, "pspmt.loaamp[257]"};
   TTreeReaderArray<Double_t> pspmt_loaampcal = {fReader, "pspmt.loaampcal[257]"};
   TTreeReaderValue<Double_t> pspmt_loaampsum = {fReader, "pspmt.loaampsum"};
   TTreeReaderArray<Double_t> pspmt_loaarea = {fReader, "pspmt.loaarea[257]"};
   TTreeReaderArray<Double_t> pspmt_loaareacal = {fReader, "pspmt.loaareacal[257]"};
   TTreeReaderValue<Double_t> pspmt_loaareasum = {fReader, "pspmt.loaareasum"};
   TTreeReaderValue<Double_t> pspmt_loasum = {fReader, "pspmt.loasum"};
   TTreeReaderArray<Double_t> pspmt_loatime = {fReader, "pspmt.loatime[257]"};
   TTreeReaderArray<Double_t> pspmt_loatdiff = {fReader, "pspmt.loatdiff[257]"};
   TTreeReaderValue<Double_t> pspmt_loamax = {fReader, "pspmt.loamax"};
   TTreeReaderValue<Double_t> pspmt_loamaxtime = {fReader, "pspmt.loamaxtime"};
   TTreeReaderValue<Int_t> pspmt_loamult = {fReader, "pspmt.loamult"};
   TTreeReaderValue<Int_t> pspmt_loamaxx = {fReader, "pspmt.loamaxx"};
   TTreeReaderValue<Int_t> pspmt_loamaxy = {fReader, "pspmt.loamaxy"};
   TTreeReaderValue<Int_t> pspmt_loaampmax = {fReader, "pspmt.loaampmax"};
   TTreeReaderValue<Int_t> pspmt_loaareamax = {fReader, "pspmt.loaareamax"};
   TTreeReaderValue<Double_t> pspmt_loposxEcent = {fReader, "pspmt.loposxEcent"};
   TTreeReaderValue<Double_t> pspmt_loposyEcent = {fReader, "pspmt.loposyEcent"};
   TTreeReaderValue<Double_t> pspmt_loposxEcent50 = {fReader, "pspmt.loposxEcent50"};
   TTreeReaderValue<Double_t> pspmt_loposyEcent50 = {fReader, "pspmt.loposyEcent50"};
   TTreeReaderValue<Double_t> pspmt_loposxampcent = {fReader, "pspmt.loposxampcent"};
   TTreeReaderValue<Double_t> pspmt_loposyampcent = {fReader, "pspmt.loposyampcent"};
   TTreeReaderValue<Double_t> pspmt_loposxampcent50 = {fReader, "pspmt.loposxampcent50"};
   TTreeReaderValue<Double_t> pspmt_loposyampcent50 = {fReader, "pspmt.loposyampcent50"};
   TTreeReaderValue<Double_t> pspmt_loposxareacent = {fReader, "pspmt.loposxareacent"};
   TTreeReaderValue<Double_t> pspmt_loposyareacent = {fReader, "pspmt.loposyareacent"};
   TTreeReaderValue<Double_t> pspmt_loposxareacent50 = {fReader, "pspmt.loposxareacent50"};
   TTreeReaderValue<Double_t> pspmt_loposyareacent50 = {fReader, "pspmt.loposyareacent50"};
   TTreeReaderValue<unsigned int> ddasdiagnostics_fUniqueID = {fReader, "ddasdiagnostics.fUniqueID"};
   TTreeReaderValue<unsigned int> ddasdiagnostics_fBits = {fReader, "ddasdiagnostics.fBits"};
   TTreeReaderValue<Int_t> ddasdiagnostics_cmult = {fReader, "ddasdiagnostics.cmult"};
   TTreeReaderValue<Int_t> ddasdiagnostics_eventlength = {fReader, "ddasdiagnostics.eventlength"};
   TTreeReaderValue<Int_t> ddasdiagnostics_tdiffevent = {fReader, "ddasdiagnostics.tdiffevent"};
   TTreeReaderArray<Int_t> ddasdiagnostics_adchit = {fReader, "ddasdiagnostics.adchit[20]"};


   MySelector(TTree * /*tree*/ =0) { }
   virtual ~MySelector() { }
   virtual Int_t   Version() const { return 2; }
   virtual void    Begin(TTree *tree);
   virtual void    SlaveBegin(TTree *tree);
   virtual void    Init(TTree *tree);
   virtual Bool_t  Notify();
   virtual Bool_t  Process(Long64_t entry);
   virtual Int_t   GetEntry(Long64_t entry, Int_t getall = 0) { return fChain ? fChain->GetTree()->GetEntry(entry, getall) : 0; }
   virtual void    SetOption(const char *option) { fOption = option; }
   virtual void    SetObject(TObject *obj) { fObject = obj; }
   virtual void    SetInputList(TList *input) { fInput = input; }
   virtual TList  *GetOutputList() const { return fOutput; }
   virtual void    SlaveTerminate();
   virtual void    Terminate();
   double          logistic(double A, double k, double x1, double x);
   double          decay(double A, double k, double x1, double x);
   double          singlePulse(double A1, double k1, double k2, double x1, double C, double x
);
   ClassDef(MySelector,0);

};

#endif

#ifdef MySelector_cxx
void MySelector::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the reader is initialized.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   fReader.SetTree(tree);
}

Bool_t MySelector::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

#endif // #ifdef MySelector_cxx
