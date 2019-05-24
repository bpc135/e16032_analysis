//   rqfiller example.
//
//    - Two histograms (1-dim and 2-dim ) can be filled by hand
//      - click left button or move mouse with button pressed to
//        fill histos.
//      - click right button to reset the histogram.
//
//   This example demonstrates:
//
//   - how object communication mechanism can be used for handling
//     pad's mouse/key events by interpreted class.
//
//*******************************************************************
//
//  1. Run ROOT
//  2. Type .L rqfiller.C  (or rqfiller.C++)
//  3. Type rqfiller()
//
//*******************************************************************

#include <TROOT.h>
#include <TPad.h>
#include <TH1.h>
#include <TH2.h>
#include <TCanvas.h>
#include <RQ_OBJECT.h>


/////////////////////////////////////////////////////////////////////
class HFiller {

RQ_OBJECT("HFiller")  // for classes not derived from TQObject
                      // RQ_OBJECT macro should be used
private:
   TH1   *fHist;   // histogram to fill
   TPad  *fPad;    // pad in which histogram is drawn
    Double_t xpos; // current x position of mouse
    Double_t ypos; // current y position of mouse
    TLine *lo;     // line to represent lower bound of cut

public:
   HFiller() { fHist = 0; fPad = 0; }
   ~HFiller() { }
   TH1  *GetHist() const { return fHist; }
   void  SetHist(TH1 *hist);
   TPad *GetPad() const { return fPad; }
   void  SetPad(TPad *pad);
   void  ExecuteEvent(Int_t event, Int_t px, Int_t py, TObject *sel);
   void  ExecuteEventProj(Int_t event, Int_t px, Int_t py, TObject *sel);
   void  Draw(Option_t *opt = "") { fHist->AppendPad(opt); }
    void  SetXpos(Double_t a) {xpos = a; }
    void  SetYpos(Double_t a) {ypos = a; }
    Double_t GetXpos() const { return xpos; }
    Double_t GetYpos() const { return ypos; }
    void SetLineLo(TLine &line) {lo = line; }
    TLine &GetLineLo() {return lo;}
};

//___________________________________________________________________
void HFiller::SetHist(TH1 *hist)
{
   // Sets new hist for manual filling.

   if (!hist || (hist == fHist)) return;
   fHist = hist;
}

//___________________________________________________________________
void HFiller::SetPad(TPad *pad)
{
   // Sets new pad containing histogram.

   if (!pad || (pad == fPad)) return;
   fPad = pad;
}

//___________________________________________________________________
void HFiller::ExecuteEvent(Int_t event, Int_t px, Int_t py, TObject *sel)
{
   // Actions in reponse to mouse button events.

   TCanvas *c = (TCanvas *) gTQSender;
   TPad *pad = (TPad *) c->GetSelectedPad();
   if (!pad || !fHist) return;
   if (pad != fPad) return;

   if ((event == kButton1Down) ||
       (event == kButton1Double) ||
      ((event == kButton1Motion) && (sel == fHist))) {

      Float_t x = pad->AbsPixeltoX(px);
      Float_t y = pad->AbsPixeltoY(py);
      x = pad->PadtoX(x);
      y = pad->PadtoY(y);

//       if (fHist->InheritsFrom(TH2::Class()))
//           //fHist->Fill(x,y);
//       else
//           //fHist->Fill(x);

      pad->cd();
      pad->Modified();
      if (event == kButton1Motion) pad->Update(); //force update
   }

   if (event == kButton3Down) {
      fHist->Reset();
      pad->Modified();
      pad->Update();
   }
}

