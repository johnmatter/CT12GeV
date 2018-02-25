// Filename: CTexample.cc
// Description: Look at some distributions by looping over each event. The main purpose is to show how to loop over events.
// Author: Latif Kabir < latif@jlab.org >
// Created: Thu Jan  4 17:03:26 2018 (-0500)
// URL: jlab.org/~latif

#include <iostream>
#include <TFile.h>
#include <TTree.h>
#include <TLeaf.h>
#include <TString.h>
#include <TH1D.h>
#include <TSystem.h>
#include "Constants.h"

using namespace std;

void CTexample()
{
    TString fileName = ROOT_FILE_PATH;
    // fileName += "coin_replay_production_1487_10000.root";
    fileName += "coin_2023.root";
    Bool_t runNotFound = gSystem->AccessPathName(fileName);
    if(runNotFound)
    {	    
	cout << "Requested file " << fileName << " does NOT exist" <<endl;
	return;
    }

    TFile *file = new TFile(fileName);
    TTree *tree = (TTree*)file->Get("T");    
    tree->Print();

    TH1D *hist = new TH1D("hist", "hist", 100, -5., 5.);
    
    Double_t PgtrBeta;
    // Double_t W;

    tree->SetBranchAddress("P.kin.primary.W", &PgtrBeta);

    for (Int_t event = 0; event < tree->GetEntries(); ++ event)
    {
	tree->GetEntry(event);

	// Alternative way to access branch variable without SetBranchAddress
	// PgtrBeta= tree->GetLeaf("P.gtr.beta")->GetValue();
	
	hist->Fill(PgtrBeta);
    }
    hist->Draw();    
}
