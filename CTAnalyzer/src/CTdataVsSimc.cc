// Filename: CTdataVsSimc.cc
// Description: Compare Data vs Simc. This script exclusively for CT expt and assumes SHMS as proton arm and HMS as electron arm
// Author: Latif Kabir < latif@jlab.org >
// Created: Thu Jan  4 17:03:26 2018 (-0500)
// URL: jlab.org/~latif

/*
Quantities to compare:
delta, ytar, xptar, yptar for Both HMS and SHMS, for both data and Simc. Simc and data histos are super layed on each other. For kinematics Em, W, Pm, Pmz.
In order to compare data and simc histograms must have same range and binning and drawn normalized.
*/

#include <iostream>
#include <TFile.h>
#include <TTree.h>
#include <TString.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TCanvas.h>
#include <TSystem.h>
#include <TStyle.h>
#include <TText.h>
#include "Constants.h"
#include "CTRun.h"
using namespace std;

void CTdataVsSimc(Int_t firstRun, Int_t lastRun, TString SimcFileName, TString target)
{

    gStyle->SetOptStat(1); 
   //-----These offsets (for data) to be removed once SHMS is better calibrated ----------
    Double_t OffsetEm = 0.0;
    Double_t OffsetPdelta = 0.0;
    Double_t OffsetPmz = 0.0;
    Double_t OffsetW = 0.0;
    int cnts = 0;
    Double_t ebeam=10.6;
    Double_t etheta=-23.19*3.141592/180.;
    Double_t ptheta=21.6*3.141592/180.;
    Double_t eP0=5.539;
    Double_t pP0=5.925*0.985;
    Double_t mass_p=0.938272;
    Double_t mass_e=0.000511;
    Double_t hms_corsi, shms_corsi, ssim_corsi, hE, hP, pP, psimP, htheta, stheta, ssimtheta;
    Double_t r;
    //--------- Histograms from the data -----------
    //HMS
    //Reconstructed quantities
    TH1D *hDelta = new TH1D("hDelta","HMS DELTA (%); HMS dp (%); Normalized counts",100,-12,12);
    TH1D *hXptar = new TH1D("hXptar","HMS XPTAR (rad); x^{'}_{tar} (rad); Normalized counts",100,-0.1,0.1);
    TH1D *hYptar = new TH1D("hYptar","HMS YPTAR (rad); x^{'}_{tar} (rad); Normalized counts",100,-0.06,0.06);
    TH1D *hYtar = new TH1D("hYtar","HMS YTAR (cm); y_{tar} (cm); Normalized counts",100,-12.0,12.0);
    TH1D *hYtarc = new TH1D("hYtarc","HMS YTAR (cm); y_{tar} (cm); Normalized counts",100,-1.5,1.5);
    TH2D *pxxpfp = new TH2D("pxvsxp","SHMS x vs xp; x_fp (cm) wo cuts; xp_fp (rad)",100,-40,40,100,-0.1,0.1);
    TH2D *pxxpfp2 = new TH2D("pxvsxp2","SHMS x vs xp; x_fp (cm); xp_fp (rad)",100,-40,40,100,-0.1,0.1);
    TH2D *pxxpfp3 = new TH2D("pxvsxp3","SHMS x vs xp; x_fp (cm); xp_fp (rad)",100,-40,40,100,-0.1,0.1);
    //Kinematic quantities
    TH1D *hW = new TH1D("hW","W (GeV); W(GeV); Normalized counts", 150, 0.0, 2.0);
    //SHMS
    //Reconstructed quantities
    TH1D *pDelta = new TH1D("pDelta","SHMS DELTA (%); SHMS dp (%); Normalized counts",100,-12,12);
    TH1D *pXptar = new TH1D("pXptar","SHMS XPTAR (rad);x^{'}_{tar} (rad); Normalized counts",100,-0.06,0.06);
    TH1D *pYptar = new TH1D("pYptar","SHMS YPTAR (rad);y^{'}_{tar}(rad); Normalized counts",100,-0.06,0.06);
    TH1D *pYtar = new TH1D("pYtar","SHMS YTAR (cm); y_{tar} (cm); Normalized counts",100,-12.0,12.0);      
    TH1D *pYtarc = new TH1D("pYtarc","SHMS YTAR (cm); y_{tar} (cm); Normalized counts",100,-1.5,1.5);      
  
    //Kinematic quantities
    TH1D *pEm = new TH1D("pEm","Missing Energy (GeV); E_{m} (GeV); Normalized counts",200,-0.15,0.25);
    TH1D *pPm = new TH1D("pPm","Pm (GeV/c); |P_{m}|(GeV/c); Normalized counts", 100, 0.0, 0.6);
    TH1D *pPmz = new TH1D("pPmz","Pmz (GeV/c); P_{mz} (GeV/c); Normalized counts", 100, -0.4, 0.4);

    //--------- Histograms from the Simc -----------
    //HMS
    //Reconstructed quantities
    TH1D *hSimcDelta = new TH1D("hSimcDelta","HMS DELTA (SIMC) (%)",100,-12,12);
    TH1D *hSimcXptar = new TH1D("hSimcXptar","HMS XPTAR (SIMC) (rad)",100,-0.1,0.1);
    TH1D *hSimcYptar = new TH1D("hSimcYptar","HMS YPTAR (SIMC) (rad)",100,-0.06,0.06);
    TH1D *hSimcYtar = new TH1D("hSimcYtar","HMS YTAR (SIMC) (cm)",100,-12.0,12.0);
    TH1D *hSimcYtarc = new TH1D("hSimcYtarc","HMS YTAR (SIMC) (cm)",100,-1.5,1.5);
    //Kinematic quantities
    TH1D *hSimcW = new TH1D("hSimcW","W (GeV)", 150, 0.0, 2.0);
   
    hSimcDelta->SetMarkerColor(kRed);  
    hSimcXptar->SetMarkerColor(kRed);  
    hSimcYptar->SetMarkerColor(kRed);  
    hSimcYtar->SetMarkerColor(kRed);  
    hSimcYtarc->SetMarkerColor(kRed);  
    hSimcW->SetMarkerColor(kRed);  
    hSimcDelta->SetLineColor(kRed);  
    hSimcXptar->SetLineColor(kRed);  
    hSimcYptar->SetLineColor(kRed);  
    hSimcYtar->SetLineColor(kRed);  
    hSimcYtarc->SetLineColor(kRed);  
    hSimcW->SetLineColor(kRed);  

    //SHMS
    //Reconstructed quantities
    TH1D *pSimcDelta = new TH1D("pSimcDelta","SHMS DELTA (SIMC) (%)",100,-12,12);
    TH1D *pSimcXptar = new TH1D("pSimcXptar","SHMS XPTAR (SIMC) (rad)",100,-0.06,0.06);
    TH1D *pSimcYptar = new TH1D("pSimcYptar","SHMS YPTAR (SIMC) (rad)",100,-0.06,0.06);
    TH1D *pSimcYtar = new TH1D("pSimcYtar","SHMS YTAR (SIMC) (cm)",100,-12.0,12.0);
    TH1D *pSimcYtarc = new TH1D("pSimcYtarc","SHMS YTAR (SIMC) (cm)",100,-1.5,1.5);
    //Kinematic quantities
    TH1D *pSimcW = new TH1D("pSimcW","W (GeV)", 150, 0.0, 2.0);
    TH1D *pSimcEm = new TH1D("pSimcEm","Missing Energy (GeV)",200,-0.15,0.25);
    TH1D *pSimcPm = new TH1D("pSimcPm","|P_{m}| (GeV/c)", 100, 0.0, 0.6);
    TH1D *pSimcPmz = new TH1D("pSimcPmz","P_{mz} (GeV/c)", 100, -0.4, 0.4);

    TH1D *hcorsi = new TH1D("hcorsi","(E_hms - E')[GeV]; h_energy diff(GeV); Normalized counts", 100, -0.2, 0.2);
    TH1D *scorsi = new TH1D("scorsi","(P_shms - P')[GeV]; p_mom diff(GeV); Normalized counts", 100, -0.2, 0.2);
    TH1D *hSimcorsi = new TH1D("hSimcorsi","hmsE - E' (GeV); h_energy diff(GeV); Normalized counts", 100, -0.2, 0.2);
    TH1D *sSimcorsi = new TH1D("sSimcorsi","P_shms - P' (GeV); p_mom diff(GeV); Normalized counts", 100, -0.2, 0.2);


    hSimcorsi->SetMarkerColor(kRed);  
    sSimcorsi->SetMarkerColor(kRed);  
    pSimcDelta->SetMarkerColor(kRed);  
    pSimcXptar->SetMarkerColor(kRed);  
    pSimcYptar->SetMarkerColor(kRed);  
    pSimcYtar->SetMarkerColor(kRed);  
    pSimcYtarc->SetMarkerColor(kRed);  
    pSimcEm->SetMarkerColor(kRed);  
    pSimcPm->SetMarkerColor(kRed);  
    pSimcPmz->SetMarkerColor(kRed);  
    hSimcorsi->SetLineColor(kRed);  
    sSimcorsi->SetLineColor(kRed);  
    pSimcDelta->SetLineColor(kRed);  
    pSimcXptar->SetLineColor(kRed);  
    pSimcYptar->SetLineColor(kRed);  
    pSimcYtar->SetLineColor(kRed);  
    pSimcYtarc->SetLineColor(kRed);  
    pSimcEm->SetLineColor(kRed);  
    pSimcPm->SetLineColor(kRed);  
    pSimcPmz->SetLineColor(kRed);  
    
    CTRun *ct = new CTRun(firstRun,lastRun);
    if(!ct->fRunExist)
	return;
    //------------------------------------ Fill histograms from data ------- 
    TTree * tree = ct->GetTree();

    Int_t nEvents = tree->GetEntries();

    for (Int_t event = 0; event < nEvents; ++ event)
    {
	if(event%50000 == 0)
	    cout << (Int_t)(100*event/nEvents) <<" % Done"<< endl;

	tree->GetEntry(event);
        // 4/4 trigger cut to check consequences.
	//	if(!(ct->fP_hod_1xnhits > 0 && ct->fP_hod_1ynhits > 0 && ct->fP_hod_2xnhits > 0 && ct->fP_hod_2ynhits > 0))
	//	  continue;
        pxxpfp->Fill(ct->fP_dc_x_fp,ct->fP_dc_xp_fp);
	if (ct->fP_hod_2ynhits > 0) pxxpfp3->Fill(ct->fP_dc_x_fp,ct->fP_dc_xp_fp);
 
	// PID & Kinematic Cut
	if( !( ct->fP_gtr_beta > 0.6 && ct->fP_gtr_beta < 1.5    // proton beta cut
	       && ct->fH_gtr_beta > 0.8 &&  ct->fH_gtr_beta < 1.2                         // e beta cut
	      && ct->fH_cer_npeSum > 0. 
	      && ct->fP_hgcer_npeSum < 0.1                    // Cerenkov counter cut for e- and proton
	      && ct->fH_cal_etottracknorm > 0.6 && ct->fH_cal_etottracknorm < 2.0 // HMS Calorimeter cut 
              //&& ct->fP_cal_eprtrack > 0.                                          // SHMS pre-shower test only
	      //   	      && ct->fH_cal_eprtracknorm > 0.1                                           // HMS Preshower cut
	      //              && ct->fP_hod_2ynhits > 0                                                 // quart-bar hit	
	      && ct->fH_hod_goodstarttime == 1                                           // good hms start time
	      && ct->fP_hod_goodstarttime == 1                                           // good shms stat time
	      && ct->fH_gtr_dp > -10 && ct->fH_gtr_dp < 10                               // e Delta cut
	      && ct->fP_gtr_dp > -15 && ct->fP_gtr_dp < 15 
              ))                             // p Delta cut	     	
	      //              && ct->fH_gtr_y > -15 && ct->fH_gtr_y < 15))
	  //              && ct->fP_gtr_y > -15 && ct->fP_gtr_y < 15))                              // y-tar cut
		      continue;

	// Target specific kinematic cut
	if(target == "c")       // For carbon-12
	{
	  if(!(fabs(ct->fP_kin_secondary_pmiss)<0.4))                                    // Kinematic cut
	    continue;
	}
	else if(target == "h")  // For hydrogen-1
	{
	    if(!(ct->fH_kin_primary_W>=0.75 && ct->fH_kin_primary_W<=1.15))                // Kinematic cut
		continue;	    
	}
	else
	{
	    cout << "Invalid target type" <<endl;
	    return;
	}
        
	//--- reconstructed quantities ---
	/*        if (target == "c")
	 {
	   hYtar->GetXaxis()->SetRangeUser(-1.2,1.2);  
	   pYtar->GetXaxis()->SetRangeUser(-1.2,1.2);  
	   }*/
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
        pxxpfp2->Fill(ct->fP_dc_x_fp,ct->fP_dc_xp_fp);

        if (target == "h")
	 {
	   pPm->GetXaxis()->SetRangeUser(-0.15,0.25);
           pEm->GetXaxis()->SetRangeUser(-0.15,0.25);
         }

	//--- Kinematic quantities ----	
	hW->Fill(ct->fH_kin_primary_W + OffsetW);
	pPm->Fill(fabs(ct->fP_kin_secondary_pmiss));
	pPmz->Fill(ct->fP_kin_secondary_pmiss_z + OffsetPmz);

	if(target == "h")
	  {
	    pEm->Fill(ct->fP_kin_secondary_emiss + OffsetEm);
            hP = eP0*(1.+ (ct->fH_gtr_dp)/100.);
            hE = hP;
	    //            r = sqrt(1. + pow(ct->fH_gtr_th,2) + pow(ct->fH_gtr_ph,2));
            htheta = etheta + (ct->fH_gtr_ph);//acos((cos(etheta) + (ct->fH_gtr_ph)*sin(etheta))/r);
	    hms_corsi = hE - ebeam/(1.+ebeam*(1-cos(htheta))/mass_p);
            pP = pP0*(1.+ (ct->fP_gtr_dp)/100.);
            stheta = ptheta + (ct->fP_gtr_ph);
            shms_corsi = pP - 2.*mass_p*ebeam*cos(stheta)/(ebeam+mass_p)/(1-pow(ebeam*cos(stheta)/(ebeam+mass_p),2));
            hcorsi->Fill(hms_corsi);
            scorsi->Fill(shms_corsi);
          }       
	else
          {
	    pEm->Fill(ct->fP_kin_secondary_emiss_nuc + OffsetEm);
	    hYtarc->Fill(ct->fH_gtr_y);
	    pYtarc->Fill(ct->fP_gtr_y);
          }
        cnts++;
    }
    cout << cnts <<" events passed all cuts" << endl;


    //------------------ Plot from Simc --------------------------
    ct->AddSimc(SimcFileName);

    if(!ct->fSimcExist)
	return;
    
    TTree *simc = ct->GetSimcTree();

    Int_t nEventsSimc = simc->GetEntries();

    for (Int_t event = 0; event < nEventsSimc; ++event)
    {
	if(event%50000 == 0)
	    cout << (Int_t)(100*event/nEventsSimc) <<" % Done"<< endl;

	simc->GetEntry(event);
	//Kinematic Cut
	if( !(ct->fHSdelta > -10 && ct->fHSdelta < 10
	      && ct->fPSdelta > -15 && ct->fPSdelta < 15))
	    continue;	
	if(target == "c")       // For carbon-12
	{
	    if(!(fabs(ct->fPm)<0.4))                                    // Kinematic cut
		continue;
	}
	else if(target == "h")  // For hydrogen-1
	{
	    if(!(ct->fW>=0.75 && ct->fW<=1.15))                // Kinematic cut
		continue;	    
	}

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

	if (target == "c")
	 {
	  hSimcYtarc->Fill(ct->fHSytar, ct->fWeight);
	  pSimcYtarc->Fill(ct->fPSytar, ct->fWeight);
	 }
        

        if (target == "h")
	 {
	   pSimcPm->GetXaxis()->SetRangeUser(-0.15,0.25);
           pSimcEm->GetXaxis()->SetRangeUser(-0.15,0.25);
           psimP = pP0*(1.+ (ct->fPSdelta)/100.);
           ssimtheta = ptheta + (ct->fPSyptar);
           ssim_corsi = psimP - 2.*mass_p*ebeam*cos(ssimtheta)/(ebeam+mass_p)/(1-pow(ebeam*cos(ssimtheta)/(ebeam+mass_p),2));        
           sSimcorsi->Fill(ssim_corsi,ct->fWeight);
           hSimcorsi->Fill(ct->fHcorsi,ct->fWeight);
         
         }
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
    if(target == "c")       // For carbon-12
     {
        hSimcYtarc->DrawNormalized();
        hYtarc->DrawNormalized("prof same");
     }
    else if(target == "h")  // For hydrogen-1
     {
        hSimcYtar->DrawNormalized();
        hYtar->DrawNormalized("prof same");
     }

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
    if(target == "c")       // For carbon-12
     {
        pSimcYtarc->DrawNormalized();
        pYtarc->DrawNormalized("prof same");
     }
    else if(target == "h")  // For hydrogen-1
     {
        pSimcYtar->DrawNormalized();
        pYtar->DrawNormalized("prof same");
     }

    c2->Print("Report.pdf","pdf");

    TCanvas *c3 = new TCanvas("Kinematic","Kinematic");
    c3->Divide(2,2);
    c3->cd(1);
    hW->DrawNormalized("prof");
    //    hW->Fit("gaus");
    hSimcW->DrawNormalized("same");
    // textSimc->DrawText(1.6,0.07,"Simc");
    // textData->DrawText(1.6,0.065,"Data");
    c3->cd(2);
    pSimcEm->DrawNormalized();
    pEm->DrawNormalized("prof same");
    c3->cd(3);
    pPm->DrawNormalized("prof");
    pSimcPm->DrawNormalized("same");
    c3->cd(4);
    pSimcPmz->DrawNormalized();
    pPmz->DrawNormalized("prof same");

    if(target == "c")       // For carbon-12
     {
      c3->Print("Report.pdf)","pdf");
     }
    else if(target == "h")  // For hydrogen-1
     {
 
       c3->Print("Report.pdf","pdf");

       TCanvas *c4 = new TCanvas("Singles","Singles");
       c4->Divide(2,2);

       c4->cd(1);
       hSimcorsi->DrawNormalized("L");
       hcorsi->DrawNormalized("prof same");
    // textSimc->DrawText(8.0,0.05,"Simc");
    // textData->DrawText(8.0,0.045,"Data");
       c4->cd(2);
       sSimcorsi->DrawNormalized("L");
       scorsi->DrawNormalized("prof same");

       c4->Print("Report.pdf)","pdf");
     }

    TCanvas *c5 = new TCanvas("SHMS_fp","SHMS_fp");
    c5->Divide(3,1);
    c5->cd(1);
    pxxpfp->Draw("COLZ");
    c5->cd(2);
    pxxpfp3->Draw("COLZ");
    c5->cd(3);
    pxxpfp2->Draw("COLZ");
    c5->Print("Pxxpfp.pdf","pdf");

}
