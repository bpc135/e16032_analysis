#include <iostream>
#include <fstream>
#include <stdlib.h>


#include "unpacker.h"
#include "correlator.h"
#include "analyzer.h"


//CALIBRATION

//specify the folder where the default calibration files are found.  This folder
//has the default calibration files for the LaBr3 and Segas
char calDefaultPath[] = "/user/e16032/RootAnalysis_Scripted_BC/e16032_analysis_full/cal";

//specify each default calibration files
string SegaDefault;
char OtherDefault[] = "OtherInit.txt";
string LaBr3Default;
string CLYCDefaut;
string PSPMTDefault;
//char LaBr3Default[] = "LaBr3Init.txt";

double starttime;
double endtime;


//*******************************************************



void reset_channel_list(vector<DDASRootFitHit*> channellist,  vector<DDASRootFitHit*>::iterator channellist_it) {
  //cout << "Reset channel list.  Size = " << channellist.size() << endl;
  
  int listsize = channellist.size();
  channellist_it = channellist.begin();
  
  for (channellist_it = channellist.begin(); 
       channellist_it < channellist.end(); 
       channellist_it++) {
    
    delete *channellist_it;
  }

  channellist.clear();
}


int unpack_data(TTree *tree_in, TTree *tree_out, string Run_Number ) {

  //Channelist of the Event Vector
  //vector<ddaschannel*>  channellist;
  //vector<ddaschannel*>::iterator channellist_it;

  vector<DDASRootFitHit*> channellist;
  vector<DDASRootFitHit*>::iterator channellist_it;

  //Variables and Results
  betadecay bdecay;
  betadecayvariables bdecayv;

  //Output of all the results...
  rootdataout *rootout = new rootdataout();

  //Instantiate correlator
  Correlator corr;

  //Reset correlator
  corr.Reset();
  
  //Results and Variables Clear
  bdecay.Reset();
  bdecayv.hit.Initialize();

  //Read in Calibrations
  char SegaInitName[100];
  char OtherInitName[100];
  char LaBr3InitName[100];
  char CLYCInitName[100];
  char PSPMTInitName[100];
  
  string tempsega;
  //tempsega = "SeGAInit_"+Run_Number.substr(0,4)+".txt";
  tempsega = "SeGAInit_basic.txt";
  SegaDefault = tempsega;

  string templabr3;
  //templabr3 = "LaBr3Init_"+Run_Number.substr(0,4)+".txt";
  templabr3 = "LaBr3Init_basic.txt";
  LaBr3Default = templabr3;

  string temppspmt;
  //temppspmt = "PSPMTInit_"+Run_Number.substr(0,4)+".txt";
  //temppspmt = "PSPMTInit_basic.txt";
  temppspmt = "PSPMTInit_Anodes.txt";

  PSPMTDefault = temppspmt;
    
  cout<<SegaDefault<<endl;
  cout<<LaBr3Default<<endl;
  cout<<PSPMTDefault<<endl;
  sprintf(SegaInitName, "%s/%s", calDefaultPath, SegaDefault.c_str());
  sprintf(OtherInitName, "%s/%s", calDefaultPath, OtherDefault);
  sprintf(LaBr3InitName, "%s/%s", calDefaultPath, LaBr3Default.c_str());
  sprintf(PSPMTInitName, "%s/%s", calDefaultPath, PSPMTDefault.c_str());
  
  bdecayv.ReadSega(SegaInitName);
  bdecayv.ReadOther(OtherInitName);
  bdecayv.ReadLaBr3(LaBr3InitName);
  bdecayv.ReadPSPMT(PSPMTInitName);

  //Report the correlation time
  cout << "Correlation time " << bdecayv.clock.max << endl;

  //Reset some event timing variables
  starttime=0;
  endtime=0;
  
  //Make the relevant output stuff in the tree
  tree_out->Branch("rootout", "rootdataout", &rootout, 32000,99);
  
  //Find the number of events in a file
  Int_t nevents = tree_in->GetEntries();
  cout << "Number events in run segment: " << tree_in->GetEntries() << endl << endl;

  //Progress metrics
  int fiveper = (int)(nevents * 0.05);
  int oneper = (int)(nevents * 0.01);
  int tenthper = (int)(nevents * 0.001);

  //DDASEvent
  //DDASEvent *devent

  vector<RootHitExtension>* fitdata(0);
  tree_in->SetBranchAddress("HitFits", &fitdata);

  //RawHits
  DDASRootFitEvent *rawhits;
  //nevents = 100;

  //Loop over all the events in the file
  for (Int_t ii=0; ii<nevents; ii++) {
    // cout << "---------- new event\n";
    // //Progress bar
    if( (ii % oneper) == 0) {
      cout << "Events processed " << ii << " - percent done " << (int)(ii/oneper)*1 << "%"<<endl;
    }
    // if( (ii % tenthper) == 0) {
    //cout << "Events processed " << ii << " - percent done " << (int)(ii/tenthper)*0.1 << "%"<<endl;
    //}
    //check with only 1% of the file
    // if( (ii > 0) && (ii % fiveper) == 0) {
    //   break;
    // }

    //Reset the channel list from the last event
    reset_channel_list(channellist,channellist_it);

    //Make the event
    //devent = new DDASEvent();
    rawhits = new DDASRootFitEvent();
    
    //***NEEDED FOR EVT BLT***//
    TBranch *aRawHitsevent = tree_in->GetBranch("RawHits");
    aRawHitsevent->SetAddress(&rawhits);
    
    //    TBranch *fitBranch = tree_in->GetBranch("HitFits");
    //   fitBranch->SetAddress(&fitdata);

    
    //Get the event
    tree_in->GetEntry(ii);

    vector<RootHitExtension>& fits(*fitdata);
    
    //Make the channel list
    //vector<ddaschannel*> eventdata;
    vector<DDASRootFitHit*> eventdata;
   
    //vector<DDASFitHit*> fitdata;
    //eventdata = devent->GetData();
    eventdata = rawhits->GetData();
    channellist = eventdata;

    // cout << "This event has " << eventdata.size() << " hits\n";
    // cout << "Fits size: " << fits.size() << endl;
    // for (int i =0; i < fits.size(); i++) {
    //   cout << "Hit " << i << (fits[i].haveExtension ? "Has" : "Does not have")<< " a fit attached to it\n";
    //   if (fits[i].haveExtension) {
    // 	RootFit1Info fit1 = fits[i].onePulseFit;
    // 	cout << "Fit1 amplitude: " << fit1.pulse.amplitude << endl;
    //   }
    // }
 
    //Unpack the event
    unpack_event(ii,&bdecay,&bdecayv,channellist,channellist_it);

    unpack_double_pulse(ii,&bdecay,fitdata);
  
    //Reset root output
    rootout->Reset();

    //Correlate this event
    int impdec = corr.Correlate(bdecay,bdecayv,16,16,0);

    
    //Fill with results of analysis
    // if(bdecay.pspmt.dyenergy>0 || bdecay.segatotal.mult>0 || bdecay.labr3.mult>0) {
    if( (bdecay.pspmt.dyamp>0 && bdecay.pspmt.amult > 0) || bdecay.labr3.mult > 0) {
      
      rootout->SetOutputValues(bdecay);
      rootout->clock = bdecay.clock;
      rootout->tac = bdecay.tac;
      rootout->pin01 = bdecay.pin01;
      rootout->pin02 = bdecay.pin02;
      rootout->pin03 = bdecay.pin03;
      rootout->pin04 = bdecay.pin04;
      rootout->pid = bdecay.pid;
      rootout->corr = bdecay.corr;
      rootout->sega = bdecay.sega;
      rootout->ddasdiagnostics = bdecay.ddasdiagnostics;
      rootout->labr3 = bdecay.labr3;
      rootout->clyc = bdecay.clyc;
      rootout->i2 = bdecay.i2;
      rootout->pspmt = bdecay.pspmt;
      
      //if(bdecay->pspmt.dynode > 0){
      tree_out->Fill();
      //}
    }

    
    // if(bdecay.pspmt.dyenergy>10 && bdecay.pspmt.dyenergy < 64000) {
    //if(bdecay.pspmt.dyenergy>10) {

      // cout<<"Event: "<<ii<<endl;
      // cout<<"Energies: "<<bdecay.pspmt.posxEcent<<"  "<<bdecay.pspmt.posyEcent<<endl;
      // cout<<"Amps: "<<bdecay.pspmt.posxampcent<<"  "<<bdecay.pspmt.posyampcent<<endl;
      // cout<<"Amps50: "<<bdecay.pspmt.posxampcent50<<"  "<<bdecay.pspmt.posyampcent50<<endl;
      // cout<<"Energies50: "<<bdecay.pspmt.posxEcent50<<"  "<<bdecay.pspmt.posyEcent50<<endl;

      // cout<<"Amp Max: "<<bdecay.pspmt.aampmaxcentcal<<endl;
      //}
    
    
    
  } /* for (int i=0; i < nevents; i++) */
   
}

  
int unpack_event(int eventnum, betadecay *bdecay, betadecayvariables *bdecayv,vector<DDASRootFitHit*> channellist, vector<DDASRootFitHit*>::iterator channellist_it )  {

  bdecay->Reset();
  bdecayv->hit.Initialize();
  //cout<<endl<<endl;
  
  //
  int eventsize = channellist.size();
  //cout << eventsize << endl;

  bdecay->ddasdiagnostics.cmult = eventsize;

  channellist_it = channellist.begin();
  
  for (int j=1; j<=20; j++) {
    bdecay->ddasdiagnostics.adchit[j] = 0;
  }

  //In e14057, ringing in the dynode upon implants was identified as a problem
  //leaving this code here but commenting it out in case we need it for e16032
  //int dynode_counter=0; 
  //bool dynode_present = false;

  for (channellist_it = channellist.begin(); channellist_it < channellist.end(); channellist_it++) {

    //cout << (*channellist_it)->haveExtension << endl;


    int crateid = (*channellist_it)->crateid;
    int slotid = (*channellist_it)->slotid;
    int channum = (*channellist_it)->chanid;
    
    if(crateid==0 && slotid==2 && channum ==0) {

      if (channellist_it == channellist.begin()) {
	
	/* Calculate time difference between events. */
	double tdiffevent = ((*channellist_it)->time) - starttime;
	tdiffevent = tdiffevent/100.; /* Microseconds time difference. */
	bdecay->ddasdiagnostics.tdiffevent = tdiffevent;
	
	starttime = (*channellist_it)->time;
	
	bdecay->clock.fast = (*channellist_it)->timelow;
	bdecay->clock.slow = (*channellist_it)->timehigh;
	//bdecay->clock.full = (*channellist_it)->time;
	bdecay->clock.cfd = (*channellist_it)->cfd;
	bdecay->clock.initial = (*channellist_it)->time;
      }
      //Update the current time for each entry
      bdecay->clock.current = (*channellist_it)->time;
      
      // Unpack data for ddasdiagnostics
      // first slot is slot 2 for each crate
      unsigned int adcnumber = 0;
      if (crateid == 0) {
	adcnumber = slotid - 1;
      }else{
	//crateid = 1
	adcnumber = slotid + 12;
      }
    
      bdecay->ddasdiagnostics.adchit[adcnumber] += pow(2.,(double)(*channellist_it)->chanid);
      bdecay->adc[adcnumber].channel[channum] = ((*channellist_it)->energy);
      bdecay->tdc[adcnumber].time[channum] = ((*channellist_it)->cfd);
      bdecay->time[adcnumber].timelow[channum] = ((*channellist_it)->timelow);
      bdecay->time[adcnumber].timehigh[channum] = ((*channellist_it)->timehigh);
      bdecay->time[adcnumber].timefull[channum] = ((*channellist_it)->time);
      bdecay->time[adcnumber].timecfd[channum] = ((*channellist_it)->timecfd);
    
      //  if(adcnumber==1 && channum == 1) {
      //    cout<<"time labr3_0: "<<bdecay->time[adcnumber].timefull[channum]<<endl;
      //  }

      //cout << "bdecay->adc[" << adcnumber << ".channel[" << channum << "] = " << bdecay->adc[adcnumber].channel[channum] << endl;
    
      //check for trace and extract
      if( (*channellist_it)->tracelength != 0 ){
	bdecay->adc[adcnumber].chantrace[channum].trace = (*channellist_it)->GetTrace();	
      }
    
      //Map/analyze the channel, calibrate, and threshold check
      analyze_dynode(crateid, slotid, channum, (*channellist_it)->GetTrace(), bdecay, bdecayv, eventnum);
    }
  } //End loop over channel list for looking at dynodes

  
  for (channellist_it = channellist.begin(); channellist_it < channellist.end(); channellist_it++) {

    //cout << (*channellist_it)->haveExtension << endl;


    int crateid = (*channellist_it)->crateid;
    int slotid = (*channellist_it)->slotid;
    int channum = (*channellist_it)->chanid;    

    // dynode_present = false;
    
    // if(crateid == 0 && slotid == 11 && channum == 0) {
    //   dynode_counter++;
    //   dynode_present = true;
    // }

    // The time of an event will be taken as the time of the first 
    // channel in the event. 
    
    if (channellist_it == channellist.begin()) {
      /* Calculate time difference between events. */
      double tdiffevent = ((*channellist_it)->time) - starttime;
      tdiffevent = tdiffevent/100.; /* Microseconds time difference. */
      bdecay->ddasdiagnostics.tdiffevent = tdiffevent;

      starttime = (*channellist_it)->time;

      bdecay->clock.fast = (*channellist_it)->timelow;
      bdecay->clock.slow = (*channellist_it)->timehigh;
      //bdecay->clock.full = (*channellist_it)->time;
      bdecay->clock.cfd = (*channellist_it)->cfd;
      bdecay->clock.initial = (*channellist_it)->time;
    }
    //Update the current time for each entry
    bdecay->clock.current = (*channellist_it)->time;

    // Unpack data for ddasdiagnostics
    // first slot is slot 2 for each crate
    unsigned int adcnumber = 0;
    if (crateid == 0) {
      adcnumber = slotid - 1;
    }else{
      //crateid = 1
      adcnumber = slotid + 12;
    }
    
    bdecay->ddasdiagnostics.adchit[adcnumber] += pow(2.,(double)(*channellist_it)->chanid);
    bdecay->adc[adcnumber].channel[channum] = ((*channellist_it)->energy);
    bdecay->tdc[adcnumber].time[channum] = ((*channellist_it)->cfd);
    bdecay->time[adcnumber].timelow[channum] = ((*channellist_it)->timelow);
    bdecay->time[adcnumber].timehigh[channum] = ((*channellist_it)->timehigh);
    bdecay->time[adcnumber].timefull[channum] = ((*channellist_it)->time);
    bdecay->time[adcnumber].timecfd[channum] = ((*channellist_it)->timecfd);
    
    //  if(adcnumber==1 && channum == 1) {
    //    cout<<"time labr3_0: "<<bdecay->time[adcnumber].timefull[channum]<<endl;
    //  }

    //cout << "bdecay->adc[" << adcnumber << ".channel[" << channum << "] = " << bdecay->adc[adcnumber].channel[channum] << endl;
    
    //check for trace and extract
    if( (*channellist_it)->tracelength != 0 ){
      bdecay->adc[adcnumber].chantrace[channum].trace = (*channellist_it)->GetTrace();	
    }
    
    //Map/analyze the channel, calibrate, and threshold check
    analyze_event(crateid, slotid, channum, (*channellist_it)->GetTrace(), bdecay, bdecayv, eventnum);
    
    endtime = (*channellist_it)->time;
    
  } //End loop over channel list


  // Calculate event length in 100 ns units. 
  bdecay->ddasdiagnostics.eventlength = (endtime - starttime)*1.0;
 
  bdecay->pin01.ecorr = bdecay->pin01.ecal;
  bdecay->pin02.ecorr = bdecay->pin02.ecal;
  bdecay->pid.de1 = bdecay->pin01.ecorr;
  bdecay->pid.de2 = bdecay->pin02.ecorr;
  bdecay->pid.i2ntof = bdecay->tac.i2ncorr;
  bdecay->pid.i2stof = bdecay->tac.i2scorr;
   
  // ******* Begin event level processing. *******

  //Calculate the implant and decay centroids
  double numerator = 0;
  double denominator = 0;
  double centroid = 0;

  double max = 0;
  double max2 = 0;
  int maxch = -1;
  int maxch2 = -1;
  int mult = 0;

  int ampcounter = 0;
  
  numerator = 0;
  denominator = 0;
  centroid = -1;  

  //Determine x and y positions for pspmt centroid
  double tempx, tempy, totalE;
  tempx = tempy = totalE = 0;
  int hitcounter;
  hitcounter = 0;


  //Determine Decay Multiplicity
  int multdecay = 0;
  
  for(int eye=1; eye<257; eye++){
    if(bdecay->pspmt.aamp[eye] > 0 ) {
      multdecay++;
    }
  }

  //time difference between dynode and max anode
  bdecay->pspmt.dytdiff = bdecay->pspmt.dytime - bdecay->pspmt.amaxtime;
    
  //Determine x and y positions for pspmt
  double asumcent, asumcent50, aampsumcent, aampsumcent50, aareasumcent, aareasumcent50, amaxcent, aampmaxcent, aampmaxcentcal, aareacent, aareamaxcent,aareamaxcentcal = -1.0;

  //first determine maxima within the intended thresholds for the centroid determinations
  for(int i=1; i<257; i++){

    if(bdecay->pspmt.aecal[i]>0 && bdecay->pspmt.aecal[i] < 12000){

      if(bdecay->pspmt.atime[i] > 0 && bdecay->pspmt.dytime > 0){
	bdecay->pspmt.atdiff[i] = bdecay->pspmt.dytime - bdecay->pspmt.atime[i];
      }
      //just set it to something at wont matter
      else {
	bdecay->pspmt.atdiff[i] = 123456.78;
      }
    }
  }
  //first determine maxima within the intended thresholds for the centroid determinations
  for(int i=1; i<257; i++){

    if(bdecay->pspmt.aecal[i]>0 && bdecay->pspmt.aecal[i] < 12000  && TMath::Abs(bdecay->pspmt.atdiff[i]) < 100) {

      //  if(bdecay->pspmt.aecal[i]>0 && bdecay->pspmt.aecal[i] < 12000) {
      
      //pixie energies
      // if(bdecay->pspmt.aecal[i] > 0 && bdecay->pspmt.aecal[i] < 30000 ){
	
      if(bdecay->pspmt.dyecal>0) bdecayv->hit.pspmt = 1;
      if(bdecay->pspmt.aecal[i] > amaxcent){
	amaxcent = bdecay->pspmt.aecal[i];
	bdecay->pspmt.asum += bdecay->pspmt.aecal[i];
	//	}
      }
    
      //amplitudes
      if(bdecay->pspmt.aamp[i] > 0) {

	if(bdecay->pspmt.aampcal[i] > aampmaxcentcal){
	  aampmaxcentcal = bdecay->pspmt.aampcal[i];
	}
	if(bdecay->pspmt.aamp[i] > aampmaxcent){
	  aampmaxcent = bdecay->pspmt.aamp[i];
	}
      
      }
    
      //areas
      if(bdecay->pspmt.aarea[i] > 0){
	if(bdecay->pspmt.aareacal[i] > aareamaxcentcal){
	  aareamaxcentcal = bdecay->pspmt.aareacal[i];
	}
	if(bdecay->pspmt.aarea[i] > aareamaxcent){
	  aareamaxcent = bdecay->pspmt.aarea[i];
	}

      }
    }
  }

  // if(bdecay->pspmt.dyenergy>0) {
  //   cout<<"amaxcent: "<<amaxcent<<" Dynode: "<<bdecay->pspmt.dyenergy<<endl;
  // }

  bdecay->pspmt.amaxcent = amaxcent;
  bdecay->pspmt.aampmaxcent = aampmaxcent;
  bdecay->pspmt.aampmaxcentcal = aampmaxcentcal;
  bdecay->pspmt.aareamaxcent = aareamaxcent;
  bdecay->pspmt.aareamaxcentcal = aareamaxcentcal;
  
  for(int i = 1; i < 257; i++){

    int xpix = (int)((i-1) % 16);
    int ypix = (int)((i-1) / 16);


    // if(bdecay->pspmt.aampcal[i] > 0 && bdecay->pspmt.aampcal[i] < 100000 && TMath::Abs(bdecay->pspmt.atdiff[i]) < 50){

    if(bdecay->pspmt.aecal[i]>0 && bdecay->pspmt.aecal[i] < 12000 && TMath::Abs(bdecay->pspmt.atdiff[i]) < 100){

      // if(bdecay->pspmt.aecal[i]>0 && bdecay->pspmt.aecal[i] < 12000){
      //cout.precision(15);

      //   cout<<"Eventnum: "<<eventnum<<"  i: "<<i<<" \ty: "<<ypix<<" \tx: "<<xpix<<" \tE: "<<bdecay->pspmt.aecal[i]<<"\t A: "<<bdecay->pspmt.aampcal[i]<<"  T: "<<bdecay->pspmt.atime[i]<<"  DyT: "<<bdecay->pspmt.dytime<<" Diff: "<<bdecay->pspmt.atdiff[i]<<endl;    
      
      //pixieEcent
      bdecay->pspmt.posxEcent += xpix * bdecay->pspmt.aecal[i];
      bdecay->pspmt.posyEcent += ypix * bdecay->pspmt.aecal[i];
      asumcent += bdecay->pspmt.aecal[i];
    
      //calculate the centroids using 50% of maximum anode values
      if(bdecay->pspmt.aecal[i] > (0.50 * amaxcent)){
	bdecay->pspmt.posxEcent50 += xpix * bdecay->pspmt.aecal[i];
	bdecay->pspmt.posyEcent50 += ypix * bdecay->pspmt.aecal[i];
	asumcent50 += bdecay->pspmt.aecal[i];
      }
      
      //pixie amplitudes cent
      bdecay->pspmt.posxampcent += xpix * bdecay->pspmt.aampcal[i];
      bdecay->pspmt.posyampcent += ypix * bdecay->pspmt.aampcal[i];
      aampsumcent += bdecay->pspmt.aampcal[i];
      
      //calculate the centroids using 50% of maximum anode values
      if(bdecay->pspmt.aampcal[i] > (0.5 * bdecay->pspmt.aampmaxcentcal)){
	
	bdecay->pspmt.posxampcent50 += xpix * bdecay->pspmt.aampcal[i];
	bdecay->pspmt.posyampcent50 += ypix * bdecay->pspmt.aampcal[i];
	aampsumcent50 += bdecay->pspmt.aampcal[i];
      }

      //pixie areas cent
      bdecay->pspmt.posxareacent += xpix * bdecay->pspmt.aareacal[i];
      bdecay->pspmt.posyareacent += ypix * bdecay->pspmt.aareacal[i];
      aareasumcent += bdecay->pspmt.aareacal[i];
      
      //calculate the centroids using 50% of maximum anode values
      if(bdecay->pspmt.aareacal[i] > (0.5 * bdecay->pspmt.aareamaxcentcal)){
	bdecay->pspmt.posxareacent50 += xpix * bdecay->pspmt.aareacal[i];
	bdecay->pspmt.posyareacent50 += ypix * bdecay->pspmt.aareacal[i];
	aareasumcent50 += bdecay->pspmt.aareacal[i];
      }
    }
  }

  bdecay->pspmt.asumcent = asumcent;
  bdecay->pspmt.asumcent50 = asumcent50;
  bdecay->pspmt.aampsumcent = aampsumcent;
  bdecay->pspmt.aampsumcent50 = aampsumcent50;
  bdecay->pspmt.aareasumcent = aareasumcent;
  bdecay->pspmt.aareasumcent50 = aareasumcent50;

  
  //calculate final centroid value
  bdecay->pspmt.posxEcent = bdecay->pspmt.posxEcent / asumcent;
  bdecay->pspmt.posyEcent = bdecay->pspmt.posyEcent / asumcent;
  bdecay->pspmt.posxEcent50 = bdecay->pspmt.posxEcent50 / asumcent50;
  bdecay->pspmt.posyEcent50 = bdecay->pspmt.posyEcent50 / asumcent50;
  bdecay->pspmt.posxampcent = bdecay->pspmt.posxampcent / aampsumcent;
  bdecay->pspmt.posyampcent = bdecay->pspmt.posyampcent / aampsumcent;
  bdecay->pspmt.posxampcent50 = bdecay->pspmt.posxampcent50 / aampsumcent50;
  bdecay->pspmt.posyampcent50 = bdecay->pspmt.posyampcent50 / aampsumcent50;
  bdecay->pspmt.posxareacent = bdecay->pspmt.posxareacent / aareasumcent;
  bdecay->pspmt.posyareacent = bdecay->pspmt.posyareacent / aareasumcent;
  bdecay->pspmt.posxareacent50 = bdecay->pspmt.posxareacent50 / aareasumcent50;
  bdecay->pspmt.posyareacent50 = bdecay->pspmt.posyareacent50 / aareasumcent50;


  //Only fill this stuff for the low ("lo") gain things
  if ((bdecayv->hit.pin01 == 1) && bdecay->pspmt.dyamp > 0) {
    
    double lomax = 0.0;
    double loasum50, loaampsum50, loaareasum50 = 0.0;

    //set the max values
    bdecay->pspmt.loamax = bdecay->pspmt.amax;
    bdecay->pspmt.loaampmax = bdecay->pspmt.aampmax;
    bdecay->pspmt.loaareamax = bdecay->pspmt.aareamax;
    
    for(int i=1;i<258;i++){

      //     cout<<"Event: "<<eventnum<<"  "<<i<<"  Anode Amp: "<<bdecay->pspmt.aamp[i]<<"  Anode Area: "<<bdecay->pspmt.aarea[i]<<" Area/Amp: "<<bdecay->pspmt.aarea[i]/bdecay->pspmt.aamp[i]<<"  "<<bdecay->pspmt.baseline[i]<<"  "<<bdecay->pspmt.lowpoint[i]<<"  "<<(bdecay->pspmt.baseline[i]-bdecay->pspmt.lowpoint[i])/bdecay->pspmt.aamp[i]<<"  Mult: "<<   bdecay->pspmt.pixmult[i]<<endl;
      int xpix = (int)((i-1) % 16);
      int ypix = (int)((i-1) / 16);
      
      bdecay->pspmt.loamult++;

      //pixie energies
      if(bdecay->pspmt.aecal[i] > 0){
	
	bdecay->pspmt.loamult++;
	bdecay->pspmt.loaenergy[i] = bdecay->pspmt.aenergy[i];
	bdecay->pspmt.loaecal[i] = bdecay->pspmt.aecal[i];
	bdecay->pspmt.loposxEcent += xpix * bdecay->pspmt.loaecal[i];
	bdecay->pspmt.loposyEcent += ypix * bdecay->pspmt.loaecal[i];
	bdecay->pspmt.loasum += bdecay->pspmt.loaecal[i];

	//calculate the centroids using 50% of maximum anode values
	if(bdecay->pspmt.loaecal[i] > (0.5 * bdecay->pspmt.loamax)){
	  bdecay->pspmt.loposxEcent50 += xpix * bdecay->pspmt.loaecal[i];
	  bdecay->pspmt.loposyEcent50 += ypix * bdecay->pspmt.loaecal[i];
	  loasum50 += bdecay->pspmt.loaecal[i];
	}
      }
      
      if(bdecay->pspmt.aamp[i] > 0){
	
	bdecay->pspmt.loaamp[i] = bdecay->pspmt.aamp[i];
	bdecay->pspmt.loaampcal[i] = bdecay->pspmt.aampcal[i];
	bdecay->pspmt.loposxampcent += xpix * bdecay->pspmt.loaampcal[i];
	bdecay->pspmt.loposyampcent += ypix * bdecay->pspmt.loaampcal[i];
	bdecay->pspmt.loasum += bdecay->pspmt.loaampcal[i];

	//calculate the centroids using 50% of maximum anode values
	if(bdecay->pspmt.loaampcal[i] > (0.5 * bdecay->pspmt.loamax)){
	  bdecay->pspmt.loposxampcent50 += xpix * bdecay->pspmt.loaampcal[i];
	  bdecay->pspmt.loposyampcent50 += ypix * bdecay->pspmt.loaampcal[i];
	  loaampsum50 += bdecay->pspmt.loaampcal[i];
	}
	
      }
      
      if(bdecay->pspmt.aarea[i] > 0){
	
	bdecay->pspmt.loaarea[i] = bdecay->pspmt.aarea[i];
	bdecay->pspmt.loaareacal[i] = bdecay->pspmt.aareacal[i];
	bdecay->pspmt.loposxareacent += xpix * bdecay->pspmt.loaareacal[i];
	bdecay->pspmt.loposyareacent += ypix * bdecay->pspmt.loaareacal[i];
	bdecay->pspmt.loasum += bdecay->pspmt.loaareacal[i];
	
	//calculate the centroids using 50% of maximum anode values
	if(bdecay->pspmt.loaareacal[i] > (0.5 * bdecay->pspmt.loamax)){
	  bdecay->pspmt.loposxareacent50 += xpix * bdecay->pspmt.loaareacal[i];
	  bdecay->pspmt.loposyareacent50 += ypix * bdecay->pspmt.loaareacal[i];
	  loaareasum50 += bdecay->pspmt.loaareacal[i];
	}
      }
    }

    bdecay->pspmt.loposxEcent = bdecay->pspmt.loposxEcent / bdecay->pspmt.loasum;
    bdecay->pspmt.loposyEcent = bdecay->pspmt.loposyEcent / bdecay->pspmt.loasum;
    bdecay->pspmt.loposxEcent50 = bdecay->pspmt.loposxEcent50 / loasum50;
    bdecay->pspmt.loposyEcent50 = bdecay->pspmt.loposyEcent50 / loasum50;
    bdecay->pspmt.loposxampcent = bdecay->pspmt.loposxampcent / bdecay->pspmt.loaampsum;
    bdecay->pspmt.loposyampcent = bdecay->pspmt.loposyampcent / bdecay->pspmt.loaampsum;
    bdecay->pspmt.loposxampcent50 = bdecay->pspmt.loposxampcent50 / loaampsum50;
    bdecay->pspmt.loposyampcent50 = bdecay->pspmt.loposyampcent50 / loaampsum50;
    bdecay->pspmt.loposxareacent = bdecay->pspmt.loposxareacent / bdecay->pspmt.loaareasum;
    bdecay->pspmt.loposyareacent = bdecay->pspmt.loposyareacent / bdecay->pspmt.loaareasum;
    bdecay->pspmt.loposxareacent50 = bdecay->pspmt.loposxareacent50 / loaareasum50;
    bdecay->pspmt.loposyareacent50 = bdecay->pspmt.loposyareacent50 / loaareasum50;


    //relic from e14057: leaving as a reminder that looking at ratios of areas to amplitudes can
    //help understand the overflows
    
    // int lohitcounter;
    // lohitcounter = 0;
    // double lotempy, lotempx, lototalE;
    // lotempy = lotempx = lototalE = 0;
    
    // for(int i=1;i<258;i++){
    //   if(bdecay->pspmt.loaenergy[i] > 0 ) {     
    // 	//      cout<<bdecay->pspmt.aarea[i]<<"  "<<bdecay->pspmt.aamp[i]<<"  "<<(bdecay->pspmt.aarea[i]/bdecay->pspmt.aamp[i])<<endl;
    // 	//  if(bdecay->pspmt.ratio[i]<0.20 && (bdecay->pspmt.aarea[i]/bdecay->pspmt.aamp[i])>1) {
    // 	if((bdecay->pspmt.aarea[i]/bdecay->pspmt.aamp[i])>25) {  

    // 	  lohitcounter++;
    // 	  lotempx = lotempx+(((i-1)%8+1)*bdecay->pspmt.loaenergy[i]);
    // 	  lotempy = lotempy+((int((i-1)/8)+1)*bdecay->pspmt.loaenergy[i]);
    // 	  lototalE = lototalE + bdecay->pspmt.loaenergy[i];

    // 	}
    //   }    
    // }
    
    // if(lohitcounter>0) {
    //   bdecay->pspmt.loposxEcent = (1.0/lototalE)*lotempx;
    //   bdecay->pspmt.loposyEcent = (1.0/lototalE)*lotempy;

    //   //    cout<<"Lo Gain: "<<bdecay->pspmt.loposxEcent<<"  "<<bdecay->pspmt.loposyEcent<<endl;

    // }

  }
  
}

