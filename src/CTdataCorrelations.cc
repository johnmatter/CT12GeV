// Filename: CTdataCorrelations.cc
// Description:Lokk at different correlations of kinematioc and reconstructed quantities
// Author: Latif Kabir < latif@jlab.org >
// Created: Thu Jan  4 17:03:26 2018 (-0500)
// URL: jlab.org/~latif

#include <iostream>
#include <TFile.h>
#include <TTree.h>
#include <TString.h>
#include <TH2D.h>
#include <TH2D.h>
#include <TCanvas.h>
#include <TSystem.h>
#include <TStyle.h>
#include <TText.h>
#include "Constants.h"
#include "CTRun.h"
using namespace std;

void CTdataCorrelations(Int_t firstRun, Int_t lastRun, TString target)
{
    //--------- Histograms from the data -----------
    //HMS
    //Reconstructed quantities
    TH2D *hDeltaVsEm = new TH2D("hDeltaVsEm","HMS DELTA (%) vs E_{m}; E_{m} (GeV); HMS dp (%)", 100, -0.15, 0.25, 100,-12,12);
    TH2D *hXptarVsEm = new TH2D("hXptarVsEm","HMS XPTAR (rad) vs E_{m}; E_{m} (GeV); x^{'}_{tar} (rad) ", 100, -0.15, 0.25, 100,-0.1,0.1);
    TH2D *hYptarVsEm = new TH2D("hYptarVsEm","HMS YPTAR (rad) vs E_{m}; E_{m} (GeV); x^{'}_{tar} (rad) ", 100, -0.15, 0.25, 100,-0.06,0.06);
    TH2D *hYtarVsEm = new TH2D("hYtarVsEm","HMS YTAR (cm) vs E_{m}; E_{m} (GeV); y_{tar} (cm) ", 100, -0.15, 0.25, 100,-12.0,12.0);

    TH2D *hXfpVsEm = new TH2D("hXfpVsEm","HMS Xfp (cm )vs E_{m}; E_{m} (GeV); X_{fp} (cm)", 100, -0.15, 0.25, 100,-30.5, 30.5);
    TH2D *hXpfpVsEm = new TH2D("hXpfpVsEm","HMS Xpfp (rad) vs E_{m}; E_{m} (GeV); X^{'}_{fp} (rad) ", 100, -0.15, 0.25, 100,-0.1,0.1);
    TH2D *hYfpVsEm = new TH2D("hYfpVsEm","HMS Yfp (cm) vs E_{m}; E_{m} (GeV); Y_{fp}  (cm)", 100, -0.15, 0.25, 100,-20.6,20.6);
    TH2D *hYpfpVsEm = new TH2D("hYpfpVsEm","HMS Ypfp (rad) vs E_{m}; E_{m} (GeV); Y^{'}_{fp} (rad) ", 100, -0.15, 0.25, 100,-0.04,0.04);
    
    //Kinematic quantities    
    TH2D *hdPVsEm = new TH2D("hdPVsEm","dP vs E_{m}; E_{m} (GeV); dP (%)", 150, -0.15, 0.25, 150, -12, 12);
    TH2D *hdPVsW = new TH2D("hdPVsW"," HMS dP vs W; W (GeV); HMS dP (%)", 150, 0.6, 1.3, 150, -8, 8);
    
    //SHMS
    //Reconstructed quantities
    TH2D *pDeltaVsEm = new TH2D("pDeltaVsEm","SHMS DELTA (%) vs E_{m}; E_{m} (GeV); SHMS dp (%)", 100, -0.15, 0.25, 100,-12,12);
    TH2D *pXptarVsEm = new TH2D("pXptarVsEm","SHMS XPTAR (rad) vs E_{m}; E_{m} (GeV); x^{'}_{tar} (rad)", 100, -0.15, 0.25, 100,-0.06,0.06);
    TH2D *pYptarVsEm = new TH2D("pYptarVsEm","SHMS YPTAR (rad) vs E_{m}; E_{m} (GeV); y^{'}_{tar}(rad)", 100, -0.15, 0.25, 100,-0.06,0.06);
    TH2D *pYtarVsEm = new TH2D("pYtarVsEm","SHMS YTAR (cm) vs E_{m}; E_{m} (GeV); y_{tar} (cm)", 100, -0.15, 0.25, 100,-12.0,12.0);

    TH2D *pXfpVsEm = new TH2D("pXfpVsEm","SHMS Xfp (cm) vs E_{m}; E_{m} (GeV); X_{fp} (cm)", 100, -0.15, 0.25, 100,-8.5, 8.5);
    TH2D *pXpfpVsEm = new TH2D("pXpfpVsEm","SHMS Xpfp (rad) vs E_{m}; E_{m} (GeV); X^{'}_{fp} (rad) ", 100, -0.15, 0.25, 100,-0.1,0.1);
    TH2D *pYfpVsEm = new TH2D("pYfpVsEm","SHMS Yfp (cm) vs E_{m}; E_{m} (GeV); Y_{fp}  (cm)", 100, -0.15, 0.25, 100,-8.0,8.0);
    TH2D *pYpfpVsEm = new TH2D("pYpfpVsEm","SHMS Ypfp (rad) vs E_{m}; E_{m} (GeV); Y^{'}_{fp} (rad) ", 100, -0.15, 0.25, 100,-0.1,0.1);
    
    //Kinematic quantities    
    TH2D *pdPVsEm = new TH2D("pdPVsEm","dP vs E_{m}; E_{m}; dP", 150, -0.15, 0.25, 150, -12, 12);
    TH2D *PeVsPp = new TH2D("PeVsPp","P_{e} vs P_{p}; P_{p} (GeV/c); P_{e} (GeV/c)", 150, -12, 12, 150, -12, 12);


    TH1D *pEm = new TH1D("pEm","Missing Energy (GeV); E_{m} (GeV); counts",200,-0.15,0.25);
    
    CTRun *ct = new CTRun(firstRun,lastRun);
    if(!ct->fRunExist)
	return;
    //------------------------------------ Fill histograms from data ------- 
    TTree * tree = ct->GetTree();

    Int_t nEvents = tree->GetEntries();
    Double_t Emiss;
    
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
	      && ct->fH_gtr_dp > -10 && ct->fH_gtr_dp < 10                               // e Delta cut
	      && ct->fP_gtr_dp > -15 && ct->fP_gtr_dp < 15))                             // p Delta cut	     	
	    continue;

	// Target specific kinematic cut
	if(target == "c")       // For carbon-12
	{
	    if(!(fabs(ct->fP_kin_secondary_pmiss)<0.4))                                    // Kinematic cut
		continue;
	    Emiss = ct->fP_kin_secondary_emiss_nuc;
	}
	else if(target == "h")  // For hydrogen-1
	{
	    if(!(ct->fH_kin_primary_W>=0.75 && ct->fH_kin_primary_W<=1.15))                // Kinematic cut
		continue;
	    Emiss = ct->fP_kin_secondary_emiss;
	}
	else
	{
	    cout << "Invalid target type" <<endl;
	    return;
	}
	//--- reconstructed quantities ---
	//HMS
	hDeltaVsEm->Fill(Emiss, ct->fH_gtr_dp);
	hXptarVsEm->Fill(Emiss, ct->fH_gtr_th);
	hYptarVsEm->Fill(Emiss, ct->fH_gtr_ph);
	hYtarVsEm->Fill(Emiss, ct->fH_gtr_y);

	hXfpVsEm->Fill(Emiss, ct->fH_dc_x_fp);
	hXpfpVsEm->Fill(Emiss, ct->fH_dc_xp_fp);
	hYfpVsEm->Fill(Emiss, ct->fH_dc_y_fp);
	hYpfpVsEm->Fill(Emiss, ct->fH_dc_yp_fp);
	
	//SHMS
	pDeltaVsEm->Fill(Emiss, ct->fP_gtr_dp);
	pXptarVsEm->Fill(Emiss, ct->fP_gtr_th);
	pYptarVsEm->Fill(Emiss, ct->fP_gtr_ph);
	pYtarVsEm->Fill(Emiss, ct->fP_gtr_y);

	pXfpVsEm->Fill(Emiss, ct->fP_dc_x_fp);
	pXpfpVsEm->Fill(Emiss, ct->fP_dc_xp_fp);
	pYfpVsEm->Fill(Emiss, ct->fP_dc_y_fp);
	pYpfpVsEm->Fill(Emiss, ct->fP_dc_yp_fp);
	
	//--- Kinematic quantities ----	
	hdPVsEm->Fill(Emiss, ct->fH_gtr_dp);
	pdPVsEm->Fill(Emiss, ct->fP_gtr_dp);
	PeVsPp->Fill(ct->fP_gtr_dp, ct->fH_gtr_dp);
	hdPVsW->Fill(ct->fH_kin_primary_W, ct->fH_gtr_dp);

	pEm->Fill(Emiss);
    }

    
    TCanvas *c1 = new TCanvas("HMS","HMS");
    c1->Divide(2,2);

    c1->cd(1);
    hDeltaVsEm->Draw("colz");
    c1->cd(2);
    hXptarVsEm->Draw("colz");
    c1->cd(3);
    hYptarVsEm->Draw("colz");
    c1->cd(4);
    hYtarVsEm->Draw("colz");

    c1->Print("Report.pdf(","pdf");
    
    TCanvas *c2 = new TCanvas("SHMS","SHMS");
    c2->Divide(2,2);

    c2->cd(1);
    pDeltaVsEm->Draw("colz");
    c2->cd(2);
    pXptarVsEm->Draw("colz");
    c2->cd(3);
    pYptarVsEm->Draw("colz");
    c2->cd(4);
    pYtarVsEm->Draw("colz");

    c2->Print("Report.pdf","pdf");

    TCanvas *c3 = new TCanvas("Corr HMS","Corr HMS");
    c3->Divide(2,2);
    c3->cd(1);
    hXfpVsEm->Draw("colz");
    c3->cd(2);
    hXpfpVsEm->Draw("colz");
    c3->cd(3);
    hYfpVsEm->Draw("colz");
    c3->cd(4);
    hYpfpVsEm->Draw("colz");
    c3->Print("Report.pdf","pdf");

    TCanvas *c4 = new TCanvas("Corr SHMS","Corr SHMS");
    c4->Divide(2,2);
    c4->cd(1);
    pXfpVsEm->Draw("colz");
    c4->cd(2);
    pXpfpVsEm->Draw("colz");
    c4->cd(3);
    pYfpVsEm->Draw("colz");
    c4->cd(4);
    pYpfpVsEm->Draw("colz");
    c4->Print("Report.pdf","pdf");
    
    TCanvas *c5 = new TCanvas("Kinematic","Kinematic");
    c5->Divide(2,1);
    c5->cd(1);
    pdPVsEm->Draw("colz");
    c5->cd(2);
    PeVsPp->Draw("colz");
    c5->Print("Report.pdf","pdf");

    TCanvas *c6 = new TCanvas("MissEng","Miss Eng");
    pEm->Draw();
    c6->Print("Report.pdf","pdf");

    TCanvas *c7 = new TCanvas("dPVsW","HMS dp vs W");
    c7->SetGrid();
    hdPVsW->Draw("colz");
    c7->Print("Report.pdf)","pdf");
}
