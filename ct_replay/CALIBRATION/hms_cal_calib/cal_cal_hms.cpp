#include <TStyle.h>
#include <TCanvas.h>
#include <TString.h>
#include <TLeaf.h>
#include <TChain.h>
#include <TROOT.h>
#include <TFile.h>
#include <TTree.h>
#include <TBranch.h>
#include <TCut.h>
#include "TH1F.h"
#include "TH2F.h"
#include "TF1.h"
#include "TMath.h"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <time.h>

// A Root script for the HMS calorimeter quality plots

  void cal_cal_hms(int numruns=1, int RunNumber=1638, int nstop=9999999) {

  Double_t D_CALO_FP= 275. ;
  Double_t XMIN= -60.;
  Double_t XMAX=  60.;
  Double_t YMIN= -58.;
  Double_t YMAX=  58.;

  Double_t DELTA_MIN= -15;   //SHMS nominal acceptance
  Double_t DELTA_MAX=  15;

  Double_t BETA_MIN= 0.5;
  Double_t BETA_MAX= 1.5;

  Double_t HGCER_NPE_MIN= 3;
  Double_t NGCER_NPE_MIN= 1.; 

  UInt_t fNentries;
  UInt_t fNstart;
  UInt_t fNstop;
 
  fNstart = 0;
  fNstop = nstop;

  // Track parameters.

  Double_t        H_tr_x;   //X FP
  Double_t        H_tr_xp;
  Double_t        H_tr_y;   //Y FP
  Double_t        H_tr_yp;

  Double_t        H_hgcer_npe;
  Double_t        H_ngcer_npe;
  Double_t        H_tr_beta;

  Double_t        hdelta; 
  Double_t        hcaleprnorm;
  Double_t        hcaletotnorm;
  Double_t        hcalepr;
  Double_t        hcaletrack;

  TH1F* hEcal;
  TH1F* hEpr;
  TH2F* hDPvsEcal;
  TH2F* hESHvsEPR;

  // Initialize the analysis clock
  clock_t t = clock();
 
  cout << "Plot run " << RunNumber << endl;

  //Reset ROOT and connect tree file.

  gROOT->Reset();

   TChain *fTree = new TChain("T");
   for (int i=0;i<numruns;i++){
   TString fname = "/volatile/hallc/e1206107/ROOTfiles/detector_calibration/shms_replay_";
   fname += RunNumber;
   fname +="_";
   fname +=nstop;
   fname +=".root";
   fTree->Add(fname);
   RunNumber++;
   cout << "Root file name = " << fname << endl;
  }

  fNentries = fTree->GetEntries();
  cout << "THcPShowerCalib::Init: fNentries= " << fNentries << endl;

  fTree->SetBranchAddress("H.dc.x_fp", &H_tr_x);
  fTree->SetBranchAddress("H.dc.y_fp", &H_tr_y);
  fTree->SetBranchAddress("H.dc.xp_fp",&H_tr_xp);
  fTree->SetBranchAddress("H.dc.yp_fp",&H_tr_yp);
 
  fTree->SetBranchAddress("H.hgcer.npeSum", &H_hgcer_npe);
  fTree->SetBranchAddress("H.ngcer.npeSum", &H_ngcer_npe);

  fTree->SetBranchAddress("H.gtr.beta", &H_tr_beta);
  fTree->SetBranchAddress("H.gtr.dp", &hdelta); 
  fTree->SetBranchAddress("H.cal.eprtracknorm", &hcaleprnorm);
  fTree->SetBranchAddress("H.cal.etottracknorm", &hcaletotnorm);
  fTree->SetBranchAddress("H.cal.eprtrack", &hcalepr); 
  fTree->SetBranchAddress("H.cal.etot", &hcaletrack);    
     
  // Histogram declarations
  gStyle->SetOptFit(2);

    Int_t cnt=0;
    hEcal = new TH1F("hEcal", "Edep/P calibrated", 200, 0., 2.);
    hEpr = new TH1F("hEpr", "Epr calibrated", 200, 0., 2.);
    hDPvsEcal = new TH2F("hDPvsEcal", "#DeltaP versus Edep/P ",
  		       400,0.,2., 440,DELTA_MIN-1.,DELTA_MAX+1.);
    hESHvsEPR = new TH2F("hESHvsEPR", "E_{SH} versus E_{PR}",
		       300,0.0,1.4, 300,0.,1.4);

    for (UInt_t ientry=TMath::Max(UInt_t(0),fNstart);
       ientry<TMath::Min(fNstop,fNentries); ientry++) {

     fTree->GetEntry(ientry);


     if (ientry%100000 == 0) cout << "   ReadTree: " << ientry << endl;

  // Request single electron track in calorimeter's fid. volume

   bool good_trk =  hdelta > DELTA_MIN &&
		    hdelta < DELTA_MAX &&
		    H_tr_x + H_tr_xp*D_CALO_FP > XMIN &&
		    H_tr_x + H_tr_xp*D_CALO_FP < XMAX &&
                    H_tr_y + H_tr_yp*D_CALO_FP > YMIN &&
		    H_tr_y + H_tr_yp*D_CALO_FP < YMAX ;
   if (!good_trk) continue;

   bool good_ngcer = H_ngcer_npe > NGCER_NPE_MIN ;
   if(!good_ngcer) continue;

   bool good_hgcer = H_hgcer_npe > HGCER_NPE_MIN ;
   if(!good_hgcer) continue;

   bool good_beta = H_tr_beta > BETA_MIN &&
                   H_tr_beta < BETA_MAX ;
   if(!good_beta) continue;
   hEcal->Fill(hcaletotnorm);   
   hEpr->Fill(hcaleprnorm);
   hDPvsEcal->Fill(hcaletotnorm,hdelta);
   hESHvsEPR->Fill(hcaleprnorm,(hcaletotnorm-hcaleprnorm));
   cnt++;
   }
     cout << cnt << " passed all cuts  " << endl;


  // Plot histograms

  TCanvas* Canvas = new TCanvas("Canvas", "HMS Shower Counter performance", 1000, 667);
  Canvas->Divide(2,2);

  //Esh (=E_total - Epr) vs Epsh
  Canvas->cd(1);
  hESHvsEPR->Draw("colz");
 
 // Normalized energy deposition after calibration.
  gStyle->SetOptStat(1);
  gStyle->SetOptFit(2);
  Canvas->cd(2);
  hEcal->Fit("gaus","","",0.9,1.1);
  hEcal->GetFunction("gaus")->SetLineColor(2);
  hEcal->GetFunction("gaus")->SetLineWidth(1);
  hEcal->GetFunction("gaus")->SetLineStyle(1);

  // HMS delta(P) versus the calibrated energy deposition.
  Canvas->cd(3);
  hDPvsEcal->Draw("colz");

  // Save canvas in a pdf format
  Canvas->Print(Form("%d_%d.pdf",RunNumber,nstop));

  // Calculate the analysis rate
  t = clock() - t;
  printf ("The analysis took %.1f seconds \n", ((float) t) / CLOCKS_PER_SEC);
}
