void ProcessList(){

  TFile *fe = new TFile("elistdouble.root");

  TEntryList *elist = (TEntryList*)fe->Get("elistdouble");


  //get the list of entrylist objects
  TList *list = (TList*) elist->GetLists();

  // number of entry lists
  int entries = list->GetEntries();
  cout << "Number of entries in list: " << entries << endl;

  //loop over all entries
  for(int i=0; i<entries; i++){

    //retrieve list for this file
    TEntryList *file_elist = (TEntryList*)list->At(i);

    //check if the list for this file has entries
    if(file_elist->GetN() >0){
      cout << "Events in file: " << file_elist->GetFileName() << endl;
    }

    
  }


}
