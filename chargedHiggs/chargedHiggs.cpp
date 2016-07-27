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

  //Upload the file with the data, make sure the adress of the file matches the one in your computer
  TFile* file = TFile::Open("/Users/Fer/Documents/traajo/samples/NeroNtuples_9.root"); // TFile::Open() instead of a constructor since it works over xrootd etc. =D
  
  //Upload the tree with the event data
  TTree *tree=(TTree*)file->Get("nero/events");

/////////////////////////////////////////////////////

  //Lepton criteria

  //Create a variable to store all the lepton event data
  TClonesArray *leptondata = new TClonesArray("leptondata");

  //Create the vector to store all the particle identifiers
  std::vector<Int_t> * lepPdgId= 0;

  //Specify where all the lepton event data will be stored
  tree->SetBranchAddress("lepP4", &leptondata);

  //Specify where all the lepton identifiers will be stored
  tree->SetBranchAddress("lepPdgId", &lepPdgId);

  //Histogram to plot the distribution of lepton mass 
  TH1F *lepmass = new TH1F("lepmass", "Lepton mass", 50, 0, 150);

/////////////////////////////////////////////////////

  //MET criteria

  //Create a variable to store all the "met" data
  TClonesArray *metdata = new TClonesArray("metdata");

  //Specify where all the "met" data will be stored
  tree->SetBranchAddress("metP4", &metdata);

  //Histogram to plot the distribution of the transverse mass 
  TH1F *metmass = new TH1F("metmass", "Missing transverse mass", 50, 0, 150);

/////////////////////////////////////////////////////

  //Eta criteria

  //Create the variable for Eta
  Double_t eta;

/////////////////////////////////////////////////////

  //Jet criteria

  //Create a variable to store all the jet event data
  TClonesArray *jetdata = new TClonesArray("jetdata");

  //Specify where all the jet event data will be stored
  tree->SetBranchAddress("jetP4", &jetdata);

  //Histogram to plot the distribution of jet mass 
  TH1F *jetmass = new TH1F("jetmass", "Jet mass", 50, 0, 150);

  //Variable to store the amount of jets
  Double_t size;

/////////////////////////////////////////////////////

  //Histogram to plot the distribution of the whole mass 
  TH1F *wholemass = new TH1F("wholemass", "Whole mass", 50, 0, 150);  

/////////////////////////////////////////////////////

  //Histogram variables

  //Create the canvas were the histograms will be ploted
  TCanvas* c1 = new TCanvas("c1", "Masses", 600, 600);

  //Divide that canvas to plot all histograms together
  c1->Divide(2,2);

/////////////////////////////////////////////////////

  //Variables for the for loop

  //Get how many events we have to loop through
  int nentries = tree->GetEntries();

  //Create a variable to store the mass values
  Double_t mass;

  //Loop through all the events
  for(int ientry = 0; ientry < nentries; ientry++) 
  {

    //Variable of the whole data
    TLorentzVector addable_lorentz_wholedata;

    //Reset the lepton data 
    leptondata->Clear();

    //Reset the met data 
    metdata->Clear();

    //Reset the jet data 
    jetdata->Clear();

    //This line stores the proper data in the variables qe specified
    tree->GetEntry(ientry);

/////////////////////////////////////////////////////

//Implementation of lepton criteria 

    //If "leptondata" is empty it skips and the for loop continues, this is to avoid segmentation errors
    if(leptondata->GetSize() != 1) continue;

      //Only if the identifier of the particle is ±11 or ± 13 (electrons muons or their anti particles) we use the data
      if((abs(lepPdgId->at(0))!=11)&&(abs(lepPdgId->at(0))!=13)) continue; 

      //Store all the data of the electron in this lorentz vector
      TLorentzVector * lorentz_leptondata = (TLorentzVector *)leptondata->At(0);

      //We create another lorentz vector that isn't a pointer for the same reasons that before
      TLorentzVector addable_lorentz_leptondata = *lorentz_leptondata;

      //Get the Eta value of that Lorentz vector
      eta=addable_lorentz_leptondata.Eta();

      //Get the transverse mass of that lorentz vector
      mass=addable_lorentz_leptondata.Mt();

      //Implementing slection criteria
      if (mass<40 || abs(eta)>2.5) continue;

      //Fill the histogram with the current data
      lepmass->Fill(mass);

/////////////////////////////////////////////////////
    
//Implementation of met criteria 

    //Create a lorentz vector with the matdata of the current entry
    TLorentzVector * lorentz_metdata = (TLorentzVector *) metdata->At(0);

    //We cannot use math with pointers for some reason, so we create a lorentz vectors that isn't a pointers
    TLorentzVector addable_lorentz_metdata = *lorentz_metdata;

    //Get the invariant transverse mass of that lorentz vector
    mass=addable_lorentz_metdata.Mt();
    
    //Implementing slection criteria
    if (mass<60) continue;
  
    metmass->Fill(mass);

/////////////////////////////////////////////////////
    
//Implementation of jet criteria 

    //If "jetdata" is empty it skips and the for loop continues, this is to avoid segmentation errors
    if(jetdata->GetSize() == 0) continue;

    size=jetdata->GetSize();

    if (size != 3) continue; 

      for (int i = 0; i < jetdata->GetSize()-1; ++i)
      {
      
        //Store all the data of the electron in this lorentz vector
        TLorentzVector * lorentz_jetdata = (TLorentzVector *)jetdata->At(i);

        //We create another lorentz vector that isn't a pointer for the same reasons that before
        TLorentzVector addable_lorentz_jetdata = *lorentz_jetdata;

        //Get the Eta value of that Lorentz vector
        eta=addable_lorentz_jetdata.Eta();

        //Get the transverse mass of that lorentz vector
        mass=addable_lorentz_jetdata.Mt();

        //Implementing slection criteria
        if (mass<30 || abs(eta)>2.4) continue;

        //Fill the histogram with the current data
        jetmass->Fill(mass);

        //We add the mass of each jet
        addable_lorentz_wholedata= addable_lorentz_jetdata+addable_lorentz_wholedata;
      }

      //Finally we add the MET mass and the lepton mass
      addable_lorentz_wholedata=addable_lorentz_metdata+addable_lorentz_leptondata;

      //Get the transverse mass of that lorentz vector
      mass=addable_lorentz_wholedata.Mt();

      //Fill the histogram with the current data
      wholemass->Fill(mass);
  }

  //Activate the first section of the canvas
  c1->cd(1);

  //Make the histogram
  metmass->Draw("H");

  //Put it in the canvas
  c1->Update();

  //Repeat
  c1->cd(2);
  lepmass->Draw("H");
  c1->Update();

  c1->cd(3);
  jetmass->Draw("H");
  c1->Update();

  c1->cd(4);
  wholemass->Draw("H");
  c1->Update();

  //Save the image
  c1->SaveAs("chargedHiggs_masses.pdf");
  c1->Close();

  // cleanup
  delete file; // automatically deletes "tree" too
  delete lepPdgId;
  delete leptondata;
  delete metdata;
  delete jetdata;
  return 0; 
}