{

  new TCanvas();

  //plot dpid
  proof->DrawSelect(tdset,"corr.dde1:corr.dtof>>hdpid(1000,0,100000,1000,0,100000)","","colz");

  TCanvas *c9 = new TCanvas();
  c9->Divide(5,5);
  c9->cd(1);  
  proof->DrawSelect(tdset,"sega.ecal>>h10(8000,0,8000)","z1a1");

  c9->cd(6);
  proof->DrawSelect(tdset,"sega.ecal>>h11(8000,0,8000)","z2a1");  
  c9->cd(7);
  proof->DrawSelect(tdset,"sega.ecal>>h12(8000,0,8000)","z2a2");  
  c9->cd(8);
  proof->DrawSelect(tdset,"sega.ecal>>h13(8000,0,8000)","z2a3");  
  c9->cd(9);
  proof->DrawSelect(tdset,"sega.ecal>>h14(8000,0,8000)","z2a4");
  
  c9->cd(11);
  proof->DrawSelect(tdset,"sega.ecal>>h15(8000,0,8000)","z3a1");  
  c9->cd(12);
  proof->DrawSelect(tdset,"sega.ecal>>h16(8000,0,8000)","z3a2");  
  c9->cd(13);
  proof->DrawSelect(tdset,"sega.ecal>>h17(8000,0,8000)","z3a3");  
  c9->cd(14);
  proof->DrawSelect(tdset,"sega.ecal>>h18(8000,0,8000)","z3a4");
  
  c9->cd(16);
  proof->DrawSelect(tdset,"sega.ecal>>h19(8000,0,8000)","z4a1");  
  c9->cd(17);
  proof->DrawSelect(tdset,"sega.ecal>>20(8000,0,8000)","z4a2");  
  c9->cd(18);
  proof->DrawSelect(tdset,"sega.ecal>>h21(8000,0,8000)","z4a3");  
  c9->cd(19);
  proof->DrawSelect(tdset,"sega.ecal>>h22(8000,0,8000)","z4a4");  
  c9->cd(20);
  proof->DrawSelect(tdset,"sega.ecal>>h23(8000,0,8000)","z4a5");  

  c9->cd(21);
  proof->DrawSelect(tdset,"sega.ecal>>h24(8000,0,8000)","z5a1");  
  c9->cd(22);
  proof->DrawSelect(tdset,"sega.ecal>>h25(8000,0,8000)","z5a2");  
  c9->cd(23);
  proof->DrawSelect(tdset,"sega.ecal>>h26(8000,0,8000)","z5a3");  
  c9->cd(24);
  proof->DrawSelect(tdset,"sega.ecal>>h27(8000,0,8000)","z5a4");  
  c9->cd(25);
  proof->DrawSelect(tdset,"sega.ecal>>h28(8000,0,8000)","z5a5");  

}
