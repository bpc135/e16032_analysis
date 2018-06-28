

void calibrate_anodes() {


  double depth[16] = {15,15,15,16,16,16,16,16,16,16,16,16,16,15,15,15};
  double mean[257];
  double gain[257];
  double desired_mean = 4000.0;
  
  TFile *fin = new TFile("anodes.root");

 
  TH2F *hanodes = (TH2F*)fin->Get("h25");

  hanodes->Draw("colz");


  TH1F *hanode_proj[257];

  TCanvas *c1 = new TCanvas("display","display");

  for(int eye=1; eye<257; eye++) {

    hanode_proj[eye] = (TH1F*)hanodes->ProjectionX(Form("anode_%d",eye),eye+1,eye+1);
    hanode_proj[eye]->SetName(Form("anode_%d",eye));

      hanode_proj[eye]->GetXaxis()->SetRangeUser(40,0.975*TMath::Power(2,depth[(int)(eye-1)/16]));

    c1->cd();
    hanode_proj[eye]->Draw();
    gPad->SetLogy();

    c1->Modified();
    c1->Update();

    mean[eye] = hanode_proj[eye]->GetMean();

    gain[eye] = desired_mean/mean[eye]; 
    cout<<eye<<"  "<<mean[eye]<<"  "<<gain[eye]<<endl;

    usleep(125000);
    
  }

  for(int eye=1; eye<257; eye++) {
    cout<<eye<<"  "<<gain[eye]<<endl;    
  }
  
  


  

}
