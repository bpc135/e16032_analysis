//FILE INCLUDES
#include "analyzer.h"

//ROOT INCLUDES
#include "TRandom.h"
#include "TRandom3.h"

//define the utilities for the various modules
void SetInitialPixie16Utilities(Pixie16Utilities *myUtils/*, vector<UShort_t> tr*/, int adcfreq, int dynodeflag){

  //Initialize the Pixie16Utilities Info
      
  int adc_freq = adcfreq;             //Sets the ADC Frequency (input is in MHz or MSPS)
  int trace_delay;          //Sets the Trace Delay (input is in ns)
  int trigfilt_gap;           //Sets the Trigger Filter Gap (input is in ns)
  int trigfilt_range;         //Sets the Trigger Filter Range
  int trigfilt_length;       //Sets the Trigger Filter Length
  int trigfilt_threshold;   //Sets the Trigger Filter Threshold
  
  int cfd_threshold;         //Sets the CFD Threshold
  int cfd_delay;             //Sets the CFD (input is in ns)
  int cfd_scalefact;          //Sets the CFD Scale Factor and calculates the weighting factor
  int tau_val;               //Sets the value of Tau and Calculates Energy Filter coefficients
  
  Int_t energy_length;        //Sets the Energy Filter Length (input is in ns)
  Int_t energy_gap;            //Sets the Energy Filter Gap (input is in ns)
  Int_t energy_range;           //Sets the Energy Filter Range

  if(adcfreq == 500 && dynodeflag == 1){
    trace_delay = 200;          
    trigfilt_gap = 0;           
    trigfilt_range = 0;         
    trigfilt_length = 100;      
    trigfilt_threshold = 10;   
  
    cfd_threshold = 20;         
    cfd_delay = 16;             
    cfd_scalefact = 0;          
    tau_val = 20;               
  
    energy_length = 200;        
    energy_gap = 60;            
    energy_range = 1;           
  }else if(adcfreq == 500){ //anode 500
    trace_delay = 80;          
    trigfilt_gap = 0;           
    trigfilt_range = 0;         
    trigfilt_length = 100;      
    trigfilt_threshold = 10;   
  
    cfd_threshold = 20;         
    cfd_delay = 16;             
    cfd_scalefact = 0;          
    tau_val = 60;               
  
    energy_length = 400;        
    energy_gap = 160;            
    energy_range = 1;
  }else{//250
    trace_delay = 80;          
    trigfilt_gap = 0;           
    trigfilt_range = 0;         
    trigfilt_length = 100;      
    trigfilt_threshold = 15;   
  
    cfd_threshold = 20;         
    cfd_delay = 16;             
    cfd_scalefact = 0;          
    tau_val = 20;               
  
    energy_length = 400;        
    energy_gap = 160;            
    energy_range = 1;
  }

  int err = 0;
  
  //trace info
  err = myUtils->SetADCFrequency(adc_freq);
  if(err != 0) {
    cout<<"Fail SetADCFrequency() err = "<<err<<endl;
  }
  err = myUtils->SetTraceDelay(trace_delay);
  if(err != 0) {
    cout<<"Fail SetTraceDelay() err = "<<err<<endl;
  }
  //trigger filter info
  // err = myUtils->SetTFGap(trigfilt_gap);
  // if(err != 0) {
  //   cout<<"Fail SetTFGap() err = "<<err<<endl;
  // }
  // err = myUtils->SetTFRange(trigfilt_range);
  // if(err != 0) {
  //   cout<<"Fail SetTFRange() err = "<<err<<endl;
  // }
  // err = myUtils->SetTFLength(trigfilt_length);
  // if(err != 0) {
  //   cout<<"Fail SetTFLength() err = "<<err<<endl;
  // }
  // err = myUtils->SetTFThreshold(trigfilt_threshold);
  // if(err != 0) {
  //   cout<<"Fail SetTFThreshold() err = "<<err<<endl;
  // }
  //cfd info
  err = myUtils->SetCFDThreshold(cfd_threshold);
  if(err != 0) {
    cout<<"Fail SetCFDThreshold() err = "<<err<<endl;
  }

  
  err = myUtils->SetCFDDelay(cfd_delay);
  if(err != 0) {
    cout<<"Fail SetCFDDelay() err = "<<err<<endl;
  }
  err = myUtils->SetCFDSF(cfd_scalefact);
  if(err != 0) {
    cout<<"Fail SetCFDSF() err = "<<err<<endl;
  }
  //energy filter info
  // err = myUtils->SetEFLength(energy_length);
  // if(err != 0) {
  //   cout<<"Fail SetEFLength() err = "<<err<<endl;
  // }
  // err = myUtils->SetEFGap(energy_gap);
  // if(err != 0) {
  //   cout<<"Fail SetEFGap() err = "<<err<<endl;
  // }
  // err = myUtils->SetEFRange(energy_range);
  // if(err != 0) {
  //   cout<<"Fail SetEFRange() err = "<<err<<endl;
  // }
  // //set peak sample
  // err = myUtils->SetPeaksample();
  // if(err != 0){
  //   cout << "Fail SetPeaksample() err = " << err << endl;
  // }

  
  
  //// //Calculate the energy filter and trigger filter
  ////cout << "trace size = " << tr.size() << endl;
  //err = myUtils->CalculateTriggerFilter(tr);
  //if(err != 0){
  //  cout << "Fail CalculateEnergyFilter() err = " << err << endl;
  //}
  //err = myUtils->CalculateEnergyFilter(tr);
  //if(err != 0){
  //  cout << "Fail CalculateEnergyFilter() err = " << err << endl;
  //}
  //tau info, which also sets the crucial filter parameters r1, b1
  // err = myUtils->SetTau(tau_val);
  // if(err != 0) {
  //   cout<<"Fail SetTau() err = "<<err<<endl;
  // }
  
  //cout << "Set Initial Pixie16Utilities" << endl;  
  
} //SetInitialPixie16Utilities


