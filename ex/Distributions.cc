// Filename: Distributions.cc
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
#include "Constants.h"

using namespace std;

void Distributions()
{
    TString fileName = ROOT_FILE_PATH;
    fileName += "coin_replay_production_1711_50000.root";
    TFile *file = new TFile(fileName);
    TTree *tree = (TTree*)file->Get("T");    
    //tree->Print();

    TH1D *hist = new TH1D("hist", "hist", 100, 0, 2);
    
    Double_t PgtrBeta;

    
    //------ De-activating unnecessary branches can speed up the looping --------
    // tree->SetBranchStatus("*",0);
    // tree->SetBranchStatus("P.gtr.beta",1);

    //------ The calssic way is to set the branch address -----
    tree->SetBranchAddress("P.gtr.beta", &PgtrBeta);

    for (Int_t event = 0; event < tree->GetEntries(); ++ event)
    {
	tree->GetEntry(event);

	// Alternative to setting the branch address is to use GetLeaf & GetValue
	// PgtrBeta= tree->GetLeaf("P.gtr.beta")->GetValue();

	hist->Fill(PgtrBeta);
    }
    hist->Draw();    
}
