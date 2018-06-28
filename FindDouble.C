//SNL 2018-06-23
//
// macro to inspect the fits one-by-one from the output of Ron's threaded fitter
//
//

// inname - filename of the input file to inspect which contains the fit
// viewcrate id - optional parameter to only look at one crate, deafult is all
// viewslot id  - optional parameter to only look at one slot, default is all
// viewchan id  - optional parameter to only look at one channel, default is all


Double_t fitfunc(Double_t *x, Double_t *par){

  Double_t A1 = par[0];  //amplitude
  Double_t k1 = par[1];  //steepness
  Double_t k2 = par[2];  // decay time factor
  Double_t x1 = par[3];  //midpoint of rise of sigmoid
  Double_t F2A1 = par[4];  //amplitude
  Double_t F2k1 = par[5];  //steepness
  Double_t F2k2 = par[6];  // decay time factor
  Double_t F2x1 = par[7];  //midpoint of rise of sigmoid

  Double_t C  = par[8];  //constant offset
  
  // logistic * decay + constant
  double onefit = A1/(1+exp(-k1*(x[0]-x1))) * 1.0*(exp(-k2*(x[0]-x1)));
  double twofit = F2A1/(1+exp(-F2k1*(x[0]-F2x1))) * 1.0*(exp(-F2k2*(x[0]-F2x1)));

  return onefit + twofit + C;
    
}

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

