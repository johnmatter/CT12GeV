// Filename: CTdemoLoop.cc
// Description: Demo
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
#include <TEventList.h>
#include "Constants.h"
#include "CTRun.h"
using namespace std;

void CTdemoLoop(Int_t runNumber)
{
    CTRun *ct = new CTRun(runNumber);
    if(!ct->fRunExist)
	return;
    
    ct->DefinePBetaCut(0.5,1.5);   // proton beta cut
    ct->DefineHBetaCut(0.5,1.5);   // e beta cut
    ct->DefinePCerCut(-1,0.1);     // Cerenkov counter cut for e- and proton
    ct->DefineHCerCut(0.1);        // HMS Calorimeter cut 
    ct->DefineHCalCut(0.8,1.5);    // HMS calorimeter cut       
    ct->DefineHPreShCut(0.2);      // HMS Pre-shower cut

    ct->ApplyCut();
    
    TH1D *h1HgtrBeta = new TH1D("H.gtr.beta", "H.gtr.beta; ", 500, 0, 2);
    TH1D *h1HkinPrimaryW = new TH1D("H.kin.primary.W", "H.kin.primary.W", 500, 0, 5);
    TH1D *h1HkinPrimaryQ2 = new TH1D("H.kin.primary.Q2", "H.kin.primary.Q2", 500, -1, 10);
    TH1D *h1HkinPrimaryNu = new TH1D("H.kin.primary.nu", "H.kin.primary.nu", 500, 0, 2);

    TH1D *h1PgtrBeta = new TH1D("P.gtr.beta", "P.gtr.beta", 500, 0, 2);
    TH1D *h1PkinSecondaryPmiss = new TH1D("P.kin.secondary.pmiss", "P.kin.secondary.pmiss", 500, -10, 10);
    TH1D *h1PkinSecondaryPmiss_x = new TH1D("P.kin.secondary.pmiss_x", "P.kin.secondary.pmiss_x", 500, -10, 10);
    TH1D *h1PkinSecondaryEmiss = new TH1D("P.kin.secondary.emiss", "P.kin.secondary.emiss", 500, -10, 10);
    
    TTree * tree = ct->GetTree();
    TEventList *list = ct->fCTEvents;
    Int_t nEvents = list->GetN();
    Int_t event = 0;
    for (Int_t index = 0; index < nEvents; ++index)
    {
	event = list->GetEntry(index);
	if(event%5000 == 0)
	    cout << (Int_t)(100*event/nEvents) <<" % Done"<< endl;

	tree->GetEntry(event);

	h1PgtrBeta->Fill(ct->fP_gtr_beta);
	h1HgtrBeta->Fill(ct->fH_gtr_beta);
	h1HkinPrimaryW->Fill(ct->fH_kin_primary_W);
	h1HkinPrimaryQ2->Fill(ct->fH_kin_primary_Q2);
	h1PkinSecondaryPmiss->Fill(ct->fP_kin_secondary_pmiss);
	h1PkinSecondaryEmiss->Fill(ct->fP_kin_secondary_emiss);
    }

    TCanvas *c1 = new TCanvas("c1","c1"); 
    h1PgtrBeta->Draw();
    TCanvas *c2 = new TCanvas("c2","c2"); 
    h1HgtrBeta->Draw();
    TCanvas *c3 = new TCanvas("c3","c3"); 
    h1HkinPrimaryW->Draw();
    TCanvas *c4 = new TCanvas("c4","c4"); 
    h1HkinPrimaryQ2->Draw();
    TCanvas *c5 = new TCanvas("c5","c5"); 
    h1PkinSecondaryPmiss->Draw();    
    TCanvas *c6 = new TCanvas("c6","c6"); 
    h1PkinSecondaryEmiss->Draw();
}
