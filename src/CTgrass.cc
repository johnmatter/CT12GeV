// Filename: CTgrass.cc
// Description: 
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
#include <TStyle.h>
#include <TText.h>
#include "Constants.h"
#include "CTRun.h"
using namespace std;

void CTgrass(Int_t firstRun, Int_t lastRun)
{
  /*    int cnts = 0;
    int cnts2 = 0;
    int bcnts = 0;
    //--------- Histograms from the data -----------
    TH1D *hW = new TH1D("hW","timing", 150, 0,1500);
    TH1D *hW0 = new TH1D("hW0","timing", 150, 0,1500);
    TH1D *hW1 = new TH1D("hW1","timing", 150, 0,1500);
    TH1D *hW2 = new TH1D("hW2","timing", 150, 0,1500);
    TH1D *hWm = new TH1D("hW2","mutiplicity", 10, 0,10);
    hW->SetMarkerColor(kRed);  
    hW0->SetMarkerColor(kRed);
    hW1->SetMarkerColor(kBlue);
    hW2->SetMarkerColor(kGreen);*/

    gStyle->SetOptStat(1);
    CTRun *ct = new CTRun(firstRun,lastRun);
    if(!ct->fRunExist)
	return;
    //------------------------------------ Fill histograms from data ------- 
    TTree * tree = ct->GetTree();

    Int_t nEvents = tree->GetEntries();
    cout << nEvents <<"  events chained together" << endl;
    TCanvas *c1 = new TCanvas("SHMS","SHMS");
    /*    c1->Divide(2,2);
    c1->cd(1);
    ct->Draw("D.pdcref1[0]","D.pdcref1[0]> 13500 && D.pdcref1[0] < 15000");
    c1-cd(2);
    ct->Draw("D.pdcref1[1]","D.pdcref1[1]> 13500 && D.pdcref1[1] < 15000");
    c1-cd(3);
    ct->Draw("D.pdcref1[2]","D.pdcref1[2]> 13500 && D.pdcref1[2] < 15000");
    c1-cd(4);
    ct->Draw("D.pdcref1[3]","D.pdcref1[3]> 13500 && D.pdcref1[3] < 15000");*/
    /*
    for (Int_t event = 0; event < nEvents; ++ event)
    {
	if(event%50000 == 0)
	    cout << (Int_t)(100*event/nEvents) <<" % Done"<< endl;

	tree->GetEntry(event);
	//        if (ct->fP_DC_tdcmultipl == 1)
	//  continue;
        if(ct->D.pdcref1[0] > 13500 && ct->D.pdcref1[0] < 15000)
          {
	   hW->Fill(ct->fP_DC_reftime);
           bcnts++;
          }
        // 4/4 trigger cut to check consequences.
	//	if(!(ct->fP_hod_1xnhits > 0 && ct->fP_hod_1ynhits > 0 && ct->fP_hod_2xnhits > 0 && ct->fP_hod_2ynhits > 0))
	//	  continue;
	hWm->Fill(ct->fP_DC_tdcmultipl);
    }
    cout << bcnts <<"  events are in the grass  "<< endl;


    
    hW0->Draw("prof");
    c1->cd(2);
    hW->Draw("prof");
    hW1->Draw("same");
    hW2->Draw("same");
    c1->cd(3);
    hWm->Draw("prof"); */
    c1->Print("grass.pdf)","pdf");
}