void InspectFit(int viewcrateid=-1, int viewslotid=-1, int viewchanid =-1 ){

  TChain *tree_in = new TChain("DDASFit");
  ChainDouble(tree_in,147);

  int run_number;
  
  TFile *rootfile_in;
  //TTree *tree_in;

  time_t start, stop;

  //cout << "Input file: \t " << Filein << endl;
  
  //cout<<Filein<<endl;
 
  time(&start);
      
  // int havedata = 0;
  // rootfile_in = new TFile(Filein.c_str());
  // if(!rootfile_in->IsZombie()){
  //   havedata = 1;
  // }
  // else{
  //   cout << "error opening file" << endl;
  //   havedata = 0;
  // }

  //create fit function
  TF1 *fitf = new TF1("fitf",fitfunc,0,1000,9);

  //create trace historgram for viewing
  TH1D *hTrace  = new TH1D("hTrace","hTrace",500,0,500);

  TCanvas *c1 = new TCanvas();
  
  //go through the data in the file
  // tree_in = (TTree*)rootfile_in->Get("DDASFit");

  vector<DDASRootFitHit*> channellist;
  vector<DDASRootFitHit*>::iterator channellist_it;
  
  //Find the number of events in a file
  Int_t nevents = tree_in->GetEntries();
  cout << "Number events in run segment: " << tree_in->GetEntries() << endl << endl;

  //Progress metrics
  int fiveper = (int)(nevents * 0.05);
  int oneper = (int)(nevents * 0.01);
  int tenthper = (int)(nevents * 0.001);

  vector<RootHitExtension>* fitdata(0);
  tree_in->SetBranchAddress("HitFits", &fitdata);

  //RawHits
  DDASRootFitEvent *rawhits;
  // nevents = 100;

  //Loop over all the events in the file
  for (Int_t ii=0; ii<nevents; ii++) {

    // cout << "---------- new event\n";
    // //Progress bar
    if( (ii % tenthper) == 0) {
      cout << "Events processed " << ii << " - tenth percent done " << (int)(ii/tenthper)*1 << "%"<<endl;
      }

    // if( (ii % tenthper) == 0) {
    //cout << "Events processed " << ii << " - percent done " << (int)(ii/tenthper)*0.1 << "%"<<endl;
    //}
    //check with only 1% of the file
    // if( (ii > 0) && (ii % oneper) == 0) {
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
       
    //Get the event
    tree_in->GetEntry(ii);

    vector<RootHitExtension>& fits(*fitdata);
    
    //Make the channel list
    vector<DDASRootFitHit*> eventdata;
    
    //vector<DDASFitHit*> fitdata;
    //eventdata = devent->GetData();
    eventdata = rawhits->GetData();
    channellist = eventdata;

    channellist_it = channellist.begin();

    int i = 0; //counter;
    double firsttime = 0;
    double pin01energy = 0;

    double A1 = 0; //amplitude
    double x1  = 0; //sigmoid position
    double k1 = 0; // rise steepness
    double k2 = 0; // decay time
    double C  = 0;  //constant offset
    
    double F2_p1A1 = 0; //amplitude
    double F2_p1x1  = 0; //sigmoid position
    double F2_p1k1 = 0; // rise steepness
    double F2_p1k2 = 0; // decay time
    double F2C    = 0;  //constant offset
    double F2_p2A1 = 0; //amplitude
    double F2_p2x1  = 0; //sigmoid position
    double F2_p2k1 = 0; // rise steepness
    double F2_p2k2     = 0; // decay time
    double singlechisq = 0;
    double doublechisq = 0;

    //find pins
    for (channellist_it = channellist.begin(); channellist_it < channellist.end(); channellist_it++) {
      int crateid = (*channellist_it)->crateid;
      int slotid  = (*channellist_it)->slotid;
      int chanid  = (*channellist_it)->chanid;    
      double energy  = (*channellist_it)->energy;

      if(crateid==0 && slotid==3 && chanid == 0){
	pin01energy = energy;
      }
    }
       
    for (channellist_it = channellist.begin(); channellist_it < channellist.end(); channellist_it++) {
      
      if(channellist_it == channellist.begin()){
	firsttime = (*channellist_it)->time;
      }

      int crateid = (*channellist_it)->crateid;
      int slotid  = (*channellist_it)->slotid;
      int chanid  = (*channellist_it)->chanid;    
      double energy  = (*channellist_it)->energy;
      double time   =  (*channellist_it)->time;
      int overflow = (*channellist_it)->overflowcode;
      
      double tdiff = time - firsttime;
 
  

      //check for trace and fit
      // if a channel we care about, find fit and display
      if( (crateid == viewcrateid) || viewcrateid == -1){
	if( (slotid == viewslotid) || viewslotid == -1){
	  if( (chanid == viewchanid) || viewchanid == -1){
	    if( (*channellist_it)->tracelength != 0 ){
	      vector<ushort> trace = (*channellist_it)->GetTrace();

	      //fill trace histogram
	      hTrace->Reset();
	      hTrace->Clear();
	      for(int tr=1; tr<trace.size()-1;tr++){
		hTrace->SetBinContent(tr,trace[tr]);
	      }

	      //fits should exist at this point
	      if(fits[i].haveExtension){
		RootFit1Info fit1 = fits[i].onePulseFit;
		RootFit2Info fit2 = fits[i].twoPulseFit;

		//get parameters
		singlechisq = fit1.chiSquare;
		doublechisq = fit2.chiSquare;
		A1 = fit1.pulse.amplitude; //amplitude
		x1  = fit1.pulse.position; //sigmoid position
		k1 = fit1.pulse.steepness; // rise steepness
		k2     = fit1.pulse.decayTime; // decay time
		C    = fit1.offset;  //constant offset

		F2_p1A1 = fit2.pulses[0].amplitude; //amplitude
		F2_p1x1  = fit2.pulses[0].position; //sigmoid position
		F2_p1k1 = fit2.pulses[0].steepness; // rise steepness
		F2_p1k2     = fit2.pulses[0].decayTime; // decay time
		F2C    = fit2.offset;  //constant offset
		F2_p2A1 = fit2.pulses[1].amplitude; //amplitude
		F2_p2x1  = fit2.pulses[1].position; //sigmoid position
		F2_p2k1 = fit2.pulses[1].steepness; // rise steepness
		F2_p2k2     = fit2.pulses[1].decayTime; // decay time
		//double F1C    = fit2.offset;  //constant offset


		//set paramters
		fitf->SetParameter(0,F2_p1A1);
		fitf->SetParameter(1,F2_p1k1);
		fitf->SetParameter(2,F2_p1k2);
		fitf->SetParameter(3,F2_p1x1);

		fitf->SetParameter(4,F2_p2A1);
		fitf->SetParameter(5,F2_p2k1);
		fitf->SetParameter(6,F2_p2k2);
		fitf->SetParameter(7,F2_p2x1);
		
		fitf->SetParameter(8,F2C);

		
	      }
	      else{
		//cout << "don't have fit information for this channel" << endl;
		//cout << "      Crate - " << crateid << ", slotid - " << slotid << ",chanid - " << chanid << endl;
	      }

	      if(singlechisq > 30000 && doublechisq < 3000 && F2_p2A1 < 16000){

		cout << "Event num " << ii << endl;
		if(pin01energy > 0){
		  cout << "Event had a pin : " << pin01energy << endl;
		}
		
		cout << "Crate : " << crateid << ", slotid : " << slotid << ",chanid : " << chanid << endl;
		cout << "Single Fit, chisq: " << singlechisq << endl;
		cout << "Energy : " << energy << " ,time : " << time << " ,time diff : " << tdiff << " ,Overflow : " << overflow << endl;
		cout << "Parameters: Amp   : " << F2_p1A1 << endl;
		cout << "            Steep : " << F2_p1k1 << endl;
		cout << "            Decay : " << F2_p1k2 << endl;
		cout << "            pos   : " << F2_p1x1 << endl;
		cout << "            C     : " << C  << endl;

		cout << "Double Fit, chisq: " << doublechisq << endl;
		cout << "Parameters: Amp   : " << F2_p2A1 << endl;
		cout << "            Steep : " << F2_p2k1 << endl;
		cout << "            Decay : " << F2_p2k2 << endl;
		cout << "            pos   : " << F2_p2x1 << endl;

		
		hTrace->GetXaxis()->SetRangeUser(0,trace.size()-1);
		hTrace->Draw();
		fitf->Draw("same");
		
		c1->Modified();
		c1->Update();
		//int response;
		//cout << "continue? (1/0)"<< endl;
		//cin >> response ;
		//if(response == 0) exit;
		c1->WaitPrimitive();

		cout << "continuing ... " << endl;
	      }
      	      
	    } // end loop over having a trace
	    
	  } // end loop over channel
	  
	} // end loop over slot
	
      } // end loop over crate

      i++;
      
    } // end loop over channel list
    
    //cout << "This event has " << eventdata.size() << " hits\n";
    //cout << "Fits size: " << fits.size() << endl;

    // for (int i =0; i < fits.size(); i++) {
    //   cout << "Hit " << i << (fits[i].haveExtension ? "Has" : "Does not have")<< " a fit attached to it\n";
    //   if (fits[i].haveExtension) {
    // 	RootFit1Info fit1 = fits[i].onePulseFit;
    // 	cout << "Fit1 amplitude: " << fit1.pulse.amplitude << endl;
    //   }
    // }
       
  } /* for (int i=0; i < nevents; i++) */

}