//___________________________________________________________________
void HFiller::ExecuteEventProj(Int_t event, Int_t px, Int_t py, TObject *sel)
{
   // Actions in reponse to mouse button events.

   TCanvas *c = (TCanvas *) gTQSender;
   TPad *pad = (TPad *) c->GetSelectedPad();
   if (!pad || !fHist) return;
   if (pad != fPad) return;

   string padname = pad->GetName();
//   cout << "pad name " << padname << " position " << fHist->GetXaxis()->FindBin(pad->AbsPixeltoX(px)) << endl;
   
   if ((event == kButton1Down) ||
       (event == kButton1Double) ||
      ((event == kButton1Motion) && (sel == fHist))) {

      Float_t x = pad->AbsPixeltoX(px);
      Float_t y = pad->AbsPixeltoY(py);
      x = pad->PadtoX(x);
      y = pad->PadtoY(y);
      
      if (fHist->InheritsFrom(TH2::Class()))
         fHist->Fill(x,y);
      else
          //fHist->Fill(x);

      pad->cd();
      pad->Modified();
      if (event == kButton1Motion) pad->Update(); //force update
   }

   // l - 108
   // h - 104
   // b - 98
   // p - 112

   if(event == kMouseMotion){
       Float_t x = pad->AbsPixeltoX(px);
       Float_t y = pad->AbsPixeltoY(py);
       x = pad->PadtoX(x);
       y = pad->PadtoY(y);
       SetXpos(x);
       SetYpos(y);
       //cout << "xpos " << GetXpos() << " ypos " << GetYpos() << endl;
   }

   
   if((event == kKeyPress)){
       //cout << "keyboard pressed " << gPad->GetEventX() << endl;
       
       TLine *lo, *hi, *bg, *bghi;
       
       TIter next(gPad->GetListOfPrimitives());
       while(obj=next()){
           //cout << "reading " << obj->GetName() << endl;
           if(obj->InheritsFrom("TLine")){
               //cout << "Line " << obj->GetName() << " " <<obj->GetUniqueID() << endl;
               if(obj->GetUniqueID() == 12345677) lo = (TLine*)obj;
               if(obj->GetUniqueID() == 12345678) hi = (TLine*)obj;
               if(obj->GetUniqueID() == 12345679) bg = (TLine*)obj;
               if(obj->GetUniqueID() == 12345680) bghi=(TLine*)obj;
           }
       }
       
       Double_t ymin = fHist->GetMinimum();
       Double_t ymax = fHist->GetMaximum();
       Double_t range = ymax - ymin;
       if(ymin == ymax){
           cout << "You appear to be trying to display limits on" << endl;
           cout << "a spectrum which does not have any variation in" << endl;
           cout << "the number of counts per bin.  Sorry but I do" << endl;
           cout << "not understand how to display limits in this case" << endl;
           cout << "but the projection will still work"<<endl;
       }
       Int_t xbin = fHist->GetXaxis()->FindBin(GetXpos());
       Double_t xbinloedge = fHist->GetXaxis()->GetBinLowEdge(xbin);
       Double_t xbinhiedge = fHist->GetXaxis()->GetBinUpEdge(xbin);
       //cout << "xpos " << GetXpos() << " bin " << xbin
       //     << " low " <<  xbinloedge << " " << xbinhiedge << endl;
       
       if(gPad->GetEventX() == 108){ // "l" pressed, set lower limit  
           
           if(!lo){
//               lo = new TLine(GetXpos(),ymin+0.1*range,GetXpos(),ymax-0.1*range);
               lo = new TLine(xbinloedge,ymin+0.1*range,xbinloedge,ymax-0.1*range);
               lo->SetLineColor(4);
               lo->SetUniqueID(12345677);
               lo->Draw();
           }else{
               lo->SetX1(/*GetXpos()*/xbinloedge);
               lo->SetY1(ymin+0.1*range);
               lo->SetX2(/*GetXpos()*/xbinloedge);
               lo->SetY2(ymax-0.1*range);
           }   
           pad->Modified();
           pad->Update();
       }
       if(gPad->GetEventX() == 104){ // "h" pressed, set higher limit
          
           if(!hi){
//               hi = new TLine(GetXpos(),ymin+0.1*range,GetXpos(),ymax-0.1*range);
               hi = new TLine(xbinhiedge,ymin+0.1*range,xbinhiedge,ymax-0.1*range);
               hi->SetLineColor(4);
               hi->SetUniqueID(12345678);
               hi->Draw();
           }else{
               hi->SetX1(/*GetXpos()*/xbinhiedge);
               hi->SetY1(ymin+0.1*range);
               hi->SetX2(/*GetXpos()*/xbinhiedge);
               hi->SetY2(ymax-0.1*range);
           }   
           pad->Modified();
           pad->Update();
       }
       if(gPad->GetEventX() == 98){ // "b" pressed, set background low limit at
                                    // cursor position. Set bg upper limit based on
                                    // difference between lo and hi.
          
           if(!bg){
//               bg = new TLine(GetXpos(),ymin+0.1*range,GetXpos(),ymax-0.1*range);
               bg = new TLine(xbinloedge,ymin+0.1*range,xbinloedge,ymax-0.1*range);
               bg->SetLineColor(2);
               bg->SetUniqueID(12345679);
               bg->Draw();
           }else{
               bg->SetX1(/*GetXpos()*/xbinloedge);
               bg->SetY1(ymin+0.1*range);
               bg->SetX2(/*GetXpos()*/xbinloedge);
               bg->SetY2(ymax-0.1*range);
           }
           if(lo && hi){ //low and hi exist - display bg upper limit
               Double_t lo_x = lo->GetX1();
               Double_t hi_x = hi->GetX1();
               if(!bghi){
                   bghi = new TLine(xbinloedge+(hi_x-lo_x),ymin+0.1*range,
                                    xbinloedge+(hi_x-lo_x),ymax-0.1*range);
                   bghi->SetLineColor(2);
                   bghi->SetUniqueID(12345680);
                   bghi->Draw();
               }else{
                   bghi->SetX1(/*GetXpos()*/xbinloedge+(hi_x-lo_x));
                   bghi->SetY1(ymin+0.1*range);
                   bghi->SetX2(/*GetXpos()*/xbinloedge+(hi_x-lo_x));
                   bghi->SetY2(ymax-0.1*range);
               }
           }
           pad->Modified();
           pad->Update();
       }
       if(gPad->GetEventX() == 112){ // "p" pressed, project the spectrum

           Double_t xvallo, xvalhi, bgval;
           xvallo = xvalhi = bgval = -1;

           // find positions of the lines
           if(lo) xvallo = lo->GetX1();
           if(hi) xvalhi = hi->GetX1();
           if(bg) bgval = bg->GetX1();

           Int_t binxlo, binxhi, binbg;
           binxlo = fHist->GetXaxis()->FindBin(xvallo);
           binxhi = fHist->GetXaxis()->FindBin(xvalhi) - 1;
           binbg  = fHist->GetXaxis()->FindBin(bgval);

           if(xvalhi < xvallo) {
               cout << "high limit is lower than low limit !" << endl;
           }
           
           if(padname == "c1_1"){
               cout << "Projx " << endl;

               //find all relevant spectra
               TPad *temp_p2 = (TPad*)gROOT->FindObject("c1_2");
               TPad *temp_p4 = (TPad*)gROOT->FindObject("c1_4");
               TPad *temp_p5 = (TPad*)gROOT->FindObject("c1_5");
               TPad *temp_p6 = (TPad*)gROOT->FindObject("c1_6");
               TH1D *temp_h4, *temp_h5, *temp_h6;
               TH2F *temp_h2;
               //TObject *obj1;
               // find the second pad with x projection
               TIter next(temp_p2->GetListOfPrimitives());
               while(obj1=next()){
                   //cout << "reading " << obj1->GetName() << endl;
                   if(obj1->InheritsFrom("TH2")){
                       temp_h2 = (TH2F*)obj1;
                       //cout << "TH2 " << temp_h2->GetName() << endl;
                   }
               }
               // find the fourth pad with x projection
               TIter next(temp_p4->GetListOfPrimitives());
               while(obj1=next()){
                   //cout << "reading " << obj1->GetName() << endl;
                   if(obj1->InheritsFrom("TH1")){
                       temp_h4 = (TH1D*)obj1;
                       //cout << "TH1 from p4 " << temp_h4->GetName() << endl;
                   }
               }
               // find the fifth pad with x bg projection
               TIter next(temp_p5->GetListOfPrimitives());
               while(obj1=next()){
                   //cout << "reading " << obj1->GetName() << endl;
                   if(obj1->InheritsFrom("TH1")){
                       temp_h5 = (TH1D*)obj1;
                       //cout << "TH1 from p5 " << temp_h5->GetName() << endl;
                   }
               }
               // find the sixth pad with difference between x and bg
               TIter next(temp_p6->GetListOfPrimitives());
               while(obj1=next()){
                   //cout << "reading " << obj1->GetName() << endl;
                   if(obj1->InheritsFrom("TH1")){
                       temp_h6 = (TH1D*)obj1;
                       //cout << "TH1 from p6 " << temp_h6->GetName() << endl;
                   }
               }
                   
               if(xvallo != -1 && xvalhi != -1 && bgval != -1){
                   if(temp_h4){
                       temp_h4->Reset();
                       TH1D *tempproj = temp_h2->ProjectionX("",binxlo,binxhi);
                       temp_h4->Add(tempproj);
                       stringstream temptitle;
                       string title;
                       temptitle << "Projection onto X axis with a Y range of ";
                       temptitle << xvallo;
                       temptitle << " to ";
                       temptitle << xvalhi;
                       title = temptitle.str();
                       temp_h4->SetTitle(title.c_str());
                   } else {
                       cout << "fourth histogram not found " << endl;
                   }
                   if(temp_h5){
                       temp_h5->Reset();
                       TH1D *tempproj = temp_h2->ProjectionX("",binbg,binbg+(binxhi-binxlo));
                       temp_h5->Add(tempproj);
                       stringstream temptitle;
                       string title;
                       temptitle << "Bg projection onto X axis with a Y range of ";
                       temptitle << bgval;
                       temptitle << " to ";
                       temptitle << bgval+(xvalhi-xvallo);
                       title = temptitle.str();
                       temp_h5->SetTitle(title.c_str());
                   } else{
                       cout << "fifth histogram not found " << endl;
                   }
                   if(temp_h6){
                       if(binbg+(binxhi-binxlo) < temp_h2->GetNbinsX()){
                           temp_h6->Add(temp_h4,temp_h5,1,-1);
                           stringstream temptitle;
                           string title;
                           temptitle << "Projection onto X axis between";
                           temptitle << xvallo;
                           temptitle << " to ";
                           temptitle << xvalhi;
                           temptitle << ", Bg Subtracted";
                           title = temptitle.str();
                           temp_h6->SetTitle(title.c_str());
                       }
                       else{
                           temp_h6->Reset();
                           cout << "background limits exceed histogram range" << endl;
                       }
                           
                   } else {
                       cout << "sixth histogram not found " << endl;
                   }
                   
                   temp_p4->Modified();
                   temp_p4->Update();
                   temp_p5->Modified();
                   temp_p5->Update();
                   temp_p6->Modified();
                   temp_p6->Update();
               }
               else{
                   cout << "not all values are defined, doing nothing" << endl;
               }
           }
           if(padname == "c1_3"){
               cout << "Projy " << endl;

               //find all relevant spectra
               TPad *temp_p2 = (TPad*)gROOT->FindObject("c1_2");
               TPad *temp_p4 = (TPad*)gROOT->FindObject("c1_7");
               TPad *temp_p5 = (TPad*)gROOT->FindObject("c1_8");
               TPad *temp_p6 = (TPad*)gROOT->FindObject("c1_9");
               TH1D *temp_h4, *temp_h5, *temp_h6;
               TH2F *temp_h2;
               //TObject *obj1;
               // find the second pad with x projection
               TIter next(temp_p2->GetListOfPrimitives());
               while(obj1=next()){
                   //cout << "reading " << obj1->GetName() << endl;
                   if(obj1->InheritsFrom("TH2")){
                       temp_h2 = (TH2F*)obj1;
                       //cout << "TH2 " << temp_h2->GetName() << endl;
                   }
               }
               // find the fourth pad with x projection
               TIter next(temp_p4->GetListOfPrimitives());
               while(obj1=next()){
                   //cout << "reading " << obj1->GetName() << endl;
                   if(obj1->InheritsFrom("TH1")){
                       temp_h4 = (TH1D*)obj1;
                       //cout << "TH1 from p4 " << temp_h4->GetName() << endl;
                   }
               }
               // find the fifth pad with x bg projection
               TIter next(temp_p5->GetListOfPrimitives());
               while(obj1=next()){
                   //cout << "reading " << obj1->GetName() << endl;
                   if(obj1->InheritsFrom("TH1")){
                       temp_h5 = (TH1D*)obj1;
                       //cout << "TH1 from p5 " << temp_h5->GetName() << endl;
                   }
               }
               // find the sixth pad with difference between x and bg
               TIter next(temp_p6->GetListOfPrimitives());
               while(obj1=next()){
                   //cout << "reading " << obj1->GetName() << endl;
                   if(obj1->InheritsFrom("TH1")){
                       temp_h6 = (TH1D*)obj1;
                       //cout << "TH1 from p6 " << temp_h6->GetName() << endl;
                   }
               }
               if(xvallo != -1 && xvalhi != -1 && bgval != -1){
                   if(temp_h4){
                       temp_h4->Reset();
                       TH1D *tempproj = temp_h2->ProjectionY("",binxlo,binxhi);
                       temp_h4->Add(tempproj);
                       stringstream temptitle;
                       string title;
                       temptitle << "Projection onto Y axis with a X range of ";
                       temptitle << xvallo;
                       temptitle << " to ";
                       temptitle << xvalhi;
                       title = temptitle.str();
                       temp_h4->SetTitle(title.c_str());
                   } else {
                       cout << "fourth histogram not found " << endl;
                   }
                   if(temp_h5){
                       temp_h5->Reset();
                       TH1D *tempproj = temp_h2->ProjectionY("",binbg,binbg+(binxhi-binxlo));
                       temp_h5->Add(tempproj);
                       stringstream temptitle;
                       string title;
                       temptitle << "Bg projection onto Y axis with a X range of ";
                       temptitle << bgval;
                       temptitle << " to ";
                       temptitle << bgval+(xvalhi-xvallo);
                       title = temptitle.str();
                       temp_h5->SetTitle(title.c_str());
                   } else{
                       cout << "fifth histogram not found " << endl;
                   }
                   if(temp_h6){
                       if(binbg+(binxhi-binxlo) < temp_h2->GetNbinsX()){
                           temp_h6->Add(temp_h4,temp_h5,1,-1);
                           stringstream temptitle;
                           string title;
                           temptitle << "Projection onto Y axis between";
                           temptitle << xvallo;
                           temptitle << " to ";
                           temptitle << xvalhi;
                           temptitle << ", Bg Subtracted";
                           title = temptitle.str();
                           temp_h6->SetTitle(title.c_str());
                       }
                       else{
                           temp_h6->Reset();
                           cout << "background limits exceed histogram range" << endl;
                       }
                           
                   } else {
                       cout << "sixth histogram not found " << endl;
                   }
                   
                   temp_p4->Modified();
                   temp_p4->Update();
                   temp_p5->Modified();
                   temp_p5->Update();
                   temp_p6->Modified();
                   temp_p6->Update();
               }
               else{
                   cout << "not all values are defined, doing nothing" << endl;
               }


               
           }
           
       }
   }
   
//    if (event == kButton3Down) {
//       fHist->Reset();
//       pad->Modified();
//       pad->Update();
//    }
}

