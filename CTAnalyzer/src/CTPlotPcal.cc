#include <TStyle.h>
#include <TCanvas.h>
#include <TString.h>
#include <TLeaf.h>
#include <TChain.h>
#include <TROOT.h>
#include <TFile.h>
#include <TTree.h>
#include <TBranch.h>
#include "TH1F.h"
#include "TH2F.h"
#include "TF1.h"
#include "TMath.h"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <time.h>
#include "Constants.h"
#include "CTRun.h"
using namespace std;




//
// A steering Root script for SHMS calorimeter quality plots 
//

void CTPlotPcal(Int_t numruns, Int_t RunNumber, Int_t nstop) {


  Int_t firstRun = RunNumber;
  Int_t lastRun = RunNumber + numruns -1;
  Double_t D_CALO_FP= 275. ;  // Calorimeter fiducial cuts
  Double_t XMIN= -60.;
  Double_t XMAX=  60.;
  Double_t YMIN= -58.;
  Double_t YMAX=  58.;

  Double_t DELTA_MIN= -10;   //SHMS nominal delta acceptance
  Double_t DELTA_MAX=  22;

  Double_t BETA_MIN= 0.5;
  Double_t BETA_MAX= 1.5;

  Double_t HGCER_NPE_MIN= 3;   // Cerenkov for electrons
  Double_t NGCER_NPE_MIN= 1.;   //



  //  TTree* fTree;
  UInt_t fNentries;
  UInt_t fNstart;
  UInt_t fNstop;
 
  fNstart = 0;
  fNstop = nstop;
  // Declaration of leaves types

  // Track parameters.




  TH1F* hEcal;
  TH1F* hEpr;
  TH2F* hDPvsEcal;
  TH2F* hESHvsEPR;
  hEcal = new TH1F("hEcal", "Edep/P calibrated", 200, 0., 2.);
  hEpr = new TH1F("hEpr", "Epr calibrated", 200, 0., 2.);
  hDPvsEcal = new TH2F("hDPvsEcal", "#DeltaP versus Edep/P ",
		       400,0.,2., 440,DELTA_MIN-1.,DELTA_MAX+1.);
  hESHvsEPR = new TH2F("hESHvsEPR", "E_{SH} versus E_{PR}",
		       300,0.0,1.4, 300,0.0,1.4);

  // Initialize the analysis clock
  clock_t t = clock();
 
  cout << "Plot run " << RunNumber << endl;

//Whole calorimeter fid. limits



  //Reset ROOT and connect tree file.

  gROOT->Reset();
  gStyle->SetOptFit(2);

  Int_t cnt=0;


  CTRun *ct = new CTRun(firstRun,lastRun);
  if(!ct->fRunExist)
	return;
    //------------------------------------ Fill histograms from data ------- 
  TTree * tree = ct->GetTree();

  Int_t nEvents = tree->GetEntries();

  

  for (Int_t event = 0; event < nEvents; ++ event)
  {
	if(event%100000 == 0)
	    cout << (Int_t)(100*event/nEvents) <<" % Done"<< endl;

	tree->GetEntry(event);



   bool good_trk =  ct->fP_gtr_dp > DELTA_MIN &&
		    ct->fP_gtr_dp < DELTA_MAX &&
		    ct->fP_dc_x_fp + ct->fP_dc_xp_fp*D_CALO_FP > XMIN &&
                    ct->fP_dc_x_fp + ct->fP_dc_xp_fp*D_CALO_FP < XMAX &&
		    ct->fP_dc_y_fp + ct->fP_dc_yp_fp*D_CALO_FP > YMIN &&
                    ct->fP_dc_y_fp + ct->fP_dc_yp_fp*D_CALO_FP < YMAX ;
   if (!good_trk) continue;

   bool good_ngcer = ct->fP_ngcer_npeSum > NGCER_NPE_MIN ;
   if(!good_ngcer) continue;

   bool good_hgcer = ct->fP_hgcer_npeSum > HGCER_NPE_MIN ;
   if(!good_hgcer) continue;

   bool good_beta = ct->fP_gtr_beta > BETA_MIN &&
                    ct->fP_gtr_beta < BETA_MAX ;
   if(!good_beta) continue;
   hEcal->Fill(ct->fP_cal_etottracknorm);   
   hEpr->Fill(ct->fP_cal_eprtracknorm);
   hDPvsEcal->Fill(ct->fP_cal_etottracknorm,ct->fP_gtr_dp);
   hESHvsEPR->Fill(ct->fP_cal_eprtracknorm,(ct->fP_cal_etottracknorm - ct->fP_cal_eprtracknorm));
   cnt++;
  }
  cout << cnt << " passed all cuts  " << endl;


  // Plot histograms

  TCanvas* Canvas =
    new TCanvas("Canvas", "SHMS Shower Counter performance", 1000, 667);
  Canvas->Divide(2,2);


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

  // SHMS delta(P) versus the calibrated energy deposition.

  Canvas->cd(3);
  hDPvsEcal->Draw("colz");

  //  Canvas->cd(4);
  //hEpr->Draw();

  // Save canvas in a pdf format.
  Canvas->Print(Form("PLOTS/Pcal_plots_%d_%d.pdf",RunNumber,nstop));

  // Calculate the analysis rate
  t = clock() - t;
  printf ("The analysis took %.1f seconds \n", ((float) t) / CLOCKS_PER_SEC);
}
