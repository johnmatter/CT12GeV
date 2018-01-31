// Filename: CTdataVsSimc.cc
// Description: Compare Data vs Simc. This script exclusively for CT expt and assumes SHMS as proton arm and HMS as electron arm
// Author: Latif Kabir < latif@jlab.org >
// Created: Thu Jan  4 17:03:26 2018 (-0500)
// URL: jlab.org/~latif

/*
Quantities to compare:
delta, ytar, xptar, yptar for Both HMS and SHMS, for both data and Simc = 4 x 2 x 2 = 16 Plots. Simc and data histos are super layed on each other.
In order to compare data and simc histograms must have same range and binning and drawn normalized.
*/

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

void CTdataVsSimc(Int_t firstRun, Int_t lastRun, TString SimcFileName)
{
    //-----These offsets (for data) to be removed once SHMS is betetr calibrated ----------
    Double_t OffsetEm = 0.035;
    Double_t OffsetPdelta = -1.2;
    Double_t OffsetPmz = 0.025;
    //--------- Histograms from the data -----------
    //HMS
    //Reconstructed quantities
    TH1D *hDelta = new TH1D("hDelta","HMS DELTA (%); HMS dp (%); Normalized counts",100,-12,12);
    TH1D *hXptar = new TH1D("hXptar","HMS XPTAR (rad); x^{'}_{tar} (rad); Normalized counts",100,-0.1,0.1);
    TH1D *hYptar = new TH1D("hYptar","HMS YPTAR (rad); x^{'}_{tar} (rad); Normalized counts",100,-0.06,0.06);
    TH1D *hYtar = new TH1D("hYtar","HMS YTAR (cm); y_{tar} (cm); Normalized counts",100,-12.0,12.0);
    //Kinematic quantities
    TH1D *hW = new TH1D("hW","W (GeV); W(GeV); Normalized counts", 150, 0.0, 2.0);
    
    //SHMS
    //Reconstructed quantities
    TH1D *pDelta = new TH1D("pDelta","SHMS DELTA (%); SHMS dp (%); Normalized counts",100,-12,12);
    TH1D *pXptar = new TH1D("pXptar","SHMS XPTAR (rad);x^{'}_{tar} (rad); Normalized counts",100,-0.06,0.06);
    TH1D *pYptar = new TH1D("pYptar","SHMS YPTAR (rad);y^{'}_{tar}(rad); Normalized counts",100,-0.06,0.06);
    TH1D *pYtar = new TH1D("pYtar","SHMS YTAR (cm); y_{tar} (cm); Normalized counts",100,-12.0,12.0);
    //Kinematic quantities
    TH1D *pEm = new TH1D("pEm","Missing Energy (GeV); E_{m} (GeV); Normalized counts",200,-0.15,0.25);
    TH1D *pPm = new TH1D("pPm","Pm (GeV/c); |P_{m}|(GeV/c); Normalized counts", 100, 0.0, 0.4);
    TH1D *pPmz = new TH1D("pPmz","Pmz (GeV/c); P_{mz} (GeV/c); Normalized counts", 100, -0.4, 0.4);
    
    //--------- Histograms from the Simc -----------
    //HMS
    //Reconstructed quantities
    TH1D *hSimcDelta = new TH1D("hSimcDelta","HMS DELTA (SIMC) (%)",100,-12,12);
    TH1D *hSimcXptar = new TH1D("hSimcXptar","HMS XPTAR (SIMC) (rad)",100,-0.1,0.1);
    TH1D *hSimcYptar = new TH1D("hSimcYptar","HMS YPTAR (SIMC) (rad)",100,-0.06,0.06);
    TH1D *hSimcYtar = new TH1D("hSimcYtar","HMS YTAR (SIMC) (cm)",100,-12.0,12.0);
    //Kinematic quantities
    TH1D *hSimcW = new TH1D("hSimcW","W (GeV)", 150, 0.0, 2.0);
   
    hSimcDelta->SetMarkerColor(kRed);  
    hSimcXptar->SetMarkerColor(kRed);  
    hSimcYptar->SetMarkerColor(kRed);  
    hSimcYtar->SetMarkerColor(kRed);  
    hSimcW->SetMarkerColor(kRed);  

    //SHMS
    //Reconstructed quantities
    TH1D *pSimcDelta = new TH1D("pSimcDelta","SHMS DELTA (SIMC) (%)",100,-12,12);
    TH1D *pSimcXptar = new TH1D("pSimcXptar","SHMS XPTAR (SIMC) (rad)",100,-0.06,0.06);
    TH1D *pSimcYptar = new TH1D("pSimcYptar","SHMS YPTAR (SIMC) (rad)",100,-0.06,0.06);
    TH1D *pSimcYtar = new TH1D("pSimcYtar","SHMS YTAR (SIMC) (cm)",100,-12.0,12.0);
    //Kinematic quantities
    TH1D *pSimcW = new TH1D("pSimcW","W (GeV)", 150, 0.0, 2.0);
    TH1D *pSimcEm = new TH1D("pSimcEm","Missing Energy (GeV)",200,-0.15,0.25);
    TH1D *pSimcPm = new TH1D("pSimcPm","|P_{m}| (GeV/c)", 100, 0.0, 0.4);
    TH1D *pSimcPmz = new TH1D("pSimcPmz","P_{mz} (GeV/c)", 100, -0.4, 0.4);

    pSimcDelta->SetMarkerColor(kRed);  
    pSimcXptar->SetMarkerColor(kRed);  
    pSimcYptar->SetMarkerColor(kRed);  
    pSimcYtar->SetMarkerColor(kRed);  
    pSimcEm->SetMarkerColor(kRed);  
    pSimcPm->SetMarkerColor(kRed);  
    pSimcPmz->SetMarkerColor(kRed);  
    
    CTRun *ct = new CTRun(firstRun,lastRun);
    if(!ct->fRunExist)
	return;
    //------------------------------------ Fill histograms from data ------- 
    TTree * tree = ct->GetTree();

    Int_t nEvents = tree->GetEntries();

    for (Int_t event = 0; event < nEvents; ++ event)
    {
	if(event%5000 == 0)
	    cout << (Int_t)(100*event/nEvents) <<" % Done"<< endl;

	tree->GetEntry(event);

	// PID & Kinematic Cut
	if( !(ct->fP_gtr_beta > 0.6 && ct->fP_gtr_beta < 1.4                             // proton beta cut
	      && ct->fH_gtr_beta > 0.8 &&  ct->fH_gtr_beta < 1.2                         // e beta cut
	      && ct->fH_cer_npeSum > 0.0 && ct->fP_hgcer_npeSum < 0.6                    // Cerenkov counter cut for e- and proton
	      && ct->fH_cal_etottracknorm > 0.6 && ct->fH_cal_etottracknorm < 2.0        // HMS Calorimeter cut 
   	      && ct->fH_cal_eprtracknorm > 0.1                                           // HMS Preshower cut
	      && fabs(ct->fP_kin_secondary_pmiss)<0.4                                    // Kinematic cut
	      && ct->fH_gtr_dp > -10 && ct->fH_gtr_dp < 10                               // e Delta cut
	      && ct->fP_gtr_dp > -15 && ct->fP_gtr_dp < 15))                             // p Delta cut	     	
	    continue;
	//--- reconstructed quantities ---
	//HMS
	hDelta->Fill(ct->fH_gtr_dp);
	hXptar->Fill(ct->fH_gtr_th);
	hYptar->Fill(ct->fH_gtr_ph);
	hYtar->Fill(ct->fH_gtr_y);
	//SHMS
	pDelta->Fill(ct->fP_gtr_dp + OffsetPdelta);
	pXptar->Fill(ct->fP_gtr_th);
	pYptar->Fill(ct->fP_gtr_ph);
	pYtar->Fill(ct->fP_gtr_y);
	
	//--- Kinematic quantities ----	
	hW->Fill(ct->fH_kin_primary_W);
	pEm->Fill(ct->fP_kin_secondary_emiss + OffsetEm);
	pPm->Fill(ct->fP_kin_secondary_pmiss);
	pPmz->Fill(ct->fP_kin_secondary_pmiss_z + OffsetPmz);	
    }

    //------------------ Plot from Simc --------------------------
    ct->AddSimc(SimcFileName);

    if(!ct->fSimcExist)
	return;
    
    TTree *simc = ct->GetSimcTree();

    Int_t nEventsSimc = simc->GetEntries();

    for (Int_t event = 0; event < nEventsSimc; ++event)
    {
	if(event%5000 == 0)
	    cout << (Int_t)(100*event/nEventsSimc) <<" % Done"<< endl;

	simc->GetEntry(event);
	//Kinematic Cut
	if( !(ct->fHSdelta > -10 && ct->fHSdelta < 10
	      && ct->fPSdelta > -15 && ct->fPSdelta < 15
	      && fabs(ct->fPm) < 0.4))
	    continue;	
	//------- Reconstructed quantities ------
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

	//------ Kinematic quanties ---------
	hSimcW->Fill(ct->fW, ct->fWeight);
	pSimcEm->Fill(ct->fEm, ct->fWeight);
	pSimcPm->Fill(fabs(ct->fPm), ct->fWeight);
	pSimcPmz->Fill(ct->fPmz, ct->fWeight);	
    }

    TText *textSimc = new TText();
    textSimc->SetTextColor(kRed);
    TText *textData = new TText();
    textData->SetTextColor(kBlue);
    
    TCanvas *c1 = new TCanvas("HMS","HMS");
    c1->Divide(2,2);

    c1->cd(1);
    hDelta->DrawNormalized("prof");
    hSimcDelta->DrawNormalized("same");
    // textSimc->DrawText(8.0,0.05,"Simc");
    // textData->DrawText(8.0,0.045,"Data");
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
    // textSimc->DrawText(8.0,0.05,"Simc");
    // textData->DrawText(8.0,0.045,"Data");
    c2->cd(2);
    pXptar->DrawNormalized("prof");
    pSimcXptar->DrawNormalized("same");
    c2->cd(3);
    pYptar->DrawNormalized("prof");
    pSimcYptar->DrawNormalized("same");
    c2->cd(4);
    pYtar->DrawNormalized("prof");
    pSimcYtar->DrawNormalized("same");

    c2->Print("Report.pdf","pdf");

    TCanvas *c3 = new TCanvas("Kinematic","Kinematic");
    c3->Divide(2,2);
    c3->cd(1);
    hW->DrawNormalized("prof");
    hSimcW->DrawNormalized("same");
    // textSimc->DrawText(1.6,0.07,"Simc");
    // textData->DrawText(1.6,0.065,"Data");
    c3->cd(2);
    pEm->DrawNormalized("prof");
    pSimcEm->DrawNormalized("same");
    c3->cd(3);
    pPm->DrawNormalized("prof");
    pSimcPm->DrawNormalized("same");
    c3->cd(4);
    pPmz->DrawNormalized("prof");
    pSimcPmz->DrawNormalized("same");

    c3->Print("Report.pdf)","pdf");
}