/////////////////////// functions ///////////////////////////////////
//___________________________________________________________________
void printEvent(Int_t event, Int_t px, Int_t py, TObject *)
{
   //  print event type and current cursor position

   TCanvas *c = (TCanvas *) gTQSender;
   TPad *pad = (TPad *) c->GetSelectedPad();
   if (!pad) return;
   printf("event=%d, px=%d, py=%d, ", event, px, py);
   Float_t x = pad->AbsPixeltoX(px);
   Float_t y = pad->AbsPixeltoY(py);
   x = pad->PadtoX(x);
   y = pad->PadtoY(y);
   printf("x=%.3g, y=%.3g\n",x,y);
}


//___________________________________________________________________
void rqfiller1(TH2 *hist2)
{
    
   TCanvas *c1 = new TCanvas("c1", "Gamma-Gamma projection canvas",
                              1000, 800);

   c1->Divide(3,3);
   TPad *p1 = new TPad();
   TPad *p2 = new TPad();
   TPad *p3 = new TPad();
   TPad *p4 = new TPad();
   TPad *p5 = new TPad();
   TPad *p6 = new TPad();
   TPad *p7 = new TPad();
   TPad *p8 = new TPad();
   TPad *p9 = new TPad();
   p1 = (TPad*)gROOT->FindObject("c1_1");
   p2 = (TPad*)gROOT->FindObject("c1_2");
   p3 = (TPad*)gROOT->FindObject("c1_3");
   p4 = (TPad*)gROOT->FindObject("c1_4");
   p5 = (TPad*)gROOT->FindObject("c1_5");
   p6 = (TPad*)gROOT->FindObject("c1_6");
   p7 = (TPad*)gROOT->FindObject("c1_7");
   p8 = (TPad*)gROOT->FindObject("c1_8");
   p9 = (TPad*)gROOT->FindObject("c1_9");

   //TPad *p1 = new TPad("p1","",0.01,0.05,0.49,0.95);
   //TPad *p2 = new TPad("p2","",0.51,0.05,0.99,0.95);

   //p1->Draw();
   //p2->Draw();

   //interactive(*c1);
   //interactive(*p1);
   //interactive(*p3);
   //interactive(*p4);
   //interactive(*p5);
   //interactive(*p6);
   //interactive(*p7);
   //interactive(*p8);
   //interactive(*p9);


   ////// create 2-dim histogram filler

   p2->cd();

   HFiller *h2 = new HFiller();

   //TH2F *hist2 = new TH2F("test2", "2-Dim hist filled manually",
   //                       100, 0, 100, 100, 0, 100);
   Double_t px,py;
//    hist2->Fill(20,80); hist2->Fill(80,80); hist2->Fill(40,40);
//    for(Int_t i=0;i<100;i++){
//        //gRandom->Rannor(px,py);
//        /*if(i%4==0)*/ hist2->Fill(i,TMath::Abs(Int_t(i/5)-5)+15);
//    }
   
   
   h2->SetPad(p2);
   h2->SetHist(hist2);
   h2->Draw("col");

   // connect signal to histogam filling method
   c1->Connect("ProcessedEvent(Int_t,Int_t,Int_t,TObject*)", "HFiller",
               h2, "ExecuteEvent(Int_t,Int_t,Int_t,TObject*)");

   ////// create 1-dim histogram filler

   Double_t binxmin = hist2->GetXaxis()->GetXmin();
   Double_t binxmax = hist2->GetXaxis()->GetXmax();
   Double_t binymin = hist2->GetYaxis()->GetXmin();
   Double_t binymax = hist2->GetYaxis()->GetXmax();

   Int_t binx = hist2->GetNbinsX();
   Int_t biny = hist2->GetNbinsY();

   
   p1->cd();
   HFiller *h1 = new HFiller();
   TH1D *hist1 = new TH1D("total projy","title",hist2->GetNbinsY(),binymin,binymax);
   hist1->SetTitle("Total Projection onto the Y axis");
   TH1D *tempproj = hist2->ProjectionY("",1,binx);
   hist1->Add(tempproj);
   delete tempproj; //finished with tempproj - delete it
   
   h1->SetPad(p1);
   h1->SetHist(hist1);
   h1->GetHist()->SetFillColor(16);
   h1->Draw();

   // connect signal to histogam filling method
   c1->Connect("ProcessedEvent(Int_t,Int_t,Int_t,TObject*)", "HFiller",
               h1, "ExecuteEventProj(Int_t,Int_t,Int_t,TObject*)");

   
   p3->cd();
   HFiller *h3 = new HFiller();
   TH1D *hist3 = new TH1D("total projx","title",hist2->GetNbinsX(),binxmin,binxmax);
   hist3->SetTitle("Total Projection onto the X axis");
   TH1D *tempproj = hist2->ProjectionX("",1,biny);
   cout << "from tempproj " << tempproj->GetNbinsX() << " "
        << tempproj->GetXaxis()->GetXmin() << " " << tempproj->GetXaxis()->GetXmax()<< endl;
   cout << "from hist3 " << hist3->GetNbinsX() << " "
        << hist3->GetXaxis()->GetXmin() << " " << hist3->GetXaxis()->GetXmax()<< endl;
   hist3->Add(tempproj);
   cout << "adding 1" << endl;
   delete tempproj; //finished with tempproj - delete it
   
   h3->SetPad(p3);
   h3->SetHist(hist3);
   h3->Draw();

   c1->Connect("ProcessedEvent(Int_t,Int_t,Int_t,TObject*)","HFiller",
               h3, "ExecuteEventProj(Int_t,Int_t,Int_t,TObject*)");

   p4->cd();
   HFiller *h4 = new HFiller();
   TH1D *hist4 = new TH1D("projx_cut","title",hist2->GetNbinsX(),binxmin,binxmax);
   hist4->SetTitle("Projection onto X axis with a Y range of - to -");
   TH1D *tempproj = hist2->ProjectionX("",1,biny);
   hist4->Add(tempproj);
   delete tempproj; //finished with tempproj - delete it
   
   h4->SetPad(p4);
   h4->SetHist(hist4);
   h4->Draw();

   p5->cd();
   HFiller *h5 = new HFiller();
   TH1D *hist5 = new TH1D("projx_bg","title",hist2->GetNbinsX(),binxmin,binxmax);
   //hist5->SetName("projx_bg_p5");
   hist5->SetTitle("Bg projection onto X axis with a Y range of - to -");
   TH1D *tempproj = hist2->ProjectionX("",1,biny);
   hist5->Add(tempproj);
   delete tempproj; //finished with tempproj - delete it
   
   h5->SetPad(p5);
   h5->SetHist(hist5);
   h5->Draw();

   p6->cd();
   HFiller *h6 = new HFiller();
   TH1D *hist6 = new TH1D("projx_diff","title",hist2->GetNbinsX(),binxmin,binxmax);
   hist6->SetTitle("Projection onto X axis between - to -, Bg Subtracted ");
   hist6->Add(hist4,hist5,1,-1);
   delete tempproj; //finished with tempproj - delete it
   
   h6->SetPad(p6);
   h6->SetHist(hist6);
   h6->Draw();


   p7->cd();
   HFiller *h7 = new HFiller();
   TH1D *hist7 = new TH1D("projy_cut","title",hist2->GetNbinsY(),binymin,binymax);
   hist7->SetTitle("Projection onto Y axis with a X range of - to -");
   TH1D *tempproj = hist2->ProjectionY("",1,binx);
   hist7->Add(tempproj);
   delete tempproj; //finished with tempproj - delete it
   
   h7->SetPad(p7);
   h7->SetHist(hist7);
   h7->Draw();

   p8->cd();
   HFiller *h8 = new HFiller();
   TH1D *hist8 = new TH1D("projy_bg","title",hist2->GetNbinsY(),binymin,binymax);
   hist8->SetTitle("Bg projection onto Y axis with a X range of - to -");
   TH1D *tempproj = hist2->ProjectionY("",1,binx);
   hist8->Add(tempproj);
   delete tempproj; //finished with tempproj - delete it
   
   h8->SetPad(p8);
   h8->SetHist(hist8);
   h8->Draw();

   p9->cd();
   HFiller *h9 = new HFiller();
   TH1D *hist9 = new TH1D("projy_diff","title",hist2->GetNbinsY(),binymin,binymax);
   hist9->SetTitle("Projection onto Y axis between - to -,Bg Substracted");
   TH1D *tempproj = hist2->ProjectionY("",1,binx);
   hist9->Add(hist7,hist8,1,-1);
   delete tempproj; //finished with tempproj - delete it
   
   h9->SetPad(p9);
   h9->SetHist(hist9);
   h9->Draw();

   p1->cd();
}


void createhis(){
    TH2F *h2d = new TH2F("h2d","addback gamma gamma",1000,0,1000,1000,0,1000);

    h2d->Fill(200,800); h2d->Fill(800,800); h2d->Fill(400,400);
    //h2d->SetMarkerStyle(21);
    for(Int_t i=0;i<1000;i++){
        //gRandom->Rannor(px,py);
        /*if(i%4==0)*/ h2d->Fill(i,TMath::Abs(Int_t(i/5)-50)+150);
    }
    h2d->Draw();
}
