// Filename: CT2run.cc
// Description: Compare Data vs Simc. This script exclusively for CT expt and assumes SHMS as proton arm and HMS as electron arm
// Author: Latif Kabir < latif@jlab.org >
// Created: Thu Jan  4 x:03:26 2018 (-0500)
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

using namespace std;

void CT2run(Int_t firstRun, Int_t secondRun, TString target)
{

    gStyle->SetOptStat(1); 
   //-----These offsets (for data) to be removed once SHMS is betetr calibrated ----------
    Double_t OffsetEm = 0.0;
    Double_t OffsetPdelta = 0.0;
    Double_t OffsetPmz = 0.0;
    Double_t OffsetW = 0.0;
    int cnts = 0;
    int cnts2 = 0;
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
    TH1D *hDelta = new TH1D("hDelta","HMS DELTA (%); HMS dp (%); Normalized counts",100,-15,15);
    //Kinematic quantities
    TH1D *hW = new TH1D("hW","W (GeV); W(GeV); Normalized counts", 150, 0.0, 2.0);
    TH1D *hbeta = new TH1D("hbeta","HMS beta; beta; Normalized counts", 150, 0.7, 1.3);
    TH1D *hcaletot = new TH1D("hcaletot","HMS caletot; caletot; Normalized counts", 150, 0.5, 2.0);
    //SHMS
    //Reconstructed quantities
    TH1D *pDelta = new TH1D("pDelta","SHMS DELTA (%); SHMS dp (%); Normalized counts",100,-20,20);
    //Kinematic quantities
    TH1D *pbeta = new TH1D("pbeta","SHMS beta; beta; Normalized counts", 150, 0.6, 1.6);
    TH1D *pEm = new TH1D("pEm","Missing Energy (GeV); E_{m} (GeV); Normalized counts",200,-0.15,0.5);
    TH1D *pPm = new TH1D("pPm","Pm (GeV/c); |P_{m}|(GeV/c); Normalized counts", 100, 0.0, 0.6);

    //--------- Histograms from 2nd run-----------
    //HMS
    //Reconstructed quantities
    TH1D *hSimcDelta = new TH1D("hSimcDelta","HMS DELTA (SIMC) (%)",100,-15,15);
    //Kinematic quantities
    TH1D *hSimcW = new TH1D("hSimcW","W (GeV)", 150, 0.0, 2.0);
    TH1D *hsbeta = new TH1D("hsbeta","HMS beta2; beta2; Normalized counts", 150, 0.7, 1.3);
    TH1D *hscaletot = new TH1D("hscaletot","HMS caletots; caletots; Normalized counts", 150, 0.5, 2.0);
   
    hSimcDelta->SetMarkerColor(kRed);  
    hSimcW->SetMarkerColor(kRed);  
    hsbeta->SetMarkerColor(kRed);  
    hscaletot->SetMarkerColor(kRed);  
    hSimcDelta->SetLineColor(kRed);  
    hSimcW->SetLineColor(kRed);  
    hsbeta->SetLineColor(kRed);  
    hscaletot->SetLineColor(kRed);  

    //SHMS
    //Reconstructed quantities
    TH1D *pSimcDelta = new TH1D("pSimcDelta","SHMS DELTA (SIMC) (%)",100,-20,20);
    //Kinematic quantities
    TH1D *psbeta = new TH1D("psbeta","SHMS beta2; beta2; Normalized counts", 150, 0.6, 1.6);

    TH1D *pSimcW = new TH1D("pSimcW","W (GeV)", 150, 0.0, 2.0);
    TH1D *pSimcEm = new TH1D("pSimcEm","Missing Energy (GeV)",200,-0.15,0.5);
    TH1D *pSimcPm = new TH1D("pSimcPm","|P_{m}| (GeV/c)", 100, 0.0, 0.6);



    pSimcDelta->SetMarkerColor(kRed);  
    psbeta->SetMarkerColor(kRed);  
    pSimcEm->SetMarkerColor(kRed);  
    pSimcPm->SetMarkerColor(kRed);  
    pSimcDelta->SetLineColor(kRed);  
    psbeta->SetLineColor(kRed);  
    pSimcEm->SetLineColor(kRed);  
    pSimcPm->SetLineColor(kRed);  

    // Momentum and beta

    Double_t PgtrdP;
    Double_t HgtrdP;
    Double_t PgtrBetaMes;    
    Double_t HgtrBetaMes;        

    Double_t H1W;
    Double_t P1Em;    
    Double_t P1Pm;        
 
    
    //Detector Response
    Double_t PhgcerNpeSum;

    Double_t HcalEtotTrackNorm;
    Double_t HcerNpeSum;


    TString fileName1 = ROOT_FILE_PATH;
    fileName1 += "coin_replay_production_";
    fileName1 += firstRun;
    fileName1 += "_-1";
    fileName1 += ".root";
    TFile *file = new TFile(fileName1);
    TTree *tree = (TTree*)file->Get("T");    

    tree->SetBranchAddress("P.gtr.dp", &PgtrdP);            
    tree->SetBranchAddress("H.gtr.dp", &HgtrdP);            
    tree->SetBranchAddress("P.gtr.beta", &PgtrBetaMes);        
    tree->SetBranchAddress("H.gtr.beta", &HgtrBetaMes);

    tree->SetBranchAddress("P.hgcer.npeSum", &PhgcerNpeSum);
    
    tree->SetBranchAddress("H.cal.etottracknorm", &HcalEtotTrackNorm);
    tree->SetBranchAddress("H.cer.npeSum", &HcerNpeSum);
    tree->SetBranchAddress("H.kin.primary.W", &H1W);
    tree->SetBranchAddress("P.kin.secondary.emiss_nuc", &P1Em);
    tree->SetBranchAddress("P.kin.secondary.pmiss", &P1Pm);

    



    Int_t nEvents = tree->GetEntries();

    for (Int_t event = 0; event < nEvents; ++ event)
    {
	if(event%50000 == 0)
	    cout << (Int_t)(100*event/nEvents) <<" % Done"<< endl;

	tree->GetEntry(event);
	if( !(
	      //PgtrBetaMes > 0.6 && PgtrBetaMes < 1.5                       // proton beta cut
	      //		  && HgtrBetaMes > 0.8 &&  HgtrBetaMes < 1.2                   // e beta cut
		  HcerNpeSum > 0.1 && PhgcerNpeSum < 0.1                    // Cerenkov counter cut for e- and proton
		  //	      && HcalEtotTrackNorm > 0.6 && HcalEtotTrackNorm < 2.0
                 )       
		)
		continue;
	
	hDelta->Fill(HgtrdP);
	pDelta->Fill(PgtrdP);
	hW->Fill(H1W);
	pPm->Fill(P1Pm);
	pEm->Fill(P1Em);
        hbeta->Fill(HgtrBetaMes);
        pbeta->Fill(PgtrBetaMes);
        hcaletot->Fill(HcalEtotTrackNorm);
        
        cnts++;
    }
    cout << cnts <<" events passed all cuts" << endl;


    Double_t P2gtrdP;
    Double_t H2gtrdP;
    Double_t P2gtrBetaMes;    
    Double_t H2gtrBetaMes;        

    Double_t H2W;
    Double_t P2Em;    
    Double_t P2Pm;        
 
    
    //Detector Response
    Double_t P2hgcerNpeSum;

    Double_t H2calEtotTrackNorm;
    Double_t H2cerNpeSum;

    //------------------ Plot second file --------------------------
    TString fileName2 = ROOT_FILE_PATH;
    fileName2 += "coin_replay_production_";
    fileName2 += secondRun;
    fileName2 += "_-1";
    fileName2 += ".root";
    TFile *file2 = new TFile(fileName2);
    TTree *simc = (TTree*)file2->Get("T");    
    
    simc->SetBranchAddress("P.gtr.dp", &P2gtrdP);            
    simc->SetBranchAddress("H.gtr.dp", &H2gtrdP);            
    simc->SetBranchAddress("P.gtr.beta", &P2gtrBetaMes);        
    simc->SetBranchAddress("H.gtr.beta", &H2gtrBetaMes);

    simc->SetBranchAddress("P.hgcer.npeSum", &P2hgcerNpeSum);
    
    simc->SetBranchAddress("H.cal.etottracknorm", &H2calEtotTrackNorm);
    simc->SetBranchAddress("H.cer.npeSum", &H2cerNpeSum);
    simc->SetBranchAddress("H.kin.primary.W", &H2W);
    simc->SetBranchAddress("P.kin.secondary.emiss_nuc", &P2Em);
    simc->SetBranchAddress("P.kin.secondary.pmiss", &P2Pm);



    Int_t nEventsSimc = simc->GetEntries();

    for (Int_t event = 0; event < nEventsSimc; ++event)
    {
	if(event%50000 == 0)
	    cout << (Int_t)(100*event/nEventsSimc) <<" % Done"<< endl;

	simc->GetEntry(event);
	if( !(
	      //P2gtrBetaMes > 0.6 && P2gtrBetaMes < 1.5                       // proton beta cut
	      //		  && H2gtrBetaMes > 0.8 &&  H2gtrBetaMes < 1.2                   // e beta cut
		  H2cerNpeSum > 0.1 && P2hgcerNpeSum < 0.1                    // Cerenkov counter cut for e- and proton
		  // && H2calEtotTrackNorm > 0.6 && H2calEtotTrackNorm < 2.0
                 )       
		)
		continue;
	
	hSimcDelta->Fill(H2gtrdP);
	pSimcDelta->Fill(P2gtrdP);
	hSimcW->Fill(H2W);
	pSimcPm->Fill(P2Pm);
	pSimcEm->Fill(P2Em);
        hsbeta->Fill(H2gtrBetaMes);
        psbeta->Fill(P2gtrBetaMes);
        hscaletot->Fill(H2calEtotTrackNorm);
        
        cnts2++;
    }

    cout << cnts2 <<" events passed all cuts for 2nd run" << endl;

    TText *textSimc = new TText();
    textSimc->SetTextColor(kRed);
    TText *textData = new TText();
    textData->SetTextColor(kBlue);
    
    TCanvas *c1 = new TCanvas("HMS","HMS");
    c1->Divide(3,2);

    c1->cd(1);
    hSimcDelta->Draw();
    hDelta->Draw("same");
    c1->cd(2);
    pSimcDelta->Draw();
    pDelta->Draw("same");
    c1->cd(3);
    hSimcW->Draw();
    hW->Draw("same");
    c1->cd(4);
    pSimcEm->Draw();
    pEm->Draw("same");
    c1->cd(5);
    pSimcPm->Draw();
    pPm->Draw("same");

    c1->Print("Report.pdf(","pdf");
    
    TCanvas *c2 = new TCanvas("SHMS","SHMS");
    c2->Divide(2,2);

    c2->cd(1);
    hscaletot->Draw();
    hcaletot->Draw("same");
    c2->cd(2);
     hsbeta->Draw();
     hbeta->Draw("same");
    c2->cd(3);
    psbeta->Draw();
    pbeta->Draw("same");
    c2->Print("Report.pdf)","pdf");

}
