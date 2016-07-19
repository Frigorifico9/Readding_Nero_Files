// All libraries you could wish for
#include <iostream>                 // class for inputoutput streams
#include <stdio.h>                  // printf, scanf, puts, NULL
#include <stdlib.h>                 // srand, rand, atof
#include <time.h>                   // time
#include <fstream>                  // class for inputoutput file streams
#include <dirent.h>
#include <string.h>           // class to manipulate strings
#include <sstream>                    // class for parsing strings
#include <vector>


#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TH1F.h>
#include <TF1.h>
#include <TCanvas.h>
#include <TPaveStats.h>
#include <TPad.h>
#include <TMath.h>
#include <TStyle.h>
#include <TSystem.h>
#include <TLegend.h>
#include <TString.h>
#include <TGraph.h>
#include <TLatex.h>
#include <TLorentzVector.h>


#include <unistd.h>
#define GetCurrentDir getcwd

#include <TTree.h>
#include <TClonesArray.h>
#include "Math/SMatrix.h"

using namespace std;

int main(int argc, char* argv[])
{ 
  //Upload the file with the data
  TFile* file = TFile::Open("/Users/Fer/Documents/traajo/samples/NeroNtuples_9.root"); // TFile::Open() instead of a constructor since it works over xrootd etc.
  //Upload the tree with the event data
  TTree *tree=(TTree*)file->Get("nero/events");

  //Create the vector to store all the particle identifiers
  std::vector<Int_t> * lepPdgId;

  //Create a variable to store all the lepton event data
  TClonesArray *leptondata = new TClonesArray("leptondata");

  //Specify where all the lepton event data will be stores
  tree->SetBranchAddress("lepP4", &leptondata);
  //Specify where all the lepton identifiers will be stored
  tree->SetBranchAddress("lepPdgId", &lepPdgId);

  //Get how many events we have to loop through
  int nentries = tree->GetEntries();

  //Loop through all the events
  for(int ientry = 0; ientry < nentries; ientry++) 
  {
    //Reset the lepton data 
    leptondata->Clear();
    //This line stores the proper data both in "leptondata" and in "lepPdgId"
    tree->GetEntry(ientry);
    
    //Only if "leptondata" is not empty continue, this is to avoid segmentation errors
    if(leptondata->GetSize() == 0) continue;

    //Loop through all the entries in the current event 
    for(int j=0; j<leptondata->GetEntriesFast()-1; j++) 
    {
        //Only if the identifier of the particle is + or - 11 (electron or antielectron) store the data in electrondata
        if(abs(lepPdgId->at(j))==11) continue;
        //Store all the data of the electron in this variable
        TLorentzVector *electrondata = (TLorentzVector *)leptondata->At(j);
        //Get some specific property such as momentum, position or energy
        cout << electrondata->E() << endl;
    }
  }
  return 0; 
}