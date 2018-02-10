// Filename: CTkinDistRun.cc
// Description:  Missing eng correlation
// Author: Latif Kabir < latif@jlab.org >
// Created: Thu Jan  4 17:03:26 2018 (-0500)
// URL: jlab.org/~latif

// Check 2047-2049

#include <iostream>
#include <TFile.h>
#include <TTree.h>
#include <TString.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TCanvas.h>
#include <TSystem.h>
#include "Constants.h"
#include "CTRun.h"
using namespace std;

void CTmissEngCorr(Int_t firstRun, Int_t lastRun)
{
    CTRun *ct;
    if(lastRun != -1)
      ct = new CTRun(firstRun, lastRun);
    else
      ct = new CTRun(firstRun);

    if(!ct->fRunExist)
	return;
    
    TH1D *h1PkinSecondaryEmiss = new TH1D("P.kin.secondary.emiss", "P.kin.secondary.emiss", 500, -1, 2);

    TH2D *h2PmissEngVsPgtrP = new TH2D("missEngVsPgtrP","missEngVsPgtrP;  P.gtr.p[GeV/c]; miss eng [GeV];", 100, 3.0, 8.0, 100,-1, 2);
    TH2D *h2PmissEngVsPgtrdP = new TH2D("missEngVsPgtrdP","missEngVsPgtrdP; P.gtr.dp[GeV/c]; miss eng [GeV]", 100, -10.0, 10.0, 100,-1, 2);
    TH2D *h2HmissEngVsPgtrP = new TH2D("missEngVsHgtrP","missEngVsHgtrP;  H.gtr.p[GeV/c]; miss eng [GeV];", 100, 1.8, 2.4, 100,-1, 2);
    TH2D *h2HmissEngVsPgtrdP = new TH2D("missEngVsHgtrdP","missEngVsHgtrdP; H.gtr.dp[GeV/c]; miss eng [GeV]", 100, -7.0, 7.0, 100,-1, 2);
    
    TTree * tree = ct->GetTree();

    //Optional:Disable unnecessary branches and enable required ones to speed up analysis for larger file
    //You do not need this if file is already skimmed while running replay
    tree->SetBranchStatus("*",0);
    tree->SetBranchStatus("P.kin.secondary.*",1);
    tree->SetBranchStatus("H.kin.primary.*",1);
    tree->SetBranchStatus("P.gtr.*",1);
    tree->SetBranchStatus("H.gtr.*",1);
    tree->SetBranchStatus("P.hgcer.*",1);
    tree->SetBranchStatus("H.cer.*",1);
    tree->SetBranchStatus("P.cal.*",1);
    tree->SetBranchStatus("H.cal.*",1);

    Int_t nEvents = tree->GetEntries();

    for (Int_t event = 0; event < nEvents; ++ event)
    {
	if(event%5000 == 0)
	    cout << (Int_t)(100*event/nEvents) <<" % Done"<< endl;

	tree->GetEntry(event);

	// PID Cut
	if( !(ct->fP_gtr_beta > 0.5 && ct->fP_gtr_beta < 1.5                             // proton beta cut
	      && ct->fH_gtr_beta > 0.5 &&  ct->fH_gtr_beta < 1.5                         // e beta cut
	      && ct->fH_cer_npeSum > 0.1 && ct->fP_hgcer_npeSum < 0.1                    // Cerenkov counter cut for e- and proton
	      && ct->fH_cal_etottracknorm > 0.8 && ct->fH_cal_etottracknorm < 1.5        // HMS Calorimeter cut 
	      && ct->fH_cal_eprtracknorm > 0.2) 	                                 // HMS calorimeter cut       
	)
	continue;
	
	h1PkinSecondaryEmiss->Fill(ct->fP_kin_secondary_emiss);
        h2PmissEngVsPgtrP->Fill(ct->fP_gtr_p, ct->fP_kin_secondary_emiss);
        h2PmissEngVsPgtrdP->Fill(ct->fP_gtr_dp, ct->fP_kin_secondary_emiss);
        h2HmissEngVsPgtrP->Fill(ct->fH_gtr_p, ct->fP_kin_secondary_emiss);
        h2HmissEngVsPgtrdP->Fill(ct->fH_gtr_dp, ct->fP_kin_secondary_emiss);
    }

    TCanvas *c1 = new TCanvas("c1","c1"); 
    h1PkinSecondaryEmiss->Draw();
    TCanvas *c2 = new TCanvas("c2","c2");
    c2->Divide(2,2); 
    c2->cd(1);
    h2PmissEngVsPgtrP->Draw("colz");    
    c2->cd(2);
    h2PmissEngVsPgtrdP->Draw("colz");    
    c2->cd(3);
    h2HmissEngVsPgtrP->Draw("colz");    
    c2->cd(4);
    h2HmissEngVsPgtrdP->Draw("colz");    
}
