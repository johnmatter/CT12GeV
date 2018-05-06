#include <iostream>
#include <TFile.h>
#include <TTree.h>
#include <TString.h>
#include <TH1D.h>
#include <TCanvas.h>
#include <TSystem.h>
#include "TH1F.h"
#include "TTreeReader.h"
#include "TTreeReaderValue.h"

using namespace std;

void hodo_cal_hms() {
    

   // Open the file containing the tree.
   TFile *myFile = TFile::Open("/volatile/hallc/comm2017/e1206107/ROOTfiles/detector_calibration/hms_replay_production_all_1268_-1.root");

    // Creating Canvas

   TCanvas *c1 = new TCanvas("c1","Calibrated HMS Hodoscope Plots",200,10,700,500);
   c1->Divide(2,2);
  
    TH1D *h1HgtrBeta = new TH1D("H.gtr.beta", "Beta; ", 200, 0, 1.4);
    TH1D *h1AdcTdcDiffTime = new TH1D("H.cer.goodAdcTdcDiffTime", "AdcTdcDiffTime", 200, -80, 20);
    TH1D *h1Delta = new TH1D("H.gtr.dp", "Delta", 200, -30, 40);
    TH2D *DeltavsBeta = new TH2D("H.gtr.dp:H.gtr.beta", "Delta vs Beta", 200, 0, 1.8, 200, -30, 40);
    
  

   // Create a TTreeReader for the tree
   TTreeReader myReader("T", myFile);

   // The tree branches
   TTreeReaderValue<Double_t> delta(myReader, "H.gtr.dp");
   TTreeReaderValue<Double_t> beta(myReader, "H.gtr.beta");
   TTreeReaderArray<Double_t> AdcTdcDiffTime(myReader, "H.cer.goodAdcTdcDiffTime");
   TTreeReaderValue<Double_t> npe(myReader, "H.cer.npeSum");
   TTreeReaderValue<Double_t> etnorm(myReader, "H.cal.etracknorm");
   
   // Loop over all entries of the TTree 

   while (myReader.Next()) {
      
     if (*beta>0.0&&*beta<1.4&&*npe>0.){
     h1HgtrBeta->Fill(*beta);
     }
   if (*delta>-30&&*delta<40){
     h1Delta->Fill(*delta);
     }
   if (AdcTdcDiffTime[0]>-80&&AdcTdcDiffTime[0]<20&&*etnorm>0.8){
   h1AdcTdcDiffTime->Fill(AdcTdcDiffTime[0]);
    }
   if (*beta>0.0&&*beta<1.8&&*delta>-30&&*delta<40&&*npe>2.&&*etnorm>0.8)
   DeltavsBeta->Fill(*beta, *delta);
     }

   //Draw in the canvas 

    c1->cd(1);
    h1HgtrBeta->Draw();
    h1HgtrBeta->Fit("gaus","","",0.9,1.1);
    c1->cd(2);
    gPad->SetLogy();
    h1AdcTdcDiffTime->Draw();
    c1->cd(3); 
    DeltavsBeta->Draw("colz");
   
    c1->SaveAs("Calibrated_HMS_Hodoscopes_Plots.pdf");
}
