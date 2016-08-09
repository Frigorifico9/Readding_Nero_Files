#include "Analysis_Libraries.hh"
#include "nero_07.hh"

#define minintu nero_07

minintu *d;

int main(int argc, char* argv[])
{ 
  //Essentials

  //Upload the file with the data, make sure the adress of the file matches the one in your computer
  TFile* file = TFile::Open("/Users/Fer/Documents/traajo/samples/NeroNtuples_9.root"); // TFile::Open() instead of a constructor since it works over xrootd etc. =D
  
  //Upload the tree with the event data
  TTree *tree=(TTree*)file->Get("nero/events");

    //Variables for the for loop

    //Get how many events we have to loop through
    int nentries = tree->GetEntries();
    //int nentries=50;
    //Create a variable to store the mass values
    Double_t mass;


/////////////////////////////////////////////////////

  //Lepton criteria

  //Create a variable to store all the lepton event data
  TClonesArray *leptondata = new TClonesArray::TClonesArray ( "TLorentzVector", nentries);

  //Create the vector to store all the particle identifiers
  std::vector<Int_t> * lepPdgId= 0;

  //Specify where all the lepton event data will be stored
  tree->SetBranchAddress("lepP4", &leptondata);

  //Specify where all the lepton identifiers will be stored
  tree->SetBranchAddress("lepPdgId", &lepPdgId);

  
/////////////////////////////////////////////////////

  //MET criteria

  //Create a variable to store all the "met" data
  TClonesArray *metdata = new TClonesArray::TClonesArray ( "TLorentzVector", nentries);

  //Specify where all the "met" data will be stored
  tree->SetBranchAddress("metP4", &metdata);

/////////////////////////////////////////////////////

  //Eta criteria

  //Create the variable for Eta
  Double_t eta;

/////////////////////////////////////////////////////

  //Jet criteria

  //Create a variable to store all the jet event data
  TClonesArray *jetdata = new TClonesArray::TClonesArray ( "TLorentzVector", nentries);

  //Specify where all the jet event data will be stored
  tree->SetBranchAddress("jetP4", &jetdata);


/////////////////////////////////////////////////////

  //All the different histograms

  //Lepton histogram
  TH1F *lepmass = new TH1F("lepmass", "Lepton transverse mass", 50, 0, 150);

  //W bosson histogram
  TH1F *wmass = new TH1F("wmass", "W mass", 50, 0, 150);

  //MET histogram
  TH1F *metmass = new TH1F("metmass", "Missing transverse energy (MET)", 50, 0, 150);

  //Jet histogram
  TH1F *jetmass = new TH1F("jetmass", "Jet transverse mass", 50, 0, 150);

  //Histogram to plot the distribution of the whole mass 
  TH1F *wholemass = new TH1F("wholemass", "Whole mass", 50, 0, 150); 


/////////////////////////////////////////////////////

  //Histogram variables

  //Create the canvas for the no criteria
  TCanvas* c = new TCanvas("c", "No criteria", 600, 600);

  //Divide that canvas to plot all histograms together
  c->Divide(3,3);

  //Create the canvas for one criteria
  TCanvas* c1 = new TCanvas("c1", "One criteria", 600, 600);

  //Divide that canvas to plot all histograms together
  c1->Divide(3,3);

  //Create the canvas for one criteria
  TCanvas* c2 = new TCanvas("c2", "Two criteria", 600, 600);

  //Divide that canvas to plot all histograms together
  c2->Divide(3,3);

  //Create the canvas for one criteria
  TCanvas* c3 = new TCanvas("c3", "Two criteria", 600, 600);

  //Divide that canvas to plot all histograms together
  c3->Divide(3,3);

  //Create the canvas for one criteria
  TCanvas* c4 = new TCanvas("c4", "Two criteria", 600, 600);

  //Divide that canvas to plot all histograms together
  c4->Divide(3,3);

//Graficar masa W con lepton mas met
//sumar todas las Et
//omitir creación de Higgs pesado en presentación
//Normalizar las graficas y graficarlas juntas
//poner gráficas en la presentación, de Jan y mías
//Implementando un criterio a la vez graficar, y also sin criterios: pt lep, met et, Jets et, lep+jet+met et, lep+met=w->Mt
//El fondo es Ntuples_9

//Each for loop uses different criteria

    //Variables inside loops only exist inside the loops
    //So we need to declare all the lorentz vectors here to use them wherever we want
    TLorentzVector addable_lorentz_wdata;
    TLorentzVector addable_lorentz_jetdata;
    TLorentzVector addable_lorentz_leptondata;
    TLorentzVector addable_lorentz_metdata;
    TLorentzVector addable_total_lorentz_leptondata;
    TLorentzVector addable_total_lorentz_jetdata;
    TLorentzVector addable_lorentz_wholedata;
    TLorentzVector addable_lorentz_empty;


////00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000

  //No criteria, uses all events
  for(int ientry = 0; ientry < nentries-1; ientry++) 
  {
    //Reset the data
    leptondata->Clear();
    metdata->Clear();
    jetdata->Clear();

    //This line stores the proper data in the TClonesArrays
    tree->GetEntry(ientry);
/////////////////////////////////////////////////////

  //Met

    //Create a lorentz vector with the metdata of the current entry (it's size is always one)
    TLorentzVector * lorentz_metdata = (TLorentzVector *) metdata->At(0);

    //It's troublesome to do math with pointers
    addable_lorentz_metdata = *lorentz_metdata;

    //Get the invariant transverse energy of that lorentz vector
    mass=addable_lorentz_metdata.Et();
  
    //Fill the histogram with the current data
    metmass->Fill(mass);

/////////////////////////////////////////////////////

  //Leptons

    //Use only non empty events to avoid segmentation errors
    if(leptondata->GetSize() != 0)
    {

      //Loop through all the leptons
      for (int i = 0; i < leptondata->GetSize()-1; ++i)
      {
        //Store all the data of the electron in this lorentz vector
        TLorentzVector * lorentz_leptondata = (TLorentzVector *)leptondata->At(i);

        //It's troublesome to do math with pointers, so we create this non pointer variable 
        addable_lorentz_leptondata = *lorentz_leptondata;

///////// Im working on a way to change the previous two lines into a single one

        //Get the transverse momentum of that lorentz vector
        mass=addable_lorentz_leptondata.Pt();

        //Fill the histogram with the current data
        lepmass->Fill(mass);

        //We store the transverse momentum of all the leptons in the event
        addable_total_lorentz_leptondata=addable_total_lorentz_leptondata+addable_lorentz_leptondata;
      }

        /////////////////////////////////////////////////////

        //Thus only if there are leptons we consider the existence of a W boson
        //W boson

        //This doens't make much sense without criteria, but as we add criteria we'll see how this value improves
        addable_lorentz_wdata = addable_lorentz_metdata + addable_total_lorentz_leptondata;

        //Get the invariant transverse energy of that lorentz vector
        mass=addable_lorentz_wdata.Et();

        //Fill the histogram with the current data
        wmass->Fill(mass);
    }
    else
    {
      //No lepton, Pt=0
      addable_lorentz_leptondata=addable_lorentz_empty;
    }

/////////////////////////////////////////////////////

  //Jets

    //Loop through all the jets
      for (int i = 0; i < jetdata->GetSize()-1; ++i)
      {
        //Store all the data of the jet in this lorentz vector
        TLorentzVector * lorentz_jetdata = (TLorentzVector *)jetdata->At(i);

        //It's troublesome to do math with pointers
        addable_lorentz_jetdata = *lorentz_jetdata;

        //Get the transverse energy of that lorentz vector
        mass=addable_lorentz_jetdata.Et();
 
        //Fill the histogram with the current data
        jetmass->Fill(mass);

        //Add them up
        addable_total_lorentz_jetdata=addable_total_lorentz_jetdata+addable_lorentz_jetdata;
      }

      //Get the energy of everything
      addable_lorentz_wholedata=addable_total_lorentz_jetdata + addable_lorentz_metdata + addable_total_lorentz_leptondata;

      //Get the transverse energy of that lorentz vector
      mass=addable_lorentz_wholedata.Et();

      //Fill the histogram with the current data
      wholemass->Fill(mass);
  }

  //Activate the first section of the canvas
  c->cd(1);

  //Make the histogram
  metmass->Draw("H");

  //Put it in the canvas
  c->Update();

  //Repeat
  c->cd(2);
  lepmass->Draw("H");
  c->Update();

  c->cd(3);
  jetmass->Draw("H");
  c->Update();

  c->cd(4);
  wmass->Draw("H");
  c->Update();

  c->cd(5);
  wholemass->Draw("H");
  c->Update();

  //Save the image
  c->SaveAs("no_criteria.pdf");
  c->Close();


//1111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111

  //Reset all the different histograms in order to use them again 
  lepmass->Reset("M");
  wmass->Reset("M");
  metmass->Reset("M");
  wholemass->Reset("M"); 

  //Reset the totals
  addable_total_lorentz_leptondata=addable_lorentz_empty;
  addable_total_lorentz_jetdata=addable_lorentz_empty;

  //One criteria, single electron or muon
  for (int ientry = 0; ientry < nentries-1; ientry++)
  {
    //Reset the data
    leptondata->Clear();
    metdata->Clear();
    jetdata->Clear();

    //This line stores the proper data in the TClonesArrays
    tree->GetEntry(ientry);
/////////////////////////////////////////////////////

  //Met

    //Create a lorentz vector with the metdata of the current entry (it's size is always one)
    TLorentzVector * lorentz_metdata = (TLorentzVector *) metdata->At(0);

    //It's troublesome to do math with pointers
    addable_lorentz_metdata = *lorentz_metdata;

    //Get the invariant transverse energy of that lorentz vector
    mass=addable_lorentz_metdata.Et();
  
    //Fill the histogram with the current data
    metmass->Fill(mass);

/////////////////////////////////////////////////////

  //Leptons

    //Use only events with a single lepton and only if the identifier of the particle is ±11 or ± 13 (electrons muons or their anti particles) we use the data
    if((leptondata->GetSize() == 1) && ((abs(lepPdgId->at(0))==11) || (abs(lepPdgId->at(0))==13)))
    {
        //Store all the data of the lepton in this lorentz vector
        TLorentzVector * lorentz_leptondata = (TLorentzVector *)leptondata->At(0);

        //It's troublesome to do math with pointers, so we create this non pointer variable 
        addable_lorentz_leptondata = *lorentz_leptondata;

///////// Im working on a way to change the previous two lines into a single one

        //Get the transverse momentum of that lorentz vector
        mass=addable_lorentz_leptondata.Pt();

        //Fill the histogram with the current data
        lepmass->Fill(mass);

        /////////////////////////////////////////////////////

        //Thus only if there are leptons we consider the existence of a W boson
        //W boson

        //This doens't make much sense without criteria, but as we add criteria we'll see how this value improves
        addable_lorentz_wdata = addable_lorentz_metdata + addable_lorentz_leptondata;

        //Get the invariant transverse energy of that lorentz vector
        mass=addable_lorentz_wdata.Et();

        //Fill the histogram with the current data
        wmass->Fill(mass);
    }
    else
    {
      //No lepton, Pt=0
      addable_lorentz_leptondata=addable_lorentz_empty; // initialized by (0., 0., 0., 0.)
    }

/////////////////////////////////////////////////////

  //Jets

    //Loop through all the jets
      for (int i = 0; i < jetdata->GetSize()-1; ++i)
      {
        //Store all the data of the jet in this lorentz vector
        TLorentzVector * lorentz_jetdata = (TLorentzVector *)jetdata->At(i);

        //It's troublesome to do math with pointers
        addable_lorentz_jetdata = *lorentz_jetdata;

        //Get the transverse energy of that lorentz vector
        mass=addable_lorentz_jetdata.Et();
 
        //Fill the histogram with the current data
        jetmass->Fill(mass);

        //Add them up
        addable_total_lorentz_jetdata=addable_total_lorentz_jetdata+addable_lorentz_jetdata;
      }

      //Get the energy of everything
      addable_lorentz_wholedata=addable_total_lorentz_jetdata + addable_lorentz_metdata + addable_lorentz_leptondata;

      //Get the transverse energy of that lorentz vector
      mass=addable_lorentz_wholedata.Et();

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
  wmass->Draw("H");
  c1->Update();

  c1->cd(5);
  wholemass->Draw("H");
  c1->Update();

  //Save the image
  c1->SaveAs("one_criteria.pdf");
  c1->Close();

//2222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222

  //Reset all the different histograms in order to use them again
  lepmass->Reset("M");
  wmass->Reset("M");
  metmass->Reset("M");
  wholemass->Reset("M");

  //Reset the totals
  addable_total_lorentz_leptondata=addable_lorentz_empty;
  addable_total_lorentz_jetdata=addable_lorentz_empty;

  //Two criteria
  for (int ientry = 0; ientry < nentries-1; ientry++)
  {
    //Reset the data
    leptondata->Clear();
    metdata->Clear();
    jetdata->Clear();

    //This line stores the proper data in the TClonesArrays
    tree->GetEntry(ientry);
/////////////////////////////////////////////////////

  //Met

    //Create a lorentz vector with the metdata of the current entry (it's size is always one)
    TLorentzVector * lorentz_metdata = (TLorentzVector *) metdata->At(0);

    //It's troublesome to do math with pointers
    addable_lorentz_metdata = *lorentz_metdata;

    //Get the invariant transverse energy of that lorentz vector
    mass=addable_lorentz_metdata.Et();
  
    //Fill the histogram with the current data
    metmass->Fill(mass);

/////////////////////////////////////////////////////

  //Leptons

    //Use only events with a single lepton and only if the identifier of the particle is ±11 or ± 13 (electrons muons or their anti particles) we use the data
    if((leptondata->GetSize() == 1) && ((abs(lepPdgId->at(0))==11) || (abs(lepPdgId->at(0))==13)))
    {
        //Store all the data of the lepton in this lorentz vector
        TLorentzVector * lorentz_leptondata = (TLorentzVector *)leptondata->At(0);

        //It's troublesome to do math with pointers, so we create this non pointer variable 
        addable_lorentz_leptondata = *lorentz_leptondata;

///////// Im working on a way to change the previous two lines into a single one

        //Get the transverse momentum of that lorentz vector
        mass=addable_lorentz_leptondata.Pt();

        //Get the Eta value of that Lorentz vector
        eta=addable_lorentz_leptondata.Eta();

        //Implementing slection criteria
        if (mass>40 && abs(eta)<2.5)
        {

          //Fill the histogram with the current data
          lepmass->Fill(mass);

/////////////////////////////////////////////////////

          //Thus only if there are leptons within our criteria we consider the existence of a W boson

          //W boson

          //This doens't make much sense without criteria, but as we add criteria we'll see how this value improves
          addable_lorentz_wdata = addable_lorentz_metdata + addable_lorentz_leptondata;

          //Get the invariant transverse energy of that lorentz vector
          mass=addable_lorentz_wdata.Et();

          //Fill the histogram with the current data
          wmass->Fill(mass);
        }
        else
        {
          //No lepton, Pt=0
          addable_lorentz_leptondata=addable_lorentz_empty;; // initialized by (0., 0., 0., 0.)
        }
    }
    else
    {
      //No lepton, Pt=0
      addable_lorentz_leptondata=addable_lorentz_empty;; // initialized by (0., 0., 0., 0.)
    }

/////////////////////////////////////////////////////

  //Jets

    //Loop through all the jets
      for (int i = 0; i < jetdata->GetSize()-1; ++i)
      {
        //Store all the data of the jet in this lorentz vector
        TLorentzVector * lorentz_jetdata = (TLorentzVector *)jetdata->At(i);

        //It's troublesome to do math with pointers
        addable_lorentz_jetdata = *lorentz_jetdata;

        //Get the transverse energy of that lorentz vector
        mass=addable_lorentz_jetdata.Et();
 
        //Fill the histogram with the current data
        jetmass->Fill(mass);

        //Add them up
        addable_total_lorentz_jetdata=addable_total_lorentz_jetdata+addable_lorentz_jetdata;
      }

      //Get the energy of everything
      addable_lorentz_wholedata=addable_total_lorentz_jetdata + addable_lorentz_metdata + addable_lorentz_leptondata;

      //Get the transverse energy of that lorentz vector
      mass=addable_lorentz_wholedata.Et();

      //Fill the histogram with the current data
      wholemass->Fill(mass);
  }

  //Activate the first section of the canvas
  c2->cd(1);

  //Make the histogram
  metmass->Draw("H");

  //Put it in the canvas
  c2->Update();

  //Repeat
  c2->cd(2);
  lepmass->Draw("H");
  c2->Update();

  c2->cd(3);
  jetmass->Draw("H");
  c2->Update();

  c2->cd(4);
  wmass->Draw("H");
  c2->Update();

  c2->cd(5);
  wholemass->Draw("H");
  c2->Update();

  //Save the image
  c2->SaveAs("two_criteria.pdf");
  c2->Close();

//3333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333

  //Reset all the different histograms in order to use them again
  lepmass->Reset("M");
  wmass->Reset("M");
  metmass->Reset("M");
  wholemass->Reset("M");

  //Reset the totals
  addable_total_lorentz_leptondata=addable_lorentz_empty;
  addable_total_lorentz_jetdata=addable_lorentz_empty;


  //Three criteria
  for (int ientry = 0; ientry < nentries-1; ientry++)
  {
    //Reset the data
    leptondata->Clear();
    metdata->Clear();
    jetdata->Clear();

    //This line stores the proper data in the TClonesArrays
    tree->GetEntry(ientry);
/////////////////////////////////////////////////////

  //Met

    //Create a lorentz vector with the metdata of the current entry (it's size is always one)
    TLorentzVector * lorentz_metdata = (TLorentzVector *) metdata->At(0);

    //It's troublesome to do math with pointers
    addable_lorentz_metdata = *lorentz_metdata;

    //Get the invariant transverse energy of that lorentz vector
    mass=addable_lorentz_metdata.Et();

    //Implementing slection criteria
    if (mass>60)
    {
      //Fill the histogram with the current data
      metmass->Fill(mass);
    }
    else
    {
      //MET=0
      addable_lorentz_metdata=addable_lorentz_empty;; // initialized by (0., 0., 0., 0.)
    }
  

/////////////////////////////////////////////////////

  //Leptons

    //Use only events with a single lepton and only if the identifier of the particle is ±11 or ± 13 (electrons muons or their anti particles) we use the data
    if((leptondata->GetSize() == 1) && ((abs(lepPdgId->at(0))==11) || (abs(lepPdgId->at(0))==13)))
    {
        //Store all the data of the lepton in this lorentz vector
        TLorentzVector * lorentz_leptondata = (TLorentzVector *)leptondata->At(0);

        //It's troublesome to do math with pointers, so we create this non pointer variable 
        addable_lorentz_leptondata = *lorentz_leptondata;

///////// Im working on a way to change the previous two lines into a single one

        //Get the transverse momentum of that lorentz vector
        mass=addable_lorentz_leptondata.Pt();

        //Get the Eta value of that Lorentz vector
        eta=addable_lorentz_leptondata.Eta();

        //Implementing slection criteria
        if (mass>40 && abs(eta)<2.5)
        {

          //Fill the histogram with the current data
          lepmass->Fill(mass);

/////////////////////////////////////////////////////

          //Thus only if there are leptons within our criteria we consider the existence of a W boson

          //W boson

          //This doens't make much sense without criteria, but as we add criteria we'll see how this value improves
          addable_lorentz_wdata = addable_lorentz_metdata + addable_lorentz_leptondata;

          //Get the invariant transverse energy of that lorentz vector
          mass=addable_lorentz_wdata.Et();

          //Fill the histogram with the current data
          wmass->Fill(mass);
        }
        else
        {
          //No lepton, Pt=0
          addable_lorentz_leptondata=addable_lorentz_empty;; // initialized by (0., 0., 0., 0.)
        }
    }
    else
    {
      //No lepton, Pt=0
      addable_lorentz_leptondata=addable_lorentz_empty;; // initialized by (0., 0., 0., 0.)
    }

/////////////////////////////////////////////////////

  //Jets

    //Loop through all the jets
      for (int i = 0; i < jetdata->GetSize()-1; ++i)
      {
        //Store all the data of the jet in this lorentz vector
        TLorentzVector * lorentz_jetdata = (TLorentzVector *)jetdata->At(i);

        //It's troublesome to do math with pointers
        addable_lorentz_jetdata = *lorentz_jetdata;

        //Get the transverse energy of that lorentz vector
        mass=addable_lorentz_jetdata.Et();
 
        //Fill the histogram with the current data
        jetmass->Fill(mass);

        //Add them up
        addable_total_lorentz_jetdata=addable_total_lorentz_jetdata+addable_lorentz_jetdata;
      }

      //Get the energy of everything
      addable_lorentz_wholedata=addable_total_lorentz_jetdata + addable_lorentz_metdata + addable_lorentz_leptondata;

      //Get the transverse energy of that lorentz vector
      mass=addable_lorentz_wholedata.Et();

      //Fill the histogram with the current data
      wholemass->Fill(mass);
  }

  //Activate the first section of the canvas
  c3->cd(1);

  //Make the histogram
  metmass->Draw("H");

  //Put it in the canvas
  c3->Update();

  //Repeat
  c3->cd(2);
  lepmass->Draw("H");
  c3->Update();

  c3->cd(3);
  jetmass->Draw("H");
  c3->Update();

  c3->cd(4);
  wmass->Draw("H");
  c3->Update();

  c3->cd(5);
  wholemass->Draw("H");
  c3->Update();

  //Save the image
  c3->SaveAs("three_criteria.pdf");
  c3->Close();


//4444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444

  //Reset all the different histograms in order to use them again
  lepmass->Reset("M");
  wmass->Reset("M");
  metmass->Reset("M");
  wholemass->Reset("M");

  //Reset the totals
  addable_total_lorentz_leptondata=addable_lorentz_empty;
  addable_total_lorentz_jetdata=addable_lorentz_empty;

  //Four
  for (int ientry = 0; ientry < nentries-1; ientry++)
  {
    //Reset the data
    leptondata->Clear();
    metdata->Clear();
    jetdata->Clear();

    //This line stores the proper data in the TClonesArrays
    tree->GetEntry(ientry);
/////////////////////////////////////////////////////

  //Met

    //Create a lorentz vector with the metdata of the current entry (it's size is always one)
    TLorentzVector * lorentz_metdata = (TLorentzVector *) metdata->At(0);

    //It's troublesome to do math with pointers
    addable_lorentz_metdata = *lorentz_metdata;

    //Get the invariant transverse energy of that lorentz vector
    mass=addable_lorentz_metdata.Et();

    //Implementing slection criteria
    if (mass>60)
    {
      //Fill the histogram with the current data
      metmass->Fill(mass);
    }
    else
    {
      //MET=0
      addable_lorentz_metdata=addable_lorentz_empty;; // initialized by (0., 0., 0., 0.)
    }
  

/////////////////////////////////////////////////////

  //Leptons

    //Use only events with a single lepton and only if the identifier of the particle is ±11 or ± 13 (electrons muons or their anti particles) we use the data
    if((leptondata->GetSize() == 1) && ((abs(lepPdgId->at(0))==11) || (abs(lepPdgId->at(0))==13)))
    {
        //Store all the data of the lepton in this lorentz vector
        TLorentzVector * lorentz_leptondata = (TLorentzVector *)leptondata->At(0);

        //It's troublesome to do math with pointers, so we create this non pointer variable 
        addable_lorentz_leptondata = *lorentz_leptondata;

///////// Im working on a way to change the previous two lines into a single one

        //Get the transverse momentum of that lorentz vector
        mass=addable_lorentz_leptondata.Pt();

        //Get the Eta value of that Lorentz vector
        eta=addable_lorentz_leptondata.Eta();

        //Implementing slection criteria
        if (mass>40 && abs(eta)<2.5)
        {
        //Fill the histogram with the current data
        lepmass->Fill(mass);

        /////////////////////////////////////////////////////

        //Thus only if there are leptons within our criteria we consider the existence of a W boson

        //W boson

        //This doens't make much sense without criteria, but as we add criteria we'll see how this value improves
        addable_lorentz_wdata = addable_lorentz_metdata + addable_lorentz_leptondata;

        //Get the invariant transverse energy of that lorentz vector
        mass=addable_lorentz_wdata.Et();

        //Fill the histogram with the current data
        wmass->Fill(mass);
        }
        else
        {
          //No lepton, Pt=0
          addable_lorentz_leptondata=addable_lorentz_empty;; // initialized by (0., 0., 0., 0.)
        }
    }
    else
    {
      //No lepton, Pt=0
      addable_lorentz_leptondata=addable_lorentz_empty;; // initialized by (0., 0., 0., 0.)
    }

/////////////////////////////////////////////////////

  //Jets

    //Loop through all the jets
    if (jetdata->GetSize()==3)
    {
      for (int i = 0; i < jetdata->GetSize()-1; ++i)
      {
        //Store all the data of the jet in this lorentz vector
        TLorentzVector * lorentz_jetdata = (TLorentzVector *)jetdata->At(i);

        //It's troublesome to do math with pointers
        addable_lorentz_jetdata = *lorentz_jetdata;

        //Get the transverse energy of that lorentz vector
        mass=addable_lorentz_jetdata.Et();
 
        if (mass>30 && abs(eta)<2.4)
        {
          //Fill the histogram with the current data
          jetmass->Fill(mass);

          //Add them up
          addable_total_lorentz_jetdata=addable_total_lorentz_jetdata+addable_lorentz_jetdata;
        }
        else
        {
          addable_lorentz_jetdata = addable_lorentz_empty;
        }
      }
    }

      //Get the energy of everything
      addable_lorentz_wholedata=addable_lorentz_jetdata + addable_lorentz_metdata + addable_lorentz_leptondata;

      //Get the transverse energy of that lorentz vector
      mass=addable_lorentz_wholedata.Et();

      //Fill the histogram with the current data
      wholemass->Fill(mass);
  }

  //Activate the first section of the canvas
  c4->cd(1);

  //Make the histogram
  metmass->Draw("H");

  //Put it in the canvas
  c4->Update();

  //Repeat
  c4->cd(2);
  lepmass->Draw("H");
  c4->Update();

  c4->cd(3);
  jetmass->Draw("H");
  c4->Update();

  c4->cd(4);
  wmass->Draw("H");
  c4->Update();

  c4->cd(5);
  wholemass->Draw("H");
  c4->Update();

  //Save the image
  c4->SaveAs("four_criteria.pdf");
  c4->Close();

  // cleanup
  delete file; // automatically deletes "tree" too
  delete lepPdgId;
  delete leptondata;
  delete metdata;
  delete jetdata;
  return 0; 
}
