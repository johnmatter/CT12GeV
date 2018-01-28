// Filename: CTdataVsSimc.cc
// Description: Compare Data vs Simc
// Author: Latif Kabir < latif@jlab.org >
// Created: Thu Jan  4 17:03:26 2018 (-0500)
// URL: jlab.org/~latif

/*
Quantities to compare:
delta, ytar, xptar, yptar for Both HMS and SHMS, for both data and Simc = 4 x 2 x 2 = 16 Plots. Simc and data histos are super layed on each other
*/

#include <iostream>
#include <TFile.h>
#include <TTree.h>
#include <TString.h>
#include <TH1D.h>
#include <TCanvas.h>
#include <TSystem.h>
#include <TStyle.h>
#include "Constants.h"
#include "CTRun.h"
using namespace std;

void CTdataVsSimc(Int_t firstRun, Int_t lastRun)
{
    //--------- Histograms from the data -----------
    //HMS
    TH1D *hDelta = new TH1D("hDelta","HMS DELTA (%)",100,-12,12);
    TH1D *hXptar = new TH1D("hXptar","HMS XPTAR (rad)",100,-0.1,0.1);
    TH1D *hYptar = new TH1D("hYptar","HMS YPTAR (rad)",100,-0.06,0.06);
    TH1D *hYtar = new TH1D("hYtar","HMS YTAR (cm)",100,-12.0,12.0);
    //SHMS
    TH1D *pDelta = new TH1D("pDelta","SHMS DELTA (%)",100,-12,12);
    TH1D *pXptar = new TH1D("pXptar","SHMS XPTAR (rad)",100,-0.06,0.06);
    TH1D *pYptar = new TH1D("pYptar","SHMS YPTAR (rad)",100,-0.06,0.06);
    TH1D *pYtar = new TH1D("pYtar","SHMS YTAR (cm)",100,-12.0,12.0);

    //--------- Histograms from the Simc -----------
    //HMS
    TH1D *hSimcDelta = new TH1D("hSimcDelta","HMS DELTA (SIMC) (%)",100,-12,12);
    TH1D *hSimcXptar = new TH1D("hSimcXptar","HMS XPTAR (SIMC) (rad)",100,-0.1,0.1);
    TH1D *hSimcYptar = new TH1D("hSimcYptar","HMS YPTAR (SIMC) (rad)",100,-0.06,0.06);
    TH1D *hSimcYtar = new TH1D("hSimcYtar","HMS YTAR (SIMC) (cm)",100,-12.0,12.0);
    hSimcDelta->SetMarkerColor(kRed);  
    hSimcXptar->SetMarkerColor(kRed);  
    hSimcYptar->SetMarkerColor(kRed);  
    hSimcYtar->SetMarkerColor(kRed);  

    //SHMS
    TH1D *pSimcDelta = new TH1D("pSimcDelta","SHMS DELTA (SIMC) (%)",100,-12,12);
    TH1D *pSimcXptar = new TH1D("pSimcXptar","SHMS XPTAR (SIMC) (rad)",100,-0.06,0.06);
    TH1D *pSimcYptar = new TH1D("pSimcYptar","SHMS YPTAR (SIMC) (rad)",100,-0.06,0.06);
    TH1D *pSimcYtar = new TH1D("pSimcYtar","SHMS YTAR (SIMC) (cm)",100,-12.0,12.0);
    pSimcDelta->SetMarkerColor(kRed);  
    pSimcXptar->SetMarkerColor(kRed);  
    pSimcYptar->SetMarkerColor(kRed);  
    pSimcYtar->SetMarkerColor(kRed);  

    
    CTRun *ct = new CTRun(firstRun,lastRun);
    if(!ct->fRunExist)
	return;
        
    TTree * tree = ct->GetTree();

    Int_t nEvents = tree->GetEntries();

    for (Int_t event = 0; event < nEvents; ++ event)
    {
	if(event%5000 == 0)
	    cout << (Int_t)(100*event/nEvents) <<" % Done"<< endl;

	tree->GetEntry(event);

	// PID Cut
	if( !(ct->fP_gtr_beta > 0.6 && ct->fP_gtr_beta < 1.4                             // proton beta cut
	      && ct->fH_gtr_beta > 0.8 &&  ct->fH_gtr_beta < 1.2                         // e beta cut
	      && ct->fH_cer_npeSum > 0.0 && ct->fP_hgcer_npeSum < 0.5                    // Cerenkov counter cut for e- and proton
	      && ct->fH_cal_etottracknorm > 0.7 && ct->fH_cal_etottracknorm < 1.65        // HMS Calorimeter cut 
	      && ct->fH_cal_eprtracknorm > 0.1) 	                                 // HMS Preshower cut       
	)
	continue;

	//HMS
	hDelta->Fill(ct->fH_gtr_dp);
	hXptar->Fill(ct->fH_gtr_th);
	hYptar->Fill(ct->fH_gtr_ph);
	hYtar->Fill(ct->fH_gtr_y);
	//SHMS
	pDelta->Fill(ct->fP_gtr_dp);
	pXptar->Fill(ct->fP_gtr_th);
	pYptar->Fill(ct->fP_gtr_ph);
	pYtar->Fill(ct->fP_gtr_y);
    }

    //------------------ Plot Simc data --------------------------
    ct->AddSimc("c12_q8_1.root");
    
    TTree *simc = ct->GetSimcTree();

    Int_t nEventsSimc = simc->GetEntries();

    for (Int_t event = 0; event < nEventsSimc; ++event)
    {
	if(event%5000 == 0)
	    cout << (Int_t)(100*event/nEventsSimc) <<" % Done"<< endl;

	simc->GetEntry(event);

	//HMS
	hSimcDelta->Fill(ct->fHSdelta, ct->fWeight);
	hSimcYtar->Fill(ct->fHSytar, ct->fWeight);
	hSimcYptar->Fill(ct->fHSyptar, ct->fWeight);
	hSimcXptar->Fill(ct->fHSxptar, ct->fWeight);

	//SHMS
	pSimcDelta->Fill(ct->fPSdelta, ct->fWeight);
	pSimcYtar->Fill(ct->fPSytar, ct->fWeight);
	pSimcYptar->Fill(ct->fPSyptar, ct->fWeight);
	pSimcXptar->Fill(ct->fPSxptar, ct->fWeight);
    }

    TCanvas *c1 = new TCanvas("HMS","HMS");
    c1->Divide(2,2);

    c1->cd(1);
    hDelta->DrawNormalized("prof");
    hSimcDelta->DrawNormalized("same");    
    c1->cd(2);
    hXptar->DrawNormalized("prof");
    hSimcXptar->DrawNormalized("same");
    c1->cd(3);
    hYptar->DrawNormalized("prof");
    hSimcYptar->DrawNormalized("same");
    c1->cd(4);
    hYtar->DrawNormalized("prof");
    hSimcYtar->DrawNormalized("same");

    c1->Print("Report.pdf(","pdf");
    
    TCanvas *c2 = new TCanvas("SHMS","SHMS");
    c2->Divide(2,2);

    c2->cd(1);
    pDelta->DrawNormalized("prof");
    pSimcDelta->DrawNormalized("same");    
    c2->cd(2);
    pXptar->DrawNormalized("prof");
    pSimcXptar->DrawNormalized("same");
    c2->cd(3);
    pYptar->DrawNormalized("prof");
    pSimcYptar->DrawNormalized("same");
    c2->cd(4);
    pYtar->DrawNormalized("prof");
    pSimcYtar->DrawNormalized("same");

    c2->Print("Report.pdf)","pdf");
}
