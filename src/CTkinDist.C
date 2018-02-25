// Filename: CTkinDist.cc
// Description: Look at some Kinematc distributions (without cut) by looping over each event. The main purpose is to show how to loop over events.
// Author: Latif Kabir < latif@jlab.org >
// Created: Thu Jan  4 17:03:26 2018 (-0500)
// URL: jlab.org/~latif

#include <iostream>
#include <TFile.h>
#include <TTree.h>
#include <TString.h>
#include <TH1D.h>
#include <TCanvas.h>
#include <TSystem.h>
#include "Constants.h"

using namespace std;

void CTkinDist()
{
    TString fileName = ROOT_FILE_PATH;
    fileName += "ep_2023.root";
    // fileName += "coin_replay_production_1711_evt7.root";

    Bool_t runNotFound = gSystem->AccessPathName(fileName);
    if(runNotFound)
    {	    
	cout << "Requested file " << fileName << " does NOT exist" <<endl;
	return;
    }
    
    TFile *file = new TFile(fileName);
    TTree *tree = (TTree*)file->Get("T");    
    //tree->Print();

    TH1D *h1PgtrBeta = new TH1D("P.gtr.beta", "P.gtr.beta; ", 500, 0, 2);
    TH1D *h1PkinPrimaryW2 = new TH1D("P.kin.primary.W", "P.kin.primary.W", 500, 0, 5);
    TH1D *h1PkinPrimaryQ2 = new TH1D("P.kin.primary.Q2", "P.kin.primary.Q2", 500, 0, 2);
    TH1D *h1PkinPrimaryNu = new TH1D("P.kin.primary.nu", "P.kin.primary.nu", 500, 0, 2);

    TH1D *h1HgtrBeta = new TH1D("H.gtr.beta", "H.gtr.beta", 500, 0, 2);
    TH1D *h1HkinSecondaryPmiss = new TH1D("H.kin.secondary.pmiss", "H.kin.secondary.pmiss", 500, -0.1, 0.5);
    TH1D *h1HkinSecondaryPmiss_x = new TH1D("H.kin.secondary.pmiss_x", "H.kin.secondary.pmiss_x", 500, -0.1, 0.5);
    TH1D *h1HkinSecondaryEmiss = new TH1D("H.kin.secondary.emiss", "H.kin.secondary.emiss", 500, -0.3, 0.3);
    
    Double_t PgtrBeta;
    Double_t PkinPrimaryW;
    Double_t PkinPrimaryQ2;
    Double_t PkinPrimaryNu;
    
    Double_t HgtrBeta;        
    Double_t HkinSecondaryPmiss;
    Double_t HkinSecondaryPmiss_x;
    Double_t HkinSecondaryEmiss;
          
    tree->SetBranchAddress("P.gtr.beta", &PgtrBeta);    
    tree->SetBranchAddress("P.kin.primary.W", &PkinPrimaryW);
    tree->SetBranchAddress("P.kin.primary.Q2", &PkinPrimaryQ2);
    tree->SetBranchAddress("P.kin.primary.nu", &PkinPrimaryNu);

    
    tree->SetBranchAddress("H.gtr.beta", &HgtrBeta);
    tree->SetBranchAddress("H.kin.secondary.pmiss", &HkinSecondaryPmiss);
    tree->SetBranchAddress("H.kin.secondary.pmiss_x", &HkinSecondaryPmiss);
    tree->SetBranchAddress("H.kin.secondary.pmiss_x", &HkinSecondaryPmiss_x);
    tree->SetBranchAddress("H.kin.secondary.emiss", &HkinSecondaryEmiss);

    Int_t nEvents = tree->GetEntries();

    for (Int_t event = 0; event < nEvents; ++ event)
    {
	tree->GetEntry(event);

	h1PgtrBeta->Fill(PgtrBeta);
	h1HgtrBeta->Fill(HgtrBeta);
	h1PkinPrimaryW2->Fill(PkinPrimaryW);
	h1PkinPrimaryQ2->Fill(PkinPrimaryQ2);
	h1HkinSecondaryPmiss->Fill(HkinSecondaryPmiss);
	h1HkinSecondaryEmiss->Fill(HkinSecondaryEmiss);

	// cout << (Int_t)(100*event/nEvents) <<"% Done"<<flush;
    }

    TCanvas *c1 = new TCanvas("c1","c1"); 
    h1PgtrBeta->Draw();
    TCanvas *c2 = new TCanvas("c2","c2"); 
    h1HgtrBeta->Draw();
    TCanvas *c3 = new TCanvas("c3","c3"); 
    h1PkinPrimaryW2->Draw();
    TCanvas *c4 = new TCanvas("c4","c4"); 
    h1PkinPrimaryQ2->Draw();
    TCanvas *c5 = new TCanvas("c5","c5"); 
    h1HkinSecondaryPmiss->Draw();    
    TCanvas *c6 = new TCanvas("c6","c6"); 
    h1HkinSecondaryEmiss->Draw();
}