/*****************************************/
/*                                       */
/* Fill parameters with raw data.        */
/*                                       */
/*****************************************/

TRandom3 *random3 = new TRandom3();

void analyze_event(int crateid, int slotid, int channum, vector<UShort_t> trace, betadecay *bdecay, betadecayvariables *bdecayv, int eventnum) {
  //depending on what is hit, fill parameters
  //calibrate and threshold check
  double starttime;
  starttime = bdecay->clock.initial;
  
  double currenttime;

  unsigned int adcnumber = 0;
  if (crateid == 0) {
    adcnumber = slotid - 1;
  }else{
    //crateid = 1
    adcnumber = slotid + 12;
  }
  
  //create an ID that will be used to identify each channel
  //first crate has 13 modules with 16 channels each, so channels
  //in first crate go from 0 to 207
  //ID
  //0 = dynode
  //1 - 15 = LaBr3
  //16 = Pin01
  //17 = Pin02
  //18 = Pin03
  //19 = TAC1 : Pin01 - I2nTOF
  //20 = TAC2 : Pin01 - I2sTOF
  //21 = TAC3 : Pin02 - I2nTOF
  //22 = TAC4 : Pin02 - I2sTOF
  //23 = TAC5 : I2N-I2S 
  //24 = TAC6 : Pin01-RF
  //30 = I2N
  //31 = I2S
  //32 = CLYC1 (UMass Lowell)
  //33 = CLYC2 (UMass Lowell)
  //48 - 63 = SeGA
  //64 - 319 = anodes (in rows from top to bottom)
  int id = (crateid * 208) + ((slotid - 2) * 16) + channum;
  int modnum = 0;
  if (crateid == 0) modnum = slotid - 2;
  if (crateid == 1) modnum = slotid + 11;

  // if(crateid > 1 || slotid > 14 || channum > 15){
  //   cout<<"ID problem in analyze_event"<<endl;
  //   cout<<"Crate: "<<crateid<<" Slot: "<<slotid<<" Channel: "<<channum<<endl;
  // }

  //array declaring the type of module:
  //0 = 100-12 (100 MSPS, 12 bit)
  //1 = 250-14 (250 MSPS, 14 bit)
  //2 = 250-16 (250 MSPS, 16 bit)
  //3 = 500-12 (500 MSPS, 12 bit)
  //4 = 500-14 (500 MSPS, 14 bit)
  int mod_type[20] = {4,3,1,0,1,1,1,2,2,4,4,4,4,4,4,2,2,1,1,1};
  int mod_freq[20] = {500,500,250,100,250,250,250,250,250,500,500,500,500,500,500,250,250,250,250,250};

  
  //  THIS NEEDS TO BE THE REAL ONE FOR THE DATA!!!!!!!!!!!!!!!!!!
  //scaling factors for differences in bit depth between the modules
  double scale_fact[20] = {0.25,0.0625,0.25,1,0.25,0.25,0.25,0.0625,0.0625,0.25,0.25,0.25,0.25,0.25,0.25,0.0625,0.0625,0.25,0.25,0.25};
  
  /*
  //THIS IS THE ONE FOR THE TEST DATA
  //scaling factors for differences in bit depth between the modules
  double scale_fact[20] = {1,1,1,1,0.25,0.25,0.25,0.0625,0.0625,0.25,0.25,0.25,0.25,0.25,0.0625,0.0625,0.25,0.25,0.25,1.};
  */
  
  //use this for the max amplitude to use when determining overflows
  int maxsize[20]={16380,4090,16380,4090,16380,16380,16380,65530,65530,16380,16380,16380,16380,16380,16380,65530,65530,16380,16380,16380};
  
  //Make the Utilities Function
  int retval=0;
  //the above arrays can be used to reference the frequency
  Pixie16Utilities *utils = new Pixie16Utilities();
  int ModMSPS = mod_freq[modnum];
  int dynodeflag;
  if( (modnum) == 0){
    dynodeflag = 1;
    SetInitialPixie16Utilities(utils,ModMSPS,dynodeflag);
  }else{
    dynodeflag = 0;
    SetInitialPixie16Utilities(utils,ModMSPS,dynodeflag);
  }

  //calculate trace analysis parameters
  double max, min, amplitude, area, baseline, amplitudecal, areacal, overflow = 0.0;
  max = min = amplitude = baseline = 0;
  double maxE = 0;
  int maxPix = 0;
  int lo, high, loarea, hiarea, offset;
  //bounds for baseline determination
  if(trace.size() > 0){
    //  lo = 0;
    //  high = int(utils->GetTraceDelay() / (int(1000./ModMSPS)));

    if(modnum==0){
      lo = 0;
      high =Int_t ( 150. / (Int_t(1000./ModMSPS)));
    }
    else{
      lo = 0;
      high = Int_t (40. / (Int_t(1000./ModMSPS)));
    }
    
    //calculate trace properties
    utils->CalculateBaseline_PR(trace,lo,high);
    baseline = utils->GetBaseline();
    utils->CalculateTraceAmplitude(trace);
    amplitude = utils->GetTraceAmplitude();
    //modify amplitude for differences in bit depth
    // cout << amplitude << " before scale, modnum = " << modnum << ", scale_fact = " << scale_fact[modnum] << endl;
    //amplitudecal = amplitude * scale_fact[modnum] + random3->Rndm();
    amplitudecal = amplitude * scale_fact[modnum];

    // cout << amplitudecal << " after scale"<<endl;
    //get trace areas
    loarea = int (utils->GetTraceDelay() / (int(1000./ModMSPS)));
    offset = int (40. / (int(1000./ModMSPS)));
    hiarea = loarea + offset;
    
    utils->CalculateTraceArea_PR(trace,loarea,hiarea);
    area = utils->GetTraceArea();
    areacal = area * scale_fact[modnum];
    
    utils->CheckOverflowUnderflow(trace,5,maxsize[modnum]);  //cheating, 16 modules are running with low amplitude sinals so all should be fairly small
    overflow = utils->GetOverflow();
  }

  /////////////////////////////
  //going through each detector id
  ////////////////////////////

  
  //PSPMT dynode
  if( id == 0){

    bdecay->pspmt.dyoverflow = overflow; //overflow

    //if the dynode overflows, put a large number for the energy so that it does not read "0"

    //set initial dynode parameters
    //check if we have already set it
    if(amplitude > bdecay->pspmt.dyamp){
      bdecay->pspmt.dyenergy = bdecay->adc[adcnumber].channel[channum]; //Pixie energy
      bdecay->pspmt.dyecal = bdecay->adc[adcnumber].channel[channum] + random3->Rndm();

      bdecay->pspmt.dytime = bdecay->time[adcnumber].timefull[channum]; //time

      bdecay->pspmt.dyamp = amplitude;
      bdecay->pspmt.dyampcal = amplitudecal;
      bdecay->pspmt.dyarea = area;
      bdecay->pspmt.dyareacal = areacal;
      bdecay->pspmt.dyeventtdc = (currenttime - starttime) + 3000.;
      
    }

    //  cout<<endl<<"Dynode: "<<bdecay->pspmt.dyenergy<<endl<<endl;
    
    
    //set overflows to have a high energy
    if(bdecay->pspmt.dyoverflow == 1){
      bdecay->pspmt.dyenergy = 99999;
      bdecay->pspmt.dyecal = 99999;
      
    }
    
    

    /*    
    //Double pulse stuff
    if(trace.size() > 0){
      vector<int> trigger_points;
      vector<double> energies;
      vector<double> TF;
      vector<double> EF;
      vector<double> times;

      //clear vectors;
      trigger_points.clear();
      TF.clear();
      EF.clear();
      energies.clear();
      times.clear();

      double tau = utils->GetTau();
      double tau_ct = tau / (int(1000./ModMSPS)); //tau in clock ticks
      int EFLength = utils->GetEFLength(); //in clock ticks
      int EFGap = utils->GetEFGap(); //in clock ticks

      //Calculate Parameters
      double b1 = exp(-1.0/tau_ct);
      double a0 = pow(b1,EFLength)/(pow(b1,EFLength)-1.0);  //Sum 0 coefficient
      double ag = 1;                                        //the gap coefficient is always 1
      double a1 = -1/(pow(b1,EFLength)-1.0);                //Sum 1 coefficient

    
      //Only do stuff if the trace is not overflow
      //if(utils->GetOverflow()==0  && bdecay->pspmt.dynode_area>0) {
      //Do stuff if pixieE > 0 for now... could instead do if area or amplitude > 0
      if(utils->GetOverflow()==0  && bdecay->pspmt.dyenergy>0) {
	//only calculate the trigger filter if we think we have a good dynode since it is computationally expensive to do so
	utils->CalculateTriggerFilter(trace);    
	TF = utils->GetTFOut();  //get the response of the trigger filter, which can have multiple trigger points

	//Other Variables
	double Threshold = 30;  //threshold for the trigger filter (ZCP)
	int PeakSamp = 6;        //Number of clock ticks to wait
	int trace_size = 500;    //Length of the trace to sample
	int Back=0;

	for(int q=0; q<trace_size; q++)  {
	  if(trigger_points.size()==0) {
	    // if(trigger_points.size()>=0) {
	    //For the settings of TF length 10 and gap 0 the peak is at q-3 before the TF ZCP
	    if(TF[q]>=0 && TF[q+1]<0 && TF[q-3]>Threshold){
	      trigger_points.push_back(q);
	      times.push_back(q+TF[q]/(TF[q]+fabs(TF[q+1])));
	   
	      //investigating multiple triggers... this used to have some old plot code that looked at the .txt output
	      // if(trigger_points.size()>1) {
	      // ofstream plotdata;
	      // plotdata.open("/mnt/projects/decay/prokop/PlotData.txt", std::ofstream::out | std::ofstream::app);
	      // plotdata<<TF[q-3]/TF[trigger_points[0]-3]<<"  "<<times[trigger_points.size()-1]-times[0]<<"\n";
	      // plotdata.close();
	      // }

	    }
	  }
	  //  THIS IS THE ORIGINAL THAT SHOULD BE USED 
	  else {
	    //Dynamic Threshold from e14057
	    // if(TF[q]>0 && TF[q+1]<0 && trigger_points[0]<300 && trigger_points[0]>100) {
	    //   Threshold = TF[trigger_points[trigger_points.size()-1]-3]* (exp(-0.674-0.0512*(q-trigger_points[trigger_points.size()-1]))  +0.0269);
	    //   Threshold = 1.25*Threshold;
	    //   //If it drops too low gotta bring it back up
	    //   if(Threshold<30) {
	    //     Threshold = 30;
	    //   }
	    //   if(TF[q-3]>Threshold){
	    //     // cout<<q<<"  Threshold: "<<Threshold<<"  "<<TF[trigger_points[trigger_points.size()-1]-3]<<"  "<<TF[q-3]<<endl;

	    //     trigger_points.push_back(q);
	    //     times.push_back(q+TF[q]/(TF[q]+fabs(TF[q+1])));	  
	    //   }
	    // }
	  
	    //no dynamic threshold
	    if(TF[q-3]>Threshold){
	      // cout<<q<<"  Threshold: "<<Threshold<<"  "<<TF[trigger_points[trigger_points.size()-1]-3]<<"  "<<TF[q-3]<<endl;
	    
	      trigger_points.push_back(q);
	      times.push_back(q+TF[q]/(TF[q]+fabs(TF[q+1])));	  
	    }
	  
	  }

	}

	//Perform the energy analysis only if there is a second trigger point
	//if(trigger_points.size()==2) {
	
	//   //output event number when we find a double pulse
	//   cout << "double pulse at event " << eventnum << endl;
	
	//   //Running sums
	//   double S0=0;
	//   double S1=0;
	//   double Sg=0;
	//   int m=0;
	//   int A=0;
	//   double Filter_Value=0;
	//   double temp=0;
	
	//   //Calculate the Filter Response
	//   for(m=0; m<(2*EFLength+EFGap-1); m++) {
	//     EF.push_back(0);
	//   }
	
	//   m=2*EFLength+EFGap;
	
	//   for(A=(m-EFLength-EFGap-EFLength); A<(m-EFGap-EFLength); A++){
	//     S0+=trace[A];
	//   }
	//   for(A=(m-EFGap-EFLength); A<(m-EFLength); A++) {
	//     Sg+=trace[A];
	//   }  
	//   for(A=(m-EFLength); A<m; A++){
	//     S1+=trace[A];
	//   }
	
	//   for(m=(2*EFLength+EFGap+1); m<(Int_t)trace_size; m++) {
	  
	//     S0=S0-trace[m-2*EFLength-EFGap-1]+trace[m-EFGap-EFLength-1];
	//     Sg=Sg-trace[m-EFGap-EFLength-1]+trace[m-EFLength-1];
	//     S1=S1-trace[m-EFLength-1]+trace[m-1];
	  
	//     Filter_Value = (Double_t)(a0*S0+ag*Sg+a1*S1-a0*EFLength*(utils->GetBaseline())-ag*EFGap*(utils->GetBaseline())-a1*EFLength*(utils->GetBaseline()));
	  
	//     EF.push_back(Filter_Value);      
	//   }
	
	//   for(int q=0; q<(int)trigger_points.size(); q++) {
	//     energies.push_back(EF[trigger_points[q]+PeakSamp]);
	  
	//     if(q>Back+10+1) {
	//       temp=0;
	//       for(int r =trigger_points[q]-(Back+10); r<trigger_points[q]-Back; r++) {
	// 	temp+=EF[r];
	//       } 
	//       temp = temp/10.0;
	//       energies.push_back(EF[trigger_points[q]+PeakSamp]-temp);
	//       //cout<<temp<<endl;
	//     }
	//   }
	
	//   bdecay->pspmt.dyE1 = energies[0];
	//   bdecay->pspmt.dyE2 = energies[1];
	//   bdecay->pspmt.dyT1 = (int(1000./ModMSPS))*times[0];
	//   bdecay->pspmt.dyT2 = (int(1000./ModMSPS))*times[1];
	//   bdecay->pspmt.dytdiffE1E2 = (int(1000./ModMSPS))*(times[1]-times[0]);

	  
	//     // cout<<"E1: "<< bdecay->pspmt.dynodeE1 << " E2: "<< bdecay->pspmt.dynodeE2<<" T1: "<<bdecay->pspmt.dynodeT1<<" T2: "<<bdecay->pspmt.dynodeT2<<" TDiff: "<<bdecay->pspmt.dynodeTDiff<<endl;

	//     for(int kk=0; kk<600; kk++) {
	//     cout<<trace[kk]<<"  "<<TF[kk]<<"  "<<EF[kk]<<endl;
	//     }
	//     string found;
	//     cin>>found;
	  
	
	// }
	//end of check on trigger points vector size

	//check for even more triggers... just display a message for now though
	if(trigger_points.size()>2 && trigger_points.size()<5 ) {
	  cout<<" Potential Triple: "<<eventnum<<"  "<<trigger_points.size()<<endl;
	}
      
      } // end overflow check (and energy requirement)

    } //end double pulse work
*/
    
  } //end of dynode processing (id = 0)

  //LaBr3
  if(id >= 1 && id<16) {
    int detnum = id;
    
    //Register energy
    bdecay->labr3.energy[detnum] = (bdecay->adc[adcnumber].channel[channum]) + (random3->Rndm());    
    
      
    bdecay->labr3.ecal[detnum] = (bdecayv->labr3.square[detnum]*pow(bdecayv->labr3.square[detnum],2) + (bdecayv->labr3.slope[detnum] * bdecay->labr3.energy[detnum]) + bdecayv->labr3.intercept[detnum]);
      
    bdecayv->hit.labr3 = 1;

    //Times
    //bdecay->labr3.timecfd[detnum] = bdecay->tdc[adcnumber].time[channum];
    bdecay->labr3.timecfd[detnum] = bdecay->time[adcnumber].timecfd[channum];
    bdecay->labr3.timelow[detnum] = bdecay->time[adcnumber].timelow[channum];
    bdecay->labr3.timehigh[detnum]= bdecay->time[adcnumber].timehigh[channum];
    bdecay->labr3.time[detnum] = bdecay->time[adcnumber].timefull[channum];
    
    //Current time and TDC
    currenttime = bdecay->time[adcnumber].timefull[channum];

    // currenttime = bdecay->time[adcnumber].timelow[channum] + bdecay->time[adcnumber].timehigh[channum] * 4294967296.;
    bdecay->labr3.eventtdc[detnum] = (currenttime - starttime) + 3000.;
    bdecay->labr3.ecal[detnum] = (bdecayv->labr3.square[detnum]*bdecay->labr3.energy[detnum]*bdecay->labr3.energy[detnum]) + (bdecayv->labr3.slope[detnum]*bdecay->labr3.energy[detnum]) + bdecayv->labr3.intercept[detnum];
    bdecay->labr3.mult++;

    //trace analysis
    bdecay->labr3.amp[detnum] = amplitude;
    bdecay->labr3.ampcal[detnum] = amplitudecal;
    bdecay->labr3.area[detnum] = area;
    bdecay->labr3.areacal[detnum] = areacal;
    
  }

  //Clyc
  if(id >= 32 && id<48) {
    int detnum = (id-32)+1;

    cout << "clyc found " << id << " " << detnum << endl;
    
    //Register energy
    bdecay->clyc.energy[detnum] = (bdecay->adc[adcnumber].channel[channum]) + (random3->Rndm());    
    
      
    //bdecay->clyc.ecal[detnum] = (bdecayv->clyc.square[detnum]*pow(bdecayv->clyc.square[detnum],2) + (bdecayv->clyc.slope[detnum] * bdecay->clyc.energy[detnum]) + bdecayv->clyc.intercept[detnum]);
      
    bdecayv->hit.clyc = 1;

    //Times
    //bdecay->clyc.timecfd[detnum] = bdecay->tdc[adcnumber].time[channum];
    bdecay->clyc.timecfd[detnum] = bdecay->time[adcnumber].timecfd[channum];
    bdecay->clyc.timelow[detnum] = bdecay->time[adcnumber].timelow[channum];
    bdecay->clyc.timehigh[detnum]= bdecay->time[adcnumber].timehigh[channum];
    bdecay->clyc.time[detnum] = bdecay->time[adcnumber].timefull[channum];
    
    //Current time and TDC
    currenttime = bdecay->time[adcnumber].timefull[channum];

    // currenttime = bdecay->time[adcnumber].timelow[channum] + bdecay->time[adcnumber].timehigh[channum] * 4294967296.;
    bdecay->clyc.eventtdc[detnum] = (currenttime - starttime) + 3000.;
    bdecay->clyc.ecal[detnum] = (bdecayv->clyc.square[detnum]*bdecay->clyc.energy[detnum]*bdecay->clyc.energy[detnum]) + (bdecayv->clyc.slope[detnum]*bdecay->clyc.energy[detnum]) + bdecayv->clyc.intercept[detnum];
    bdecay->clyc.mult++;

    //trace analysis
    bdecay->clyc.amp[detnum] = amplitude;
    bdecay->clyc.ampcal[detnum] = amplitudecal;
    bdecay->clyc.area[detnum] = area;
    bdecay->clyc.areacal[detnum] = areacal;

    //first sum
    for(int i=200;i<=245;i++){
      bdecay->clyc.clyc_sum1[detnum] = bdecay->clyc.clyc_sum1[detnum] + trace[i];
    }
    //second sum
    for(int i=246;i<=400;i++){
      bdecay->clyc.clyc_sum2[detnum] = bdecay->clyc.clyc_sum2[detnum] + trace[i];
    }
  }

  //I2N input into PIXIE
  if(id == 30){
    bdecay->i2.i2nenergy = bdecay->adc[adcnumber].channel[channum];
    bdecay->i2.i2necal = bdecay->i2.i2nenergy; //need to add ability to calibrate this
    currenttime = bdecay->time[adcnumber].timefull[channum];

    //  currenttime = bdecay->time[adcnumber].timelow[channum] + bdecay->time[adcnumber].timehigh[channum] * 4294967296.;
    bdecay->i2.i2ntime = currenttime;
    bdecay->i2.i2ntdc = (currenttime - starttime) + 3000.;

    if(bdecay->i2.i2necal > bdecayv->i2n.thresh){
      bdecayv->hit.i2n = 1;
    }    
  }

  //I2S input into PIXIE
  if(id == 31){
    bdecay->i2.i2senergy = bdecay->adc[adcnumber].channel[channum];
    bdecay->i2.i2secal = bdecay->i2.i2senergy; //need to add ability to calibrate this
    currenttime = bdecay->time[adcnumber].timefull[channum];

    //  currenttime = bdecay->time[adcnumber].timelow[channum] + bdecay->time[adcnumber].timehigh[channum] * 4294967296.;
    bdecay->i2.i2stime = currenttime;
    bdecay->i2.i2stdc = (currenttime - starttime) + 3000.;

    if(bdecay->i2.i2secal > bdecayv->i2s.thresh){
      bdecayv->hit.i2s = 1;
    }    
  }

  // Pin01
  if( id == 16 ) {
    bdecay->pin01.energy = bdecay->adc[adcnumber].channel[channum];
    bdecay->pin01.ecal = (bdecayv->pin01.slope*bdecay->pin01.energy + bdecayv->pin01.intercept);
    currenttime = bdecay->time[adcnumber].timefull[channum];

    //  currenttime = bdecay->time[adcnumber].timelow[channum] + bdecay->time[adcnumber].timehigh[channum] * 4294967296.;
    bdecay->pin01.time = currenttime;
    bdecay->pin01.eventtdc = (currenttime - starttime) + 3000.;

    if(bdecay->pin01.ecal > bdecayv->pin01.thresh){
      bdecayv->hit.pin01 = 2;
    }
    if(bdecay->pin01.ecal > bdecayv->pin01.ithresh && bdecay->pin01.ecal < bdecayv->pin01.iuld){
      bdecayv->hit.pin01 = 1;
    }
    
  }

  // Pin02
  if( id == 17 ) {
    bdecay->pin02.energy = bdecay->adc[adcnumber].channel[channum];
    bdecay->pin02.ecal = (bdecayv->pin02.slope*bdecay->pin02.energy + bdecayv->pin02.intercept);
    currenttime = bdecay->time[adcnumber].timefull[channum];
    //currenttime = bdecay->time[adcnumber].timelow[channum] + bdecay->time[adcnumber].timehigh[channum] * 4294967296.;
    bdecay->pin02.time = currenttime;
    bdecay->pin02.eventtdc = (currenttime - starttime) + 3000.;

    if(bdecay->pin02.ecal > bdecayv->pin02.thresh){
      bdecayv->hit.pin02 = 1;
    }
    else if(bdecay->pin02.ecal>10){
      //bdecayv->hit.pin02 = 2;
    }
    else{
      bdecayv->hit.pin02 = -1;
    }
  }

  // Pin03
  if ( id == 18 ) {
    bdecay->pin03.energy = bdecay->adc[adcnumber].channel[channum];
    bdecay->pin03.ecal = (bdecayv->pin03.slope*bdecay->pin03.energy + 
			  bdecayv->pin03.intercept);
    
    bdecay->pin03.eventtdc = ((bdecay->time[adcnumber].timefull[channum] - 
			       starttime) + 3000.);
    
    if( (bdecay->pin03.ecal > bdecayv->pin03.thresh) && (bdecay->pin03.ecal < 30000) ) {
      bdecayv->hit.pin03 = 1;
    } else if(bdecay->pin03.ecal>20) { 
      bdecayv->hit.pin03 = 2; 
    } else {
      bdecayv->hit.pin03 = -1;
    }
  }

  //calculating I2 corrections from PIN01
  // TAC1 : Pin01 - I2nTOF
  if( id == 19 ) {
    bdecay->tac.pin01i2nE = bdecay->adc[adcnumber].channel[channum];  
    currenttime = bdecay->time[adcnumber].timefull[channum];

    // currenttime = bdecay->time[adcnumber].timelow[channum] + bdecay->time[adcnumber].timehigh[channum] * 4294967296.;
    bdecay->tac.pin01i2nT = (currenttime - starttime) + 3000.;
    bdecay->tac.i2ncorr = bdecay->tac.pin01i2nE;// + bdecay->tac.i2pos*0.9;

    bdecay->tac.pin01i2nA = amplitude;
    bdecay->tac.pin01i2nTDC = (currenttime - starttime) + 3000.;
  }

  // TAC2 : Pin01 - I2sTOF TAC
  if( id == 20 ) {
    bdecay->tac.pin01i2sE = bdecay->adc[adcnumber].channel[channum];  
    currenttime = bdecay->time[adcnumber].timefull[channum];
    
    // //currenttime = bdecay->time[adcnumber].timelow[channum] + bdecay->time[adcnumber].timehigh[channum] * 4294967296.;
    bdecay->tac.pin01i2sT = (currenttime - starttime) + 3000.;
    bdecay->tac.i2scorr = bdecay->tac.pin01i2sE;// + bdecay->tac.i2pos*0.9;

    bdecay->tac.pin01i2sA = amplitude;
    bdecay->tac.pin01i2sTDC = (currenttime - starttime) + 3000.;
  }

  // TAC3 : Pin02 - I2nTOF
  if( id == 21 ) {
    bdecay->tac.pin02i2nE = bdecay->adc[adcnumber].channel[channum];  
    currenttime = bdecay->time[adcnumber].timefull[channum];

    // currenttime = bdecay->time[adcnumber].timelow[channum] + bdecay->time[adcnumber].timehigh[channum] * 4294967296.;
    bdecay->tac.pin02i2nT = (currenttime - starttime) + 3000.;

    bdecay->tac.pin02i2nT = amplitude;
    bdecay->tac.pin02i2nTDC = (currenttime - starttime) + 3000.;
  }

  // TAC4 : Pin02 - I2sTOF TAC
  if( id == 22 ) {
    bdecay->tac.pin02i2sE = bdecay->adc[adcnumber].channel[channum];  
    currenttime = bdecay->time[adcnumber].timefull[channum];
    
    // //currenttime = bdecay->time[adcnumber].timelow[channum] + bdecay->time[adcnumber].timehigh[channum] * 4294967296.;
    bdecay->tac.pin02i2sT = (currenttime - starttime) + 3000.;

    bdecay->tac.pin02i2sA = amplitude;
    bdecay->tac.pin02i2sTDC = (currenttime - starttime) + 3000.;
  }

  // TAC5 : I2N - I2S
  if( id == 23 ) {
    bdecay->tac.i2ni2sE = bdecay->adc[adcnumber].channel[channum];  
    currenttime = bdecay->time[adcnumber].timefull[channum];

    //  currenttime = bdecay->time[adcnumber].timelow[channum] + bdecay->time[adcnumber].timehigh[channum] * 4294967296.;
    bdecay->tac.i2ni2sT = (currenttime - starttime) + 3000.;

    bdecay->tac.i2ni2sA = amplitude;
    bdecay->tac.i2ni2sTDC = (currenttime - starttime) + 3000.;
  }

  // TAC6 : Pin01 - RF 
  if( id == 24) {
    bdecay->tac.pin01rfE = bdecay->adc[adcnumber].channel[channum];
    currenttime = bdecay->time[adcnumber].timefull[channum];

    // currenttime = bdecay->time[adcnumber].timelow[channum] + bdecay->time[adcnumber].timehigh[channum] * 4294967296.;
    bdecay->tac.pin01rfT = (currenttime - starttime) + 3000.;

    bdecay->tac.pin01rfT = amplitude;
    bdecay->tac.pin01rfTDC = (currenttime - starttime) + 3000.;
  }
  

  //SeGA
  if( (id >= 48) && (id < 64) ) { /*for eff cal files */
    
    int detnum = (id-48) + 1;

    bdecay->segatotal.mult++;
    
    //Register energy
    bdecay->sega.energy[detnum] = (bdecay->adc[adcnumber].channel[channum]) + (random3->Rndm());
    
    //   cout<<detnum<<"  "<<bdecayv->sega.square[detnum]<<"  "<<bdecayv->sega.slope[detnum]<<"   "<<bdecayv->sega.intercept[detnum]<<endl;
    if(bdecay->sega.energy[detnum] < 60000) {  /*Thresh ???*/
      
      bdecay->sega.ecal[detnum] = (bdecayv->sega.square[detnum]*pow(bdecayv->sega.square[detnum],2) + (bdecayv->sega.slope[detnum] * bdecay->sega.energy[detnum]) + bdecayv->sega.intercept[detnum]);
      
      //Thresholds
      if(bdecay->sega.ecal[detnum] > bdecayv->sega.thresh[detnum]){
	bdecayv->hit.sega = 1;
      }
    }
    
    //Times
    bdecay->sega.timecfd[detnum] = bdecay->tdc[adcnumber].time[channum];
    bdecay->sega.timelow[detnum] = bdecay->time[adcnumber].timelow[channum];
    bdecay->sega.timehigh[detnum] = bdecay->time[adcnumber].timehigh[channum];
    bdecay->sega.time[detnum] = bdecay->time[adcnumber].timefull[channum];
    
    //Current time and TDC
    currenttime = bdecay->time[adcnumber].timefull[channum];

    // currenttime = bdecay->time[adcnumber].timelow[channum] + bdecay->time[adcnumber].timehigh[channum] * 4294967296.;
    bdecay->sega.eventtdc[detnum] = (currenttime - starttime) + 3000.;
    
  }


  //PSPMT anode : ID 64 - 319
  if( (id >= 64) && (id < 320) ){
    int pix = 0;
    int rownum = modnum - 4;
    //  int ypix = channum;
    //  int xpix = 15 - rownum;
    
     int xpix = channum;
     int ypix = 15 - rownum;
    pix = (ypix * 16) + xpix + 1;    
    
    if(pix>256) {
      cout<<"Pixel > 256   Problem with Pixel Number in PSPMT unpacker"<<endl;
    }
    if(pix<1) {
      cout<<"Pixel < 1   Problem with Pixel Number in PSPMT unpacker"<<endl;
    }

    //only take the first trigger from this channel in the event
    //if(bdecay->pspmt.pixmult[pix]==0) {
    //  if(mult[pix]==0) {
    
    //Do some trace stuff;
    double max, min = 0.0;
    if(trace.size() > 0){
      //check if the trace goes below baseline
      //Min and Max
      for(int q=0; q<trace.size(); q++) {
	if(trace[q]>max) {
	  max = trace[q];
	}
	if(trace[q]<min) {
	  min = trace[q];
	}
      }
      max = max-baseline;
    }
    
    
    bdecay->pspmt.aoverflow[pix] = overflow; //overflow
    if(overflow == 1){
      bdecay->pspmt.aoverflowcount += 1;
    }
    bdecay->pspmt.lowpoint[pix] = min; //not scaled
    bdecay->pspmt.baseline[pix] = baseline;
    bdecay->pspmt.ratio[pix] = (baseline-min)/max;
    bdecay->pspmt.amult += 1;

    if(amplitude > bdecay->pspmt.aamp[pix]){
      bdecay->pspmt.aenergy[pix] = bdecay->adc[adcnumber].channel[channum];
      bdecay->pspmt.aeventtdc[pix] = (currenttime - starttime) + 3000.;
      bdecay->pspmt.aecal[pix] = bdecay->pspmt.aenergy[pix];
      bdecay->pspmt.aamp[pix] = amplitude;
      bdecay->pspmt.aampcal[pix] = amplitudecal;
      bdecay->pspmt.aarea[pix] = area;
      bdecay->pspmt.aareacal[pix] = areacal;
      bdecay->pspmt.atime[pix] = bdecay->time[adcnumber].timefull[channum];
    }
    //cout << bdecay->adc[adcnumber].channel[channum] << " " << bdecay->pspmt.aenergy[pix] << endl;
    //bdecay->pspmt.aenergy[pix] = bdecay->adc[adcnumber].channel[channum];
    //cout<<"pix: "<<pix<<" pspmt.aenergy: "<<bdecay->pspmt.aenergy[pix];
    
    
    bdecay->pspmt.pixmult[pix] = bdecay->pspmt.pixmult[pix] + 1;

    //set max using scaled amplitudes so bit depth doesn't
    //alter the idea of what is large or small
    if(bdecay->pspmt.aecal[pix] > bdecay->pspmt.amax){
      bdecay->pspmt.amax = bdecay->pspmt.aecal[pix];
      bdecay->pspmt.amaxx = xpix;
      bdecay->pspmt.amaxy = ypix;
      bdecay->pspmt.amaxtime = bdecay->time[adcnumber].timefull[channum];
    }
    if(bdecay->pspmt.aampcal[pix] > bdecay->pspmt.aampmaxcal){
      bdecay->pspmt.aampmax = bdecay->pspmt.aamp[pix];
      bdecay->pspmt.aampmaxcal = bdecay->pspmt.aampcal[pix];
      bdecay->pspmt.aampmaxx = xpix;
      bdecay->pspmt.aampmaxy = ypix;
      bdecay->pspmt.aampmaxtime = bdecay->time[adcnumber].timefull[channum];
    }
    if(bdecay->pspmt.aareacal[pix] > bdecay->pspmt.aareamax){
      bdecay->pspmt.aareamax = bdecay->pspmt.aareacal[pix];
      bdecay->pspmt.aareamaxx = xpix;
      bdecay->pspmt.aareamaxy = ypix;
      bdecay->pspmt.aareamaxtime = bdecay->time[adcnumber].timefull[channum];
    }
    
    // int anode_threshold = 30000; //for pixieE
    // if(bdecay->pspmt.aecal[i] > 0 && bdecay->pspmt.aecal[i] < anode_threshold){
    //   if(bdecay->pspmt.aecal[i] > bdecay->pspmt.amaxcent){
    // 	bdecay->pspmt.amaxcent = bdecay->pspmt.aecal[i];
    //   }
    // }
    
    // } // end requirement of the first trigger from this channel in the event
  }

  //pulser test
  if(id == 44){

    bdecay->pspmt.testsig = bdecay->adc[adcnumber].channel[channum];
    bdecay->pspmt.testamp = amplitude;
    bdecay->pspmt.testarea = area;
    bdecay->pspmt.testtime = bdecay->time[adcnumber].timefull[channum];
    bdecay->pspmt.testoverflow = overflow;
    cout << "have the event" << endl;
    
    
  }
  
    
  utils->Delete();
  
}


