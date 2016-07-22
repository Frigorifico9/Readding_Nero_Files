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
  //Essentials

  //Upload the file with the data
  TFile* file = TFile::Open("/Users/Fer/Documents/traajo/samples/NeroNtuples_9.root"); // TFile::Open() instead of a constructor since it works over xrootd etc. =D
  
  //Upload the tree with the event data
  TTree *tree=(TTree*)file->Get("nero/events");

  //Create a variable to store all the lepton event data
  TClonesArray *leptondata = new TClonesArray("leptondata");

  //Create the vector to store all the particle identifiers
  std::vector<Int_t> * lepPdgId= 0;

  //Create a variable to store all the "met" data
  TClonesArray *metdata = new TClonesArray("metdata");

  //Specify where all the lepton event data will be stored
  tree->SetBranchAddress("lepP4", &leptondata);

  //Specify where all the lepton identifiers will be stored
  tree->SetBranchAddress("lepPdgId", &lepPdgId);

  //Specify where all the "met" data will be stored
  tree->SetBranchAddress("metP4", &metdata);


  //Histogram variables

  //Create the canvas were the histograms will be ploted
  TCanvas* c1 = new TCanvas("c1", "Masses", 600, 600);

  //Divide that canvas to plot all histograms together
  c1->Divide(2,2);

  //Histogram to plot the distribution of electron mass 
  TH1F *emass = new TH1F("emass", "Electron mass", 50, 0, 150);

  //Histogram to plot the distribution of electron transverse mass 
  TH1F *etmass = new TH1F("etmass", "Electron transverse momentum", 50, 0, 150);

  //Histogram to plot the distribution of the transverse mass 
  TH1F *tmass = new TH1F("tmass", "Missing transverse mass", 50, 0, 150);

  //Histogram to plot the distribution of the W mass  
  TH1F *wmass = new TH1F("wmass", "W mass", 20, 30, 170);

  //Variables for the for loop

  //Get how many events we have to loop through
  int nentries = tree->GetEntries();

  //Create a variable to store the mass values
  Double_t mass;

  //Loop through all the events
  for(int ientry = 0; ientry < nentries; ientry++) 
  {
    //Reset the lepton data 
    leptondata->Clear();

    //Reset the met data 
    metdata->Clear();

    //This line stores the proper data in "leptondata", in "lepPdgId", and "metdata"
    tree->GetEntry(ientry);

    //Create a lorentz vector with the matdata of the current entry
    TLorentzVector * lorentz_metdata = (TLorentzVector *) metdata->At(0);

    //We cannot use math with pointers for some reason, so we create a lorentz vectors that isn't a pointers
    TLorentzVector addable_lorentz_metdata = *lorentz_metdata;

    //Get the invariant transverse mass of that lorentz vector
    mass=addable_lorentz_metdata.Mt();

    //Note: There's a ->M() method we could use, but the values given by ->Mt() seem more reasonable
      
    //Fill the histogram with the current data
    tmass->Fill(mass);
    
    //If "leptondata" is empty it skips and the for loop continues, this is to avoid segmentation errors
    if(leptondata->GetSize() == 0) continue;
 
    //Loop through all the entries in the current event 
    for(int j=0; j<leptondata->GetSize(); j++) 
    {
      //Only if the identifier of the particle is + or - 11 (electron or antielectron) store the data in electrondata
      if(abs(lepPdgId->at(j))!=11) continue;        

        //Store all the data of the electron in this lorentz vector
        TLorentzVector * lorentz_electrondata = (TLorentzVector *)leptondata->At(j);

        //We create another lorentz vector that isn't a pointer for the same reasons that before
        TLorentzVector addable_lorentz_electrondata = *lorentz_electrondata;

        //Get the transverse mass of that lorentz vector
        mass=addable_lorentz_electrondata.Mt();

        //Fill the histogram with the current data
        etmass->Fill(mass);

        //We also want the invariant electron mass, which should be neglegible
        mass=addable_lorentz_electrondata.M();

        //Fill the histogram with the current data
        emass->Fill(mass); 

        //And then we add them up
        TLorentzVector lorentz_wholedata = addable_lorentz_electrondata + addable_lorentz_metdata;
        
        //Get the transverse mass of the resulting lorentz vector
        mass=lorentz_wholedata.Mt();

        //Fill the histogram with the current data
        wmass->Fill(mass); 
    }
  }

  //Activate the first section of the canvas
  c1->cd(1);

  //Make the histogram
  tmass->Draw("H");

  //Put it in the canvas
  c1->Update();

  //Repeat
  c1->cd(2);
  etmass->Draw("H");
  c1->Update();
  c1->cd(3);
  emass->Draw("H");
  c1->Update();
  c1->cd(4);
  wmass->Draw("H");
  c1->Update();

  //Save the image
  c1->SaveAs("masses.pdf");
  c1->Close();

  // cleanup
  delete file; // automatically deletes "tree" too
  delete lepPdgId;
  delete leptondata;
  delete metdata;
  return 0; 
}