void unpack_double_pulse(int eventnum,  betadecay *bdecay, vector <RootHitExtension> *fitdata){

  vector<RootHitExtension>& fits(*fitdata);

  // cout << "------------- in unpack_double_pulse -------------" << endl;
  // cout << "Fits size: " << fits.size() << endl;
  // for (int i =0; i < fits.size(); i++) {
  //   cout << "Hit " << i << (fits[i].haveExtension ? "Has" : "Does not have")<< " a fit attached to it\n";
  //   if (fits[i].haveExtension) {
  //     RootFit1Info fit1 = fits[i].onePulseFit;
  //     cout << "Fit1 amplitude: " << fit1.pulse.amplitude << endl;
  //   }
  // }

  for(int i = 0; i < fits.size(); i++){
    if (fits[i].haveExtension){
      RootFit1Info fit1 = fits[i].onePulseFit;
      RootFit2Info fit2 = fits[i].twoPulseFit;

      //assign single pulse fit results
      bdecay->pspmt.dyE1_single = fit1.pulse.amplitude;
      bdecay->pspmt.dyT1_single = fit1.pulse.position;
      bdecay->pspmt.dyE1_steepness_single = fit1.pulse.steepness;
      bdecay->pspmt.dyE1_decayTime_single = fit1.pulse.decayTime;
      bdecay->pspmt.dyoffset_single = fit1.offset;
      bdecay->pspmt.dychisq_single = fit1.chiSquare;

      //check organization for defining E2 to be the second pulse
      if(fit2.pulses[0].position < fit2.pulses[1].position){

	bdecay->pspmt.dyE1_double = fit2.pulses[0].amplitude;
	bdecay->pspmt.dyT1_double = fit2.pulses[0].position;
	bdecay->pspmt.dyE1_steepness_double = fit2.pulses[0].steepness;
	bdecay->pspmt.dyE1_decayTime_double = fit2.pulses[0].decayTime;
	bdecay->pspmt.dyE2_double = fit2.pulses[1].amplitude;
	bdecay->pspmt.dyT2_double = fit2.pulses[1].position;
	bdecay->pspmt.dyE2_steepness_double = fit2.pulses[1].steepness;
	bdecay->pspmt.dyE2_decayTime_double = fit2.pulses[1].decayTime;
	
	
      }else{
	bdecay->pspmt.dyE1_double = fit2.pulses[1].amplitude;
	bdecay->pspmt.dyT1_double = fit2.pulses[1].position;
	bdecay->pspmt.dyE1_steepness_double = fit2.pulses[1].steepness;
	bdecay->pspmt.dyE1_decayTime_double = fit2.pulses[1].decayTime;
	bdecay->pspmt.dyE2_double = fit2.pulses[0].amplitude;
	bdecay->pspmt.dyT2_double = fit2.pulses[0].position;
	bdecay->pspmt.dyE2_steepness_double = fit2.pulses[0].steepness;
	bdecay->pspmt.dyE2_decayTime_double = fit2.pulses[0].decayTime;
      }
      
      bdecay->pspmt.dychisq_double = fit2.chiSquare;
      bdecay->pspmt.dyoffset_double = fit2.offset;
      bdecay->pspmt.dytdiffE1E2 = bdecay->pspmt.dyT2_double - bdecay->pspmt.dyT1_double;
      
    }
  }

}
