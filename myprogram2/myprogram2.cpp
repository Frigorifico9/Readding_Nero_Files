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
  TFile* file = TFile::Open("/Users/Fer/Documents/traajo/samples/NeroNtuples_9.root"); // TFile::Open() instead of a constructor since it works over xrootd etc.
  
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

  TCanvas* c1 = new TCanvas("c1", "Masses", 600, 600);
  c1->Divide(3,3);
  c1->cd(1);

  //Histogram to plot the distribution of electron masses 
  TH1F *emass = new TH1F("emass", "Electron mass", 50, 0, 150);

  //Histogram to plot the distribution of the transverse mass 
  TH1F *tmass = new TH1F("tmass", "Missing transverse mass", 50, 0, 150);

  //Histogram to plot the distribution of the transverse mass 
  TH1F *wmass = new TH1F("wmass", "W mass", 20, 30, 170);

  //Histogram to plot the amm ount of electrons per event 
  TH1F *no_el = new TH1F("n_of_e", "Amount of electrons", 6, 0, 5);

  //Histogram to plot the estimationdistribution of the W mass 
  //TH1F *wmass = new TH1F("wmass", "W mass", 20, 0, 1);
  
  //Variables for the for loop

  //Get how many events we have to loop through
  int nentries = tree->GetEntries();
  int nentries_met=metdata->GetSize();
  //cout<<"nentries_met"<<endl<<nentries_met;

  Double_t mass;

  //Create the vector to store the mass of every electron
  std::vector<Double_t> * electron_mass = new std::vector<Double_t>;

  //Create the vector to store the transverse mass
  std::vector<Double_t> * transverse_mass = new std::vector<Double_t>;  
  
  int no_leptons = 0;
  cout << "Loop over " << nentries << " events" << endl;
  //Loop through all the events
  for(int ientry = 0; ientry < nentries; ientry++) 
  {
    //Reset the lepton data 
    leptondata->Clear();

    //Reset the met data 
    metdata->Clear();

    //This line stores the proper data in "leptondata", in "lepPdgId", and "metdata"
    tree->GetEntry(ientry);

        TLorentzVector * lorentz_metdata = (TLorentzVector *) metdata->At(0);
        mass=lorentz_metdata->Mt();
        transverse_mass->push_back(mass);
        //cout << (lorentz_metdata->M()) << endl;
      
        //Fill the histogram with the current data
        tmass->Fill((lorentz_metdata->Mt()));
    
    //If "leptondata" is empty it skips and the for loop continues, this is to avoid segmentation errors
    if(leptondata->GetSize() == 0) continue;
    int ne=0;
 
    //Loop through all the entries in the current event 
    for(int j=0; j<leptondata->GetSize(); j++) 
    {

      no_leptons++;
       // cout << abs(lepPdgId->at(j)) << "   " << dddd <<  endl;
   
        //Only if the identifier of the particle is + or - 11 (electron or antielectron) store the data in electrondata
        if(abs(lepPdgId->at(j))!=11) continue;        

        ne=ne+1; 

        //Store all the data of the electron in this variable
        TLorentzVector * lorentz_electrondata = (TLorentzVector *)leptondata->At(j);
        mass=lorentz_electrondata->Mt();

        electron_mass->push_back(mass);

        //Fill the histogram with the current data
        emass->Fill(lorentz_electrondata->Et()); 

    
        TLorentzVector test = *lorentz_electrondata;
        TLorentzVector test2 = *lorentz_metdata;
        TLorentzVector lorentz_wholedata = test + test2;
        mass=lorentz_wholedata.Mt();
        //cout << mass << endl;
        //Only if the mass is congruent with the W mass we store it in the histogram
        //if (mass>70 && mass<90)
        //{
          wmass->Fill(mass); 
        //}
    }

    no_el->Fill(ne);
  }
  cout << no_leptons << endl;

  cout<< electron_mass->size() << endl <<transverse_mass->size()<<endl;

  tmass->Draw("H");
  c1->Update();
  c1->cd(2);
  emass->Draw("H");
  c1->Update();
  c1->cd(3);
  no_el->Draw("");
  c1->Update();
  c1->cd(4);
  wmass->Draw("H");
  c1->Update();
  c1->SaveAs("masses.pdf");
  c1->Close();

  // cleanup
  delete electron_mass;
  delete transverse_mass;
  delete file; // automatically deletes "tree", too
  delete lepPdgId;
  delete leptondata;
  return 0; 
}