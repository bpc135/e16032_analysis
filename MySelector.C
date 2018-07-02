#define MySelector_cxx
// The class definition in MySelector.h has been generated automatically
// by the ROOT utility TTree::MakeSelector(). This class is derived
// from the ROOT class TSelector. For more information on the TSelector
// framework see $ROOTSYS/README/README.SELECTOR or the ROOT User Manual.


// The following methods are defined in this file:
//    Begin():        called every time a loop on the tree starts,
//                    a convenient place to create your histograms.
//    SlaveBegin():   called after Begin(), when on PROOF called only on the
//                    slave servers.
//    Process():      called for each event, in this function you decide what
//                    to read and fill your histograms.
//    SlaveTerminate: called at the end of the loop on the tree, when on PROOF
//                    called only on the slave servers.
//    Terminate():    called at the end of the loop on the tree,
//                    a convenient place to draw/fit your histograms.
//
// To use this file, try the following session on your Tree T:
//
// root> T->Process("MySelector.C")
// root> T->Process("MySelector.C","some options")
// root> T->Process("MySelector.C+")
//


#include "MySelector.h"
#include <TH2.h>
#include <TStyle.h>

TH2D *hgg;
TH2D *hE2E1raw;
TH2D *hE2E1;
TH2D *hE2dT;
TFile *fileout;
TTree *smalltree;

/**
 * logistic
 *    Evaluate a logistic function for the specified parameters and point.
 *    A logistic function is a function with a sigmoidal shape.  We use it
 *    to fit the rising edge of signals DDAS digitizes from detectors.
 *    See e.g. https://en.wikipedia.org/wiki/Logistic_function for
 *    a discussion of this function.
 *
 * @param A  - Amplitude of the signal.
 * @param k1 - steepness of the signal (related to the rise time).
 * @param x1 - Mid point of the rise of the sigmoid.
 * @param x  = Location at which to evaluate the function.
 * @return double
 */
double
MySelector::logistic(double A, double k, double x1, double x)
{
    return A/(1+exp(-k*(x-x1)));
}

/**
 * decay
 *    Signals from detectors usually have a falling shape that approximates
 *    an exponential.  This function evaluates this decay at some point.
 *
 *  @param A1 - amplitude of the signal
 *  @param k1 - Decay time factor f the signal.
 *  @param x1 - Position of the pulse.
 *  @param x  - Where to evaluate the signal.
 *  @return double
 */
double
MySelector::decay(double A, double k, double x1, double x)
{
    return A*(exp(-k*(x-x1)));
}

/**
 * singlePulse
 *    Evaluate the value of a single pulse in accordance with our
 *    canonical functional form.  The form is a sigmoid rise with an
 *    exponential decay that sits on top of a constant offset.
 *    The exponential decay is turned on with switchOn() above when
 *    x > the rise point of the sigmoid.
 *
 * @param A1  - pulse amplitiude
 * @parm  k1  - sigmoid rise steepness.
 * @param k2  - exponential decay time constant.
 * @param x1  - sigmoid position.
 * @param C   - Constant offset.
 * @param x   - Position at which to evaluat this function
 * @return double
 */
double
MySelector::singlePulse(
    double A1, double k1, double k2, double x1, double C, double x
)
{
    return (logistic(A1, k1, x1, x)  * decay(1.0, k2, x1, x)) // decay term
        + C;                                        // constant.
}





void MySelector::Begin(TTree * /*tree*/)
{
   // The Begin() function is called at the start of the query.
   // When running with PROOF Begin() is only called on the client.
   // The tree argument is deprecated (on PROOF 0 is passed).

  fileout = new TFile("fileout.root","recreate");
  TTree *smalltree = fChain->CloneTree(0);
  
  hgg= new TH2D("gg","gg",1500,0,3000,1500,0,3000);
  hE2E1raw = new TH2D("hE2E1raw","hE2E1raw",2000,0,20000,2000,0,20000);
  hE2E1 = new TH2D("hE2E1","hE2E1",2000,0,20000,2000,0,20000);
  hE2dT = new TH2D("hE2dT","hE2dT",2000,0,20000,200,0,200);
  
   TString option = GetOption();
   
}

void MySelector::SlaveBegin(TTree * /*tree*/)
{
   // The SlaveBegin() function is called after the Begin() function.
   // When running with PROOF SlaveBegin() is called on each slave server.
   // The tree argument is deprecated (on PROOF 0 is passed).

   TString option = GetOption();
   counter=0;

}

Bool_t MySelector::Process(Long64_t entry)
{
   // The Process() function is called for each entry in the tree (or possibly
   // keyed object in the case of PROOF) to be processed. The entry argument
   // specifies which entry in the currently loaded tree is to be processed.
   // When processing keyed objects with PROOF, the object is already loaded
   // and is available via the fObject pointer.
   //
   // This function should contain the \"body\" of the analysis. It can contain
   // simple or elaborate selection criteria, run algorithms on the data
   // of the event and typically fill histograms.
   //
   // The processing can be stopped by calling Abort().
   //
   // Use fStatus to set the return value of TTree::Process().
   //
   // The return value is currently not used.

   fReader.SetLocalEntry(entry);

   if(*pspmt_dyamp > 0.0 && *pspmt_dyamp < 8000.0 && *corr_dtimecal > 0.0 && *corr_dtimecal < 30.0) {
     for(int eye=0; eye<17; eye++) {
       if(sega_ecal[eye] > 0) {
	 for(int jay=eye+1; jay<17; jay++) {
	   if(sega_ecal[jay] > 0) {
	     hgg->Fill(sega_ecal[eye],sega_ecal[jay],1);
	     hgg->Fill(sega_ecal[jay],sega_ecal[eye],1);
	     
	     // cout<<"gamma"<<endl;
	   }
	 }
       }
     }
   }

   if(entry %100000 == 0){
     cout << "Entry " << entry << endl;
     if(entry==0) counter++;
     cout << "file counter " << counter << endl;
   }

   double E1amp, E2amp;

   //Find E1 amplitude
   double amp   = *pspmt_dyE1_double;
   double t1     = *pspmt_dyT1_double;
   double steep = *pspmt_dyE1_steepness_double;
   double decay = *pspmt_dyE1_decayTime_double;

   double xpos = (steep*t1 + TMath::Log((steep/decay) - 1))/steep;
   E1amp = singlePulse(amp,steep,decay,t1,0.0,xpos);

   //Find E2 amplitude
   amp           = *pspmt_dyE2_double;
   double t2     = *pspmt_dyT2_double;
   steep         = *pspmt_dyE2_steepness_double;
   decay         = *pspmt_dyE2_decayTime_double;

   xpos = (steep*t2 + TMath::Log((steep/decay) - 1))/steep;
   E2amp = singlePulse(amp,steep,decay,t2,0.0,xpos);
     
   hE2E1raw->Fill(E2amp,E1amp);
   if(*pspmt_dychisq_single>30000 && *pspmt_dychisq_double<3000 ){
      hE2E1->Fill(E2amp,E1amp);
      hE2dT->Fill(E2amp,t2-t1);
      smalltree->Fill();
   }
   
   return kTRUE;
}

void MySelector::SlaveTerminate()
{
   // The SlaveTerminate() function is called after all entries or objects
   // have been processed. When running with PROOF SlaveTerminate() is called
   // on each slave server.

}

void MySelector::Terminate()
{
   // The Terminate() function is the last function to be called during
   // a query. It always runs on the client, it can be used to present
   // the results graphically or save the results to file.

  hgg->Draw("colz");
  //new TCanvas();
  hE2E1raw->Draw("colz");
  hE2E1->Draw("colz");
  hE2dT->Draw("colz");
  fileout->Write();
  fileout->Close();

}
