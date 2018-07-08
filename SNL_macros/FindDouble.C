//SNL 2018-06-23
//
// macro to inspect the fits one-by-one from the output of Ron's threaded fitter
//
//

#include "/user/e16032/threadfit/functions.h"


// inname - filename of the input file to inspect which contains the fit
// viewcrate id - optional parameter to only look at one crate, deafult is all
// viewslot id  - optional parameter to only look at one slot, default is all
// viewchan id  - optional parameter to only look at one channel, default is all

//test inclusion of other code
Double_t fitfunctest(Double_t *x, Double_t *par){

  Double_t A1 = par[0];  //amplitude
  Double_t k1 = par[1];  //steepness
  Double_t k2 = par[2];  // decay time factor
  Double_t x1 = par[3];  //midpoint of rise of sigmoid
  Double_t A2 = par[4];  //amplitude
  Double_t k3 = par[5];  //steepness
  Double_t k4 = par[6];  // decay time factor
  Double_t x2 = par[7];  //midpoint of rise of sigmoid
  Double_t C  = par[8];  //constant offset

  return DDAS::doublePulse(A1, k1, k2, x1, A2, k3, k4, x2, C, x[0]);
  
}

//test inclusion of other code
Double_t fitfuncfirst(Double_t *x, Double_t *par){
  
  Double_t A1 = par[0];  //amplitude
  Double_t k1 = par[1];  //steepness
  Double_t k2 = par[2];  // decay time factor
  Double_t x1 = par[3];  //midpoint of rise of sigmoid
  //Double_t C  = par[4];  //constant offset

  return DDAS::singlePulse(A1, k1, k2, x1, 0.0, x[0]);
  
}
//test inclusion of other code
Double_t fitfuncsecond(Double_t *x, Double_t *par){

  Double_t A2 = par[0];  //amplitude
  Double_t k3 = par[1];  //steepness
  Double_t k4 = par[2];  // decay time factor
  Double_t x2 = par[3];  //midpoint of rise of sigmoid

  return DDAS::singlePulse(A2, k3, k4, x2, 0.0, x[0]);
  
}


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
  ChainDouble(tree_in,172);
  //ChainDouble(tree_in,173);
  //ChainDouble(tree_in,175);
  //ChainDouble(tree_in,181);
  //ChainDouble(tree_in,182);
  //ChainDouble(tree_in,183);
  //ChainDouble(tree_in,184);
  //ChainDouble(tree_in,185);
  //ChainDouble(tree_in,186);
  //ChainDouble(tree_in,187);
  //ChainDouble(tree_in,188);
  //ChainDouble(tree_in,189);
  //ChainDouble(tree_in,190);
  //ChainDouble(tree_in,192);
  //ChainDouble(tree_in,193);
  //ChainDouble(tree_in,194);
  //ChainDouble(tree_in,195);
  //ChainDouble(tree_in,196);
  //ChainDouble(tree_in,197);
  //ChainDouble(tree_in,198);

  //open up an entry list
  TFile *fe = new TFile("elistdouble.root");
  TEntryList *elistdouble;// = (TEntryList*)fe->Get("elistdouble");
  
  
  int run_number;
  
  TFile *rootfile_in;
  //TTree *tree_in;

  time_t start, stop;

  //cout << "Input file: \t " << Filein << endl;
  
  //cout<<Filein<<endl;
 
  time(&start);

  TFile *f = new TFile("doublelist_172_186partial.root");
  TEntryList *doublelist = (TEntryList*)f->Get("doublelist_172_186partial.root");
  
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
  TF1 *fitf = new TF1("fitf",fitfunctest,0,1000,9);
  //create fit function
  TF1 *fitffirst = new TF1("fitffirst",fitfuncfirst,0,1000,4);
  //create fit function
  TF1 *fitfsecond = new TF1("fitfsecond",fitfuncsecond,0,1000,4);
  //create fit function
  TF1 *fitfconst = new TF1("fitfconst","[0]",0,1000);

  //create trace historgram for viewing
  TH1D *hTrace  = new TH1D("hTrace","hTrace",500,0,500);

  TH2D *hE2E1 =  new TH2D("hE2E1","hE2E1",1700,0,17000,1700,0,17000);
  
  TCanvas *c1 = new TCanvas();
  
  //go through the data in the file
  // tree_in = (TTree*)rootfile_in->Get("DDASFit");

  vector<DDASRootFitHit*> channellist;
  vector<DDASRootFitHit*>::iterator channellist_it;
  
  //Find the number of events in a file
  Int_t nevents = tree_in->GetEntries();
  cout << "Number events in run segment: " << tree_in->GetEntries() << endl << endl;

  //set the entrylist list
  if(elistdouble){
    nevents = elistdouble->GetN();
    cout << "setting the entry list " << elistdouble->GetN() << endl;
    cout << "looping over " << nevents << " events " << endl;
    tree_in->SetEntryList(elistdouble);
    cout <<"Got here " << endl;
  }
  
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
  for (Int_t ii=nevents*0.41; ii<nevents; ii++) {
  //for (Int_t ii=0; ii<nevents; ii++) {

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

    int entryNumber;
    //if entrylist exists use it to find where to get the next item
    if(elistdouble) entryNumber = tree_in->GetEntryNumber(ii);
    
    //Get the event
    if(elistdouble){
      tree_in->GetEntry(entryNumber);
    }
    else {
      tree_in->GetEntry(ii);
    }

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

    double dyE1real_double = 0;
    double dyE2real_double = 0;
    
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
		//F2_p2k1 = F2_p1k1;//fit2.pulses[1].steepness; // rise steepness
		//F2_p2k2     = F2_p1k2;//fit2.pulses[1].decayTime; // decay time

		//double F1C    = fit2.offset;  //constant offset
		
		// check for swap
		if(F2_p2x1 < F2_p1x1){
		  double tempF2_p1A1 = F2_p1A1;
		  double tempF2_p1x1 = F2_p1x1;
		  double tempF2_p1k1 = F2_p1k1;
		  double tempF2_p1k2 = F2_p1k2;

		  F2_p1A1 = F2_p2A1;
		  F2_p1x1  = F2_p2x1;
		  F2_p1k1 = F2_p2k1;
		  F2_p1k2 = F2_p2k2;

		  F2_p2A1 = tempF2_p1A1;
		  F2_p2x1 = tempF2_p1x1;
		  F2_p2k1 = tempF2_p1k1;
		  F2_p2k2 = tempF2_p1k2;
		  
		}

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

		fitffirst->SetParameter(0,F2_p1A1);
		fitffirst->SetParameter(1,F2_p1k1);
		fitffirst->SetParameter(2,F2_p1k2);
		fitffirst->SetParameter(3,F2_p1x1);

		fitfsecond->SetParameter(0,F2_p2A1);
		fitfsecond->SetParameter(1,F2_p2k1);
		fitfsecond->SetParameter(2,F2_p2k2);
		fitfsecond->SetParameter(3,F2_p2x1);

		fitfconst->SetParameter(0,F2C);


		
	      }
	      else{
		//cout << "don't have fit information for this channel" << endl;
		//cout << "      Crate - " << crateid << ", slotid - " << slotid << ",chanid - " << chanid << endl;
	      }

	      double xpos = (F2_p2k1*F2_p2x1 + TMath::Log((F2_p2k1/F2_p2k2) - 1))/F2_p2k1;

	      if(singlechisq > 70000 && doublechisq < 70000 && fitfsecond->Eval(xpos) < 4000 && fitfsecond->Eval(xpos)>0 ){

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

		fitffirst->SetLineColor(kBlue);
		fitffirst->Draw("same");
		fitfsecond->SetLineColor(kRed);
		fitfsecond->Draw("same");

		fitfconst->SetLineColor(kOrange);
		fitfconst->Draw("same");

		cout << "Peak Size 1: " << fitffirst->GetMaximum() << ", 2: " << fitfsecond->GetMaximum() << endl;
		if(F2_p1k1>0 && F2_p1k2>0 && F2_p1k1>F2_p1k2){
		  double xpos = (F2_p1k1*F2_p1x1 + TMath::Log((F2_p1k1/F2_p1k2) - 1))/F2_p1k1;
		  cout << "Peak Amp 1: " << fitffirst->Eval(xpos);
		}
		if(F2_p2k1>0 && F2_p2k2>0 && F2_p2k1>F2_p2k2){
		  //double xpos = (F2_p2k1*F2_p2x1 + TMath::Log((F2_p2k1/F2_p2k2) - 1))/F2_p2k1;
		  cout << "Peak Amp 2: " << fitfsecond->Eval(xpos);
		}
		
		c1->Modified();
		c1->Update();
		//int response;
		//cout << "continue? (1/0)"<< endl;
		//cin >> response ;
		//if(response == 0) exit;
		c1->WaitPrimitive();

		hE2E1->Fill(fitfsecond->GetMaximum(), fitffirst->GetMaximum());
		
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

  hE2E1->Draw("colz");
  
}
