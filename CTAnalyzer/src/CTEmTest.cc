// Filename: CTEmTest.cc
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

void CTEmTest(Int_t firstRun, Int_t lastRun, TString SimcFileName)
{

    gStyle->SetOptStat(1); 
   //-----These offsets (for data) to be removed once SHMS is betetr calibrated ----------
    Double_t OffsetEm = 0.0;
    Double_t OffsetPdelta = 0.0;
    Double_t OffsetPmz = 0.0;
    Double_t OffsetW = 0.0;
    int cnts = 0;
    Double_t mass_p=0.938272;
    Double_t mass_e=0.000511;
    //--------- Histograms from the data -----------
    //HMS
    //Reconstructed quantities
    TH2D *Emhdelta = new TH2D("Emhdelta","Em vs HMS delta; HMS delta (%); Em (GeV)",100,-12,12,100,-0.15,0.5);
    TH2D *Empdelta = new TH2D("Empdelta","Em vs SHMS delta; SHMS delta (%); Em (GeV)",100,-17,17,100,-0.15,0.5);
    TH2D *Emhtheta = new TH2D("Emhtheta","Em vs HMS yptar; HMS yptar (rad); Em (GeV)",100,-0.05,0.05,100,-0.15,0.5);
    TH2D *Emptheta = new TH2D("Emptheta","Em vs SHMS yptar; SHMS yptar (rad); Em (GeV)",100,-0.05,0.05,100,-0.15,0.5);
    TH2D *Emhxptar = new TH2D("Emhxptar","Em vs xptar; HMS xptar (rad); Em (GeV)",100,-0.1,0.1,100,-0.15,0.5);
    TH2D *Empxptar = new TH2D("Empxptar","Em vs xptar; SHMS xptar (rad); Em (GeV)",100,-0.1,0.1,100,-0.15,0.5);
    TH2D *Empxfp = new TH2D("Empxfp","Em vs xfp; SHMS xfp (cm); Em (GeV)",100,-20,20,100,-0.15,0.5);
    TH2D *Empxpfp = new TH2D("Empxpfp","Em vs xpfp; SHMS xpfp (rad); Em (GeV)",100,-0.06,0.06,100,-0.15,0.5);
    TH2D *Empyfp = new TH2D("Empyfp","Em vs yfp; SHMS yfp (cm); Em (GeV)",100,-15,15,100,-0.15,0.5);
    TH2D *Empypfp = new TH2D("Empypfp","Em vs ypfp; SHMS ypfp (rad); Em (GeV)",100,-0.035,0.035,100,-0.15,0.5);
    TH2D *Emhxfp = new TH2D("Emhxfp","Em vs xfp; HMS xfp (cm); Em (GeV)",100,-25,25,100,-0.15,0.5);
    TH2D *Emhxpfp = new TH2D("Emhxpfp","Em vs xpfp; HMS xpfp (rad); Em (GeV)",100,-0.065,0.065,100,-0.15,0.5);
    TH2D *Emhyfp = new TH2D("Emhyfp","Em vs yfp; HMS yfp (cm); Em (GeV)",100,-15,20,100,-0.15,0.5);
    TH2D *Emhypfp = new TH2D("Emhypfp","Em vs ypfp; HMS ypfp (rad); Em (GeV)",100,-0.035,0.035,100,-0.15,0.5);

 

    Emhdelta->SetMarkerColor(kBlue);  
    Emhtheta->SetMarkerColor(kBlue);  
    Emhxptar->SetMarkerColor(kBlue);  
    Empxptar->SetMarkerColor(kBlue);  
    Emptheta->SetMarkerColor(kBlue);  
    Empdelta->SetMarkerColor(kBlue);  
    Empxfp->SetMarkerColor(kBlue);  
    Empxpfp->SetMarkerColor(kBlue);  
    Empyfp->SetMarkerColor(kBlue);  
    Empypfp->SetMarkerColor(kBlue);  
    Emhxfp->SetMarkerColor(kBlue);  
    Emhxpfp->SetMarkerColor(kBlue);  
    Emhyfp->SetMarkerColor(kBlue);  
    Emhypfp->SetMarkerColor(kBlue);  
 
    Emhdelta->SetLineColor(kBlue);  
    Emhtheta->SetLineColor(kBlue);  
    Emhxptar->SetLineColor(kBlue);  
    Empxptar->SetLineColor(kBlue);  
    Emptheta->SetLineColor(kBlue);  
    Empdelta->SetLineColor(kBlue);  
    Empxfp->SetLineColor(kBlue);  
    Empxpfp->SetLineColor(kBlue);  
    Empyfp->SetLineColor(kBlue);  
    Empypfp->SetLineColor(kBlue);  
    Emhxfp->SetLineColor(kBlue);  
    Emhxpfp->SetLineColor(kBlue);  
    Emhyfp->SetLineColor(kBlue);  
    Emhypfp->SetLineColor(kBlue);  

    //SIMC
    //Reconstructed quantities
    TH2D *sEmhdelta = new TH2D("sEmhdelta","SIMC Em vs HMS delta; HMS delta (%); Em (GeV)",100,-12,12,100,-0.15,0.5);
    TH2D *sEmpdelta = new TH2D("sEmpdelta","SIMC Em vs SHMS delta; SHMS delta (%); Em (GeV)",100,-17,17,100,-0.15,0.5);
    TH2D *sEmhtheta = new TH2D("sEmhtheta","SIMC Em vs HMS yptar; HMS yptar (rad); Em (GeV)",100,-0.05,0.05,100,-0.15,0.5);
    TH2D *sEmptheta = new TH2D("sEmptheta","SIMC Em vs SHMS yptar; SHMS yptar (rad); Em (GeV)",100,-0.05,0.05,100,-0.15,0.5);
    TH2D *sEmhxptar = new TH2D("sEmhxptar","SIMC Em vs xptar; HMS xptar (rad); Em (GeV)",100,-0.1,0.1,100,-0.15,0.5);
    TH2D *sEmpxptar = new TH2D("sEmpxptar","SIMC Em vs xptar; SHMS xptar (rad); Em (GeV)",100,-0.1,0.1,100,-0.15,0.5);
    TH2D *sEmpxfp = new TH2D("sEmpxfp","Em vs SHMS xfp; SHMS xfp (cm); Em (GeV)",100,-20,20,100,-0.15,0.5);
    TH2D *sEmpxpfp = new TH2D("sEmpxpfp","Em vs SHMS xpfp; SHMS xpfp (rad); Em (GeV)",100,-0.06,0.06,100,-0.15,0.5);
    TH2D *sEmpyfp = new TH2D("sEmpyfp","Em vs SHMS yfp; SHMS yfp (cm); Em (GeV)",100,-15,15,100,-0.15,0.5);
    TH2D *sEmpypfp = new TH2D("sEmpypfp","Em vs SHMS ypfp; SHMS ypfp (rad); Em (GeV)",100,-0.035,0.035,100,-0.15,0.5);
    TH2D *sEmhxfp = new TH2D("sEmhxfp","Em vs HMS xfp; HMS xfp (cm); Em (GeV)",100,-25,25,100,-0.15,0.5);
    TH2D *sEmhxpfp = new TH2D("sEmhxpfp","Em vs HMS xpfp; HMS xpfp (rad); Em (GeV)",100,-0.065,0.065,100,-0.15,0.5);
    TH2D *sEmhyfp = new TH2D("sEmhyfp","Em vs HMS yfp; HMS yfp (cm); Em (GeV)",100,-15,20,100,-0.15,0.5);
    TH2D *sEmhypfp = new TH2D("sEmhypfp","Em vs HMS ypfp; HMS ypfp (rad); Em (GeV)",100,-0.035,0.035,100,-0.15,0.5);


    sEmhdelta->SetMarkerColor(kRed);  
    sEmhtheta->SetMarkerColor(kRed);  
    sEmhxptar->SetMarkerColor(kRed);  
    sEmpxptar->SetMarkerColor(kRed);  
    sEmptheta->SetMarkerColor(kRed);  
    sEmpdelta->SetMarkerColor(kRed);  
    sEmpxfp->SetMarkerColor(kRed);  
    sEmpxpfp->SetMarkerColor(kRed);  
    sEmpyfp->SetMarkerColor(kRed);  
    sEmpypfp->SetMarkerColor(kRed);  
    sEmhxfp->SetMarkerColor(kRed);  
    sEmhxpfp->SetMarkerColor(kRed);  
    sEmhyfp->SetMarkerColor(kRed);  
    sEmhypfp->SetMarkerColor(kRed);  

    sEmhdelta->SetLineColor(kRed);  
    sEmhtheta->SetLineColor(kRed);  
    sEmhxptar->SetLineColor(kRed);  
    sEmpxptar->SetLineColor(kRed);  
    sEmptheta->SetLineColor(kRed);  
    sEmpdelta->SetLineColor(kRed);  
    sEmpxfp->SetLineColor(kRed);  
    sEmpxpfp->SetLineColor(kRed);  
    sEmpyfp->SetLineColor(kRed);  
    sEmpypfp->SetLineColor(kRed);  
    sEmhxfp->SetLineColor(kRed);  
    sEmhxpfp->SetLineColor(kRed);  
    sEmhyfp->SetLineColor(kRed);  
    sEmhypfp->SetLineColor(kRed);  
  



    
    CTRun *ct = new CTRun(firstRun,lastRun);
    if(!ct->fRunExist)
	return;
    //------------------------------------ Fill histograms from data ------- 
    TTree * tree = ct->GetTree();

    Int_t nEvents = tree->GetEntries();
    cout << "there are  "<< nEvents << " events " << endl; 
    for (Int_t event = 0; event < nEvents; ++ event)
    {
	if(event%50000 == 0)
	    cout << (Int_t)(100*event/nEvents) <<" % Done"<< endl;

	tree->GetEntry(event);
        // 4/4 trigger cut to check consequences.
	//        if(!(ct->fP_hod_1xnhits > 0 && ct->fP_hod_1ynhits > 0 && ct->fP_hod_2xnhits > 0 && ct->fP_hod_2ynhits > 0))
	//	  continue;
	// PID & Kinematic Cut
	if( !(ct->fP_gtr_beta > 0.6 && ct->fP_gtr_beta < 1.4                             // proton beta cut
	      && ct->fH_gtr_beta > 0.8 &&  ct->fH_gtr_beta < 1.2                         // e beta cut
	      && ct->fH_cer_npeSum > 0.1 && ct->fP_hgcer_npeSum < 0.1                    // Cerenkov counter cut for e- and proton
	      && ct->fH_cal_etottracknorm > 0.6 && ct->fH_cal_etottracknorm < 2.0        // HMS Calorimeter cut 
              //&& ct->fP_cal_eprtrack > 0.                                          // SHMS pre-shower test only
	      //   	      && ct->fH_cal_eprtracknorm > 0.1                                           // HMS Preshower cut
              && ct->fP_hod_2ynhits > 0                                                 // quart-bar hit	
              && ct->fH_hod_goodstarttime == 1                                           // good hms start time
              && ct->fP_hod_goodstarttime == 1                                           // good shms stat time
	      && ct->fH_gtr_dp > -10 && ct->fH_gtr_dp < 10                               // e Delta cut
	      && ct->fP_gtr_dp > -15 && ct->fP_gtr_dp < 15                              // p Delta cut	     	
              && ct->fH_gtr_y > -15 && ct->fH_gtr_y < 15
              && ct->fP_gtr_y > -15 && ct->fP_gtr_y < 15                              // y-tar cut
	      && fabs(ct->fP_kin_secondary_pmiss)<0.4))
	    continue;

	//--- reconstructed quantities ---
        Emhdelta->Fill(ct->fH_gtr_dp,ct->fP_kin_secondary_emiss_nuc);
        Empdelta->Fill(ct->fP_gtr_dp,ct->fP_kin_secondary_emiss_nuc);
        Emhtheta->Fill(ct->fH_gtr_ph,ct->fP_kin_secondary_emiss_nuc);
        Emptheta->Fill(ct->fP_gtr_ph,ct->fP_kin_secondary_emiss_nuc);
        Emhxptar->Fill(ct->fH_gtr_th,ct->fP_kin_secondary_emiss_nuc);
        Empxptar->Fill(ct->fP_gtr_th,ct->fP_kin_secondary_emiss_nuc);
        Empxfp->Fill(ct->fP_dc_x_fp,ct->fP_kin_secondary_emiss_nuc);
        Empxpfp->Fill(ct->fP_dc_xp_fp,ct->fP_kin_secondary_emiss_nuc);
        Empyfp->Fill(ct->fP_dc_y_fp,ct->fP_kin_secondary_emiss_nuc);
        Empypfp->Fill(ct->fP_dc_yp_fp,ct->fP_kin_secondary_emiss_nuc);
        Emhxfp->Fill(ct->fH_dc_x_fp,ct->fP_kin_secondary_emiss_nuc);
        Emhxpfp->Fill(ct->fH_dc_xp_fp,ct->fP_kin_secondary_emiss_nuc);
        Emhyfp->Fill(ct->fH_dc_y_fp,ct->fP_kin_secondary_emiss_nuc);
        Emhypfp->Fill(ct->fH_dc_yp_fp,ct->fP_kin_secondary_emiss_nuc);


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
	      && ct->fPSdelta > -15 && ct->fPSdelta < 15
              && fabs(ct->fPm)<0.23))
	    continue;	

	//------- Reconstructed quantities ------
        sEmhdelta->Fill(ct->fHSdelta,ct->fEm,ct->fWeight);
        sEmpdelta->Fill(ct->fPSdelta,ct->fEm);
        sEmhtheta->Fill(ct->fHSyptar,ct->fEm);
        sEmptheta->Fill(ct->fPSyptar,ct->fEm);
        sEmhxptar->Fill(ct->fHSxptar,ct->fEm);
        sEmpxptar->Fill(ct->fPSxptar,ct->fEm);
        sEmpxfp->Fill(ct->fPSxfp,ct->fEm);
        sEmpxpfp->Fill(ct->fPSxpfp,ct->fEm);
        sEmpyfp->Fill(ct->fPSyfp,ct->fEm);
        sEmpypfp->Fill(ct->fPSypfp,ct->fEm);
        sEmhxfp->Fill(ct->fHSxfp,ct->fEm);
        sEmhxpfp->Fill(ct->fHSxpfp,ct->fEm);
        sEmhyfp->Fill(ct->fHSyfp,ct->fEm);
        sEmhypfp->Fill(ct->fHSypfp,ct->fEm);

    }

    TText *textSimc = new TText();
    textSimc->SetTextColor(kRed);
    TText *textData = new TText();
    textData->SetTextColor(kBlue);
    
    TCanvas *c1 = new TCanvas("Em test","EM test");
    c1->Divide(3,2);

    c1->cd(1);
    sEmhdelta->Draw();
    Emhdelta->Draw("same");
    c1->cd(2);
    sEmhtheta->Draw();
    Emhtheta->Draw("same");
    c1->cd(3);
    sEmhxptar->Draw();
    Emhxptar->Draw("same");
    c1->cd(4);
    sEmpdelta->Draw();
    Empdelta->Draw("same");
    c1->cd(5);
    sEmptheta->Draw();
    Emptheta->Draw("same");
    c1->cd(6);
    sEmpxptar->Draw();
    Empxptar->Draw("same");

    c1->Print("Emreport.pdf","pdf");

    TCanvas *c2 = new TCanvas("Em test fp","EM test fp");
    c2->Divide(4,2);

    c2->cd(1);
    sEmhxfp->Draw();
    Emhxfp->Draw("same");
    c2->cd(2);
    sEmhxpfp->Draw();
    Emhxpfp->Draw("same");
    c2->cd(3);
    sEmhyfp->Draw();
    Emhyfp->Draw("same");
    c2->cd(4);
    sEmhypfp->Draw();
    Emhypfp->Draw("same");
    c2->cd(5);
    sEmpxfp->Draw();
    Empxfp->Draw("same");
    c2->cd(6);
    sEmpxpfp->Draw();
    Empxpfp->Draw("same");
    c2->cd(7);
    sEmpyfp->Draw();
    Empyfp->Draw("same");
    c2->cd(8);
    sEmpypfp->Draw();
    Empypfp->Draw("same");

    c2->Print("Emreport2.pdf","pdf");
        

}
