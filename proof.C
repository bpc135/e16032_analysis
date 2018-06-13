{
  gROOT->ProcessLine(".L Chain.C");
  TChain *data = new TChain("data");
  Chain(data,43);
  TDSet *tdset = new TDSet(*data);
  TProof *proof = TProof::Open("");

}
