// Filename: CTkinDistRun.cc
// Description: Look at some Kinematc distributions (without cut) by looping over each event using CTRun class
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
#include "CTRun.h"
using namespace std;

void CTkinDistRun(Int_t runNumber)
{
    CTRun *run = new CTRun(runNumber);
    if(!run->fRunExist)
	return;
    
    TH1D *h1PgtrBeta = new TH1D("P.gtr.beta", "P.gtr.beta; ", 500, 0, 2);
    TH1D *h1PkinPrimaryW = new TH1D("P.kin.primary.W", "P.kin.primary.W", 500, 0, 5);
    TH1D *h1PkinPrimaryQ2 = new TH1D("P.kin.primary.Q2", "P.kin.primary.Q2", 500, 0, 2);
    TH1D *h1PkinPrimaryNu = new TH1D("P.kin.primary.nu", "P.kin.primary.nu", 500, 0, 2);

    TH1D *h1HgtrBeta = new TH1D("H.gtr.beta", "H.gtr.beta", 500, 0, 2);
    TH1D *h1HkinSecondaryPmiss = new TH1D("H.kin.secondary.pmiss", "H.kin.secondary.pmiss", 500, -0.1, 0.5);
    TH1D *h1HkinSecondaryPmiss_x = new TH1D("H.kin.secondary.pmiss_x", "H.kin.secondary.pmiss_x", 500, -0.1, 0.5);
    TH1D *h1HkinSecondaryEmiss = new TH1D("H.kin.secondary.emiss", "H.kin.secondary.emiss", 500, -0.3, 0.3);
    
    TTree * tree = run->GetTree();

    //Optional:Disable unnecessary branches and enable required ones to speed up analysis for larger file
    //You do not need this if file is already skimmed while running replay
    tree->SetBranchStatus("*",0);
    tree->SetBranchStatus("P.kin.primary.*",1);
    tree->SetBranchStatus("H.kin.secondary.*",1);
    tree->SetBranchStatus("P.gtr.*",1);
    tree->SetBranchStatus("H.gtr.*",1);

    Int_t nEvents = tree->GetEntries();

    for (Int_t event = 0; event < nEvents; ++ event)
    {
	if(event%5000 == 0)
	    cout << (Int_t)(100*event/nEvents) <<" % Done"<< endl;

	tree->GetEntry(event);

	h1PgtrBeta->Fill(run->fP_gtr_beta);
	h1HgtrBeta->Fill(run->fH_gtr_beta);
	h1PkinPrimaryW->Fill(run->fP_kin_Primary_W);
	h1PkinPrimaryQ2->Fill(run->fP_kin_Primary_Q2);
	h1HkinSecondaryPmiss->Fill(run->fH_kin_secondary_pmiss);
	h1HkinSecondaryEmiss->Fill(run->fH_kin_secondary_emiss);
    }

    TCanvas *c1 = new TCanvas("c1","c1"); 
    h1PgtrBeta->Draw();
    TCanvas *c2 = new TCanvas("c2","c2"); 
    h1HgtrBeta->Draw();
    TCanvas *c3 = new TCanvas("c3","c3"); 
    h1PkinPrimaryW->Draw();
    TCanvas *c4 = new TCanvas("c4","c4"); 
    h1PkinPrimaryQ2->Draw();
    TCanvas *c5 = new TCanvas("c5","c5"); 
    h1HkinSecondaryPmiss->Draw();    
    TCanvas *c6 = new TCanvas("c6","c6"); 
    h1HkinSecondaryEmiss->Draw();
}
