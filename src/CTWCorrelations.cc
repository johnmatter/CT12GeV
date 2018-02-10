// Filename: CTWCorrelations.cc
// Description: Look at different correlations of kinematics and reconstructed quantities against W
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

void CTWCorrelations(Int_t firstRun, Int_t lastRun, TString target)
{
    //--------- Histograms from the data -----------
    //HMS
    //Reconstructed quantities
    TH2D *hDeltaVsW = new TH2D("hDeltaVsW","HMS DELTA (%) vs W; W (GeV); HMS dp (%)", 100, 0.6, 1.3, 100,-12,12);
    TH2D *hXptarVsW = new TH2D("hXptarVsW","HMS XPTAR (rad) vs W; W (GeV); x^{'}_{tar} (rad) ", 100, 0.6, 1.3, 100,-0.1,0.1);
    TH2D *hYptarVsW = new TH2D("hYptarVsW","HMS YPTAR (rad) vs W; W (GeV); x^{'}_{tar} (rad) ", 100, 0.6, 1.3, 100,-0.06,0.06);
    TH2D *hYtarVsW = new TH2D("hYtarVsW","HMS YTAR (cm) vs W; W (GeV); y_{tar} (cm) ", 100, 0.6, 1.3, 100,-12.0,12.0);

    TH2D *hXfpVsW = new TH2D("hXfpVsW","HMS Xfp (cm )vs W; W (GeV); X_{fp} (cm)", 100, 0.6, 1.3, 100,-30.5, 30.5);
    TH2D *hXpfpVsW = new TH2D("hXpfpVsW","HMS Xpfp (rad) vs W; W (GeV); X^{'}_{fp} (rad) ", 100, 0.6, 1.3, 100,-0.1,0.1);
    TH2D *hYfpVsW = new TH2D("hYfpVsW","HMS Yfp (cm) vs W; W (GeV); Y_{fp}  (cm)", 100, 0.6, 1.3, 100,-20.6,20.6);
    TH2D *hYpfpVsW = new TH2D("hYpfpVsW","HMS Ypfp (rad) vs W; W (GeV); Y^{'}_{fp} (rad) ", 100, 0.6, 1.3, 100,-0.04,0.04);
    
    //Kinematic quantities    
    TH2D *hEmVsW = new TH2D("hdPVsW","Em vs W; W (GeV); E_{m}", 150, 0.6, 1.3, 150, -0.15, 0.25);
    TH2D *hdPVsW = new TH2D("hdPVsW"," HMS dP vs W; W (GeV); HMS dP (%)", 150, 0.6, 1.3, 150, -8, 8);
    TH1D *hW = new TH1D("hW","W (GeV); W (GeV); counts",200,0.6,1.3);
    
    //SHMS
    //Reconstructed quantities
    TH2D *pDeltaVsW = new TH2D("pDeltaVsW","SHMS DELTA (%) vs W; W (GeV); SHMS dp (%)", 100, 0.6, 1.3, 100,-12,12);
    TH2D *pXptarVsW = new TH2D("pXptarVsW","SHMS XPTAR (rad) vs W; W (GeV); x^{'}_{tar} (rad)", 100, 0.6, 1.3, 100,-0.06,0.06);
    TH2D *pYptarVsW = new TH2D("pYptarVsW","SHMS YPTAR (rad) vs W; W (GeV); y^{'}_{tar}(rad)", 100, 0.6, 1.3, 100,-0.06,0.06);
    TH2D *pYtarVsW = new TH2D("pYtarVsW","SHMS YTAR (cm) vs W; W (GeV); y_{tar} (cm)", 100, 0.6, 1.3, 100,-12.0,12.0);

    TH2D *pXfpVsW = new TH2D("pXfpVsW","SHMS Xfp (cm) vs W; W (GeV); X_{fp} (cm)", 100, 0.6, 1.3, 100,-8.5, 8.5);
    TH2D *pXpfpVsW = new TH2D("pXpfpVsW","SHMS Xpfp (rad) vs W; W (GeV); X^{'}_{fp} (rad) ", 100, 0.6, 1.3, 100,-0.1,0.1);
    TH2D *pYfpVsW = new TH2D("pYfpVsW","SHMS Yfp (cm) vs W; W (GeV); Y_{fp}  (cm)", 100, 0.6, 1.3, 100,-8.0,8.0);
    TH2D *pYpfpVsW = new TH2D("pYpfpVsW","SHMS Ypfp (rad) vs W; W (GeV); Y^{'}_{fp} (rad) ", 100, 0.6, 1.3, 100,-0.1,0.1);
    
    //Kinematic quantities    
    TH2D *pdPVsW = new TH2D("pdPVsW","SHMS dP vs W; W; dP", 150, 0.6, 1.3, 150, -12, 12);
    TH2D *PeVsPp = new TH2D("PeVsPp","P_{e} vs P_{p}; P_{p} (GeV/c); P_{e} (GeV/c)", 150, -12, 12, 150, -12, 12);
    
    CTRun *ct = new CTRun(firstRun,lastRun);
    if(!ct->fRunExist)
	return;
    //------------------------------------ Fill histograms from data ------- 
    TTree * tree = ct->GetTree();

    Int_t nEvents = tree->GetEntries();
    Double_t Wkin;
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
	Wkin = ct->fH_kin_primary_W;
	//--- reconstructed quantities ---
	//HMS
	hDeltaVsW->Fill(Wkin, ct->fH_gtr_dp);
	hXptarVsW->Fill(Wkin, ct->fH_gtr_th);
	hYptarVsW->Fill(Wkin, ct->fH_gtr_ph);
	hYtarVsW->Fill(Wkin, ct->fH_gtr_y);

	hXfpVsW->Fill(Wkin, ct->fH_dc_x_fp);
	hXpfpVsW->Fill(Wkin, ct->fH_dc_xp_fp);
	hYfpVsW->Fill(Wkin, ct->fH_dc_y_fp);
	hYpfpVsW->Fill(Wkin, ct->fH_dc_yp_fp);
	
	//SHMS
	pDeltaVsW->Fill(Wkin, ct->fP_gtr_dp);
	pXptarVsW->Fill(Wkin, ct->fP_gtr_th);
	pYptarVsW->Fill(Wkin, ct->fP_gtr_ph);
	pYtarVsW->Fill(Wkin, ct->fP_gtr_y);

	pXfpVsW->Fill(Wkin, ct->fP_dc_x_fp);
	pXpfpVsW->Fill(Wkin, ct->fP_dc_xp_fp);
	pYfpVsW->Fill(Wkin, ct->fP_dc_y_fp);
	pYpfpVsW->Fill(Wkin, ct->fP_dc_yp_fp);
	
	//--- Kinematic quantities ----	
	hdPVsW->Fill(Wkin, ct->fH_gtr_dp);
	pdPVsW->Fill(Wkin, ct->fP_gtr_dp);
	PeVsPp->Fill(ct->fP_gtr_dp, ct->fH_gtr_dp);
	hEmVsW->Fill(Wkin, Emiss);

	hW->Fill(Wkin);
    }

    TCanvas *c0 = new TCanvas("W","W");
    c0->SetGrid();
    hW->Draw();
    c0->Print("Report.pdf(","pdf");

    
    TCanvas *c1 = new TCanvas("HMS","HMS");
    c1->SetGrid();
    c1->Divide(2,2);

    c1->cd(1);
    hDeltaVsW->Draw("colz");
    c1->cd(2);
    hXptarVsW->Draw("colz");
    c1->cd(3);
    hYptarVsW->Draw("colz");
    c1->cd(4);
    hYtarVsW->Draw("colz");

    c1->Print("Report.pdf","pdf");
    
    TCanvas *c2 = new TCanvas("SHMS","SHMS");
    c2->SetGrid();
    c2->Divide(2,2);

    c2->cd(1);
    pDeltaVsW->Draw("colz");
    c2->cd(2);
    pXptarVsW->Draw("colz");
    c2->cd(3);
    pYptarVsW->Draw("colz");
    c2->cd(4);
    pYtarVsW->Draw("colz");

    c2->Print("Report.pdf","pdf");

    TCanvas *c3 = new TCanvas("Corr HMS","Corr HMS");
    c3->SetGrid();
    c3->Divide(2,2);
    c3->cd(1);
    hXfpVsW->Draw("colz");
    c3->cd(2);
    hXpfpVsW->Draw("colz");
    c3->cd(3);
    hYfpVsW->Draw("colz");
    c3->cd(4);
    hYpfpVsW->Draw("colz");
    c3->Print("Report.pdf","pdf");

    TCanvas *c4 = new TCanvas("Corr SHMS","Corr SHMS");
    c4->SetGrid();
    c4->Divide(2,2);
    c4->cd(1);
    pXfpVsW->Draw("colz");
    c4->cd(2);
    pXpfpVsW->Draw("colz");
    c4->cd(3);
    pYfpVsW->Draw("colz");
    c4->cd(4);
    pYpfpVsW->Draw("colz");
    c4->Print("Report.pdf","pdf");
    
    TCanvas *c5 = new TCanvas("Kinematic","Kinematic");
    c5->SetGrid();
    c5->Divide(2,1);
    c5->cd(1);
    pdPVsW->Draw("colz");
    c5->cd(2);
    PeVsPp->Draw("colz");
    c5->Print("Report.pdf","pdf");

    TCanvas *c6 = new TCanvas("dPVsW","HMS dp vs W");
    c6->SetGrid();
    hdPVsW->Draw("colz");
    c6->Print("Report.pdf)","pdf");
}
