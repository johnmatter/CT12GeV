// Filename: Plot_kine_compare.cc
// Description: W, Em, Pm and Cointime distributions for SHMS and HMS to compare between data and SIMC (except for Pm_per).
// Needs runnumber, 
// Also needs normfac from SIMC and the simc root file name

#include <iostream>
#include <TFile.h>
#include <TTree.h>
#include <TLeaf.h>
#include <TString.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TCanvas.h>
#include <math.h>
#include <cmath>
#include <TStyle.h>
#include <TROOT.h>
#include <TCut.h>
#include <TSystem.h>
#include "Constants.h"

using namespace std;


void Plot_kine_compare(Int_t runNumber, TString shms_part, TString simfile, Double_t normfac){

 //read the input file from data
  TString fileNameD = ROOT_FILE_PATH;
  fileNameD += "ep_"; //read the root file from data
  fileNameD += runNumber; //read the root file from data
  fileNameD += ".root"; //read the root file from data
  TFile *f1 = new TFile(fileNameD);
  TTree *tt = (TTree*)f1->Get("T");
  //get the relevant branch
  int nentriesD = tt->GetEntries();
  cout<<"Entries:\t"<<nentriesD<<endl;

  TString fileNameM = ROOT_FILE_PATH;
  fileNameM += simfile; //read the root file from SIMC
  TFile *f2 = new TFile(fileNameM);
  TTree *ts = (TTree*)f2->Get("h666");
  //get the relevant branch
  int nentriesM = ts->GetEntries();
  cout<<"Simc Entries:\t"<<nentriesM<<endl;

  TFile *fout = new TFile("run_kine_hists.root","RECREATE");

  TString filecO;
   filecO += "run_"; //read the root file from data
   filecO += runNumber; //read the root file from data
   filecO += "_kine.pdf"; //read the root file from data
  TString filecO1;
   filecO1 += "run_"; //read the root file from data
   filecO1 += runNumber; //read the root file from data
   filecO1 += "_kine.pdf["; //read the root file from data
  TString filecO2;
   filecO2 += "run_"; //read the root file from data
   filecO2 += runNumber; //read the root file from data
   filecO2 += "_kine.pdf]"; //read the root file from data

   //  gROOT->SetBatch(kTRUE);
  gROOT->ForceStyle(); 
  gStyle->SetPalette(kOcean);
  TCanvas *cc = new TCanvas ("cc","cc",800,600);
  cc->SetGrid();
  cc->SetLeftMargin(0.2);
  cc->SetRightMargin(0.2);
  gStyle->SetOptStat(0);

  
  //make histograms:
  TH1F *h_tmpEmd = new TH1F("h_Emd","Missing Energy (GeV)",200,-0.025,0.025);
  TH1F *h_Emd = new TH1F("h_Emd","Missing Energy (GeV)",200,-0.025,0.025);
  TH1F *h_Ems = new TH1F("h_Ems","Missing Energy (GeV)",200,-0.025,0.025);
  TH1F *h_Wd = new TH1F("h_Wd","W (GeV)", 150, 0.0, 2.0);
  TH1F *h_Ws = new TH1F("h_Ws","W (GeV)", 150, 0.0, 2.0);
  TH1F *h_tmppmd = new TH1F("h_pmd","Pm (GeV/c)", 100, -0.15, 0.15);
  TH1F *h_pmd = new TH1F("h_pmd","Pm (GeV/c)", 100, -0.15, 0.15);
  TH1F *h_pms = new TH1F("h_pms","Pm (GeV/c)", 100, -0.15, 0.15);
  TH1F *h_coint = new TH1F("h_coint","Coincidence Time (ns)",100, -50, 50);  
  TH1D *h1PcointimeROC1 = new TH1D("SHMS ROC1 Corrected Coin Time","SHMS ROC1 Corrected Coin Time; cointime [ns]", 200, -20, 20);    
  TH1D *h1PcointimeROC2 = new TH1D("SHMS ROC2 Corrected Coin Time","SHMS ROC2 Corrected Coin Time; cointime [ns]", 300, -15, 15); 
 
  TH1D *s_tmp = new TH1D("s_delta","stmp",100,-12,12);  //tmp hist to get integral    

  Double_t HgtrX, HgtrTh, HgtrY, HgtrPh;
  Double_t HgtrBetaCalc, PgtrBetaCalc, evtType, PgtrP, HgtrP, PhodStatus, PhodStartTime, PhodfpHitsTime;
  Double_t cointime, HhodStatus, HhodStartTime, HhodfpHitsTime, SHMSpartMass, HMSpartMass;
  Double_t pkinW, pEm, pPm, pbeta, hbeta, hcalepr, hcaletot, hcernpe, pcaletot, pcalepr, pcernpe;
  Double_t TcoinpTRIG1_ROC1_tdcTimeRaw, TcoinpTRIG4_ROC1_tdcTimeRaw, TcoinpTRIG1_ROC2_tdcTimeRaw;
  Double_t TcoinhTRIG1_ROC1_tdcTimeRaw, TcoinhTRIG1_ROC2_tdcTimeRaw, TcoinhTRIG4_ROC1_tdcTimeRaw;
  Double_t TcoinhTRIG4_ROC2_tdcTimeRaw, TcoinpTRIG4_ROC2_tdcTimeRaw;

  int cnts=0;
  TCut evtCut;                                                                   
  TCut eBetaCut;                                                                 
  TCut pBetaCut;                                                                 
  TCut cerCut;                                                                   
  TCut calCut;                                                                   
  TCut hodoTimeCut;                                                              

  tt->SetBranchAddress("P.gtr.p", &PgtrP);                                                                                          
  tt->SetBranchAddress("H.gtr.p", &HgtrP);                                                                                          
  tt->SetBranchAddress("P.gtr.beta", &pbeta);                                                                                 
  tt->SetBranchAddress("H.gtr.beta", &hbeta);                                                                                 
                                                                                                                                        
  tt->SetBranchAddress("P.cal.eprtracknorm", &pcalepr);                                                                    
  tt->SetBranchAddress("P.cal.etottracknorm", &pcaletot);                                                                  
  tt->SetBranchAddress("P.hgcer.npeSum", &pcernpe);                                                                            
                                                                                                                                        
  tt->SetBranchAddress("H.cal.eprtracknorm", &hcalepr);                                                                    
  tt->SetBranchAddress("H.cal.etottracknorm", &hcaletot);                                                                  
  tt->SetBranchAddress("H.cer.npeSum", &hcernpe); 
  tt->SetBranchAddress("P.kin.primary.W", &pkinW);                                                                               
  tt->SetBranchAddress("H.kin.secondary.emiss", &pEm);                                                                               
  tt->SetBranchAddress("H.kin.secondary.pmiss", &pPm);                                                                               
                                                                                                                                         
  tt->SetBranchAddress("P.hod.goodstarttime", &PhodStatus);                                                                         
  tt->SetBranchAddress("P.hod.starttime", &PhodStartTime);                                                                          
  tt->SetBranchAddress("P.hod.fpHitsTime", &PhodfpHitsTime);                                                                        
                                                                                                                                        
  tt->SetBranchAddress("H.hod.goodstarttime", &HhodStatus);                                                                         
  tt->SetBranchAddress("H.hod.starttime", &HhodStartTime);                                                                          
  tt->SetBranchAddress("H.hod.fpHitsTime", &HhodfpHitsTime);                                                                        
                                                                                                                                        
  tt->SetBranchAddress("H.gtr.x", &HgtrX);                                                                                          
  tt->SetBranchAddress("H.gtr.th", &HgtrTh);                                                                                        
  tt->SetBranchAddress("H.gtr.y", &HgtrY);                                                                                          
  tt->SetBranchAddress("H.gtr.ph", &HgtrPh);                                                                                        
                                                                                                                                        
  tt->SetBranchAddress("T.coin.pTRIG1_ROC1_tdcTimeRaw", &TcoinpTRIG1_ROC1_tdcTimeRaw);                                              
  tt->SetBranchAddress("T.coin.pTRIG4_ROC1_tdcTimeRaw", &TcoinpTRIG4_ROC1_tdcTimeRaw);
  tt->SetBranchAddress("T.coin.pTRIG1_ROC2_tdcTimeRaw", &TcoinpTRIG1_ROC2_tdcTimeRaw);                                              
  tt->SetBranchAddress("T.coin.pTRIG4_ROC2_tdcTimeRaw", &TcoinpTRIG4_ROC2_tdcTimeRaw);                                              
                                                                                                                                        
  tt->SetBranchAddress("T.coin.hTRIG1_ROC1_tdcTimeRaw", &TcoinhTRIG1_ROC1_tdcTimeRaw);                                              
  tt->SetBranchAddress("T.coin.hTRIG4_ROC1_tdcTimeRaw", &TcoinhTRIG4_ROC1_tdcTimeRaw);                                              
  tt->SetBranchAddress("T.coin.hTRIG1_ROC2_tdcTimeRaw", &TcoinhTRIG1_ROC2_tdcTimeRaw);                                              
  tt->SetBranchAddress("T.coin.hTRIG4_ROC2_tdcTimeRaw", &TcoinhTRIG4_ROC2_tdcTimeRaw);                                              
  

  if(shms_part == "e")                                                      
    {                                                                              
      SHMSpartMass = 0.000510998; // electron mass in GeV/c^2                
      HMSpartMass = 0.93827208; // proton mass in GeV/c^2                  
      eBetaCut = "P.gtr.beta > 0.5 && P.gtr.beta < 1.5";                         
      pBetaCut = "H.gtr.beta > 0.5 &&  H.gtr.beta < 1.5";                        
      cerCut= "P.hgcer.npeSum > 0.1 && H.cer.npeSum < 0.1";                      
      calCut = "P.cal.etottracknorm > 0.8 && P.cal.etottracknorm < 1.5 && P.cal.eprtracknorm  > 0.2";        
      hodoTimeCut ="P.hod.goodstarttime == 1 && H.hod.goodstarttime == 1";       
    }                                                                              
  else                                                                          
    {                                                                             
      SHMSpartMass = 0.93827208; // proton mass in GeV/c^2                 
      HMSpartMass = 0.000510998; // electron mass in GeV/c^2                 
      eBetaCut = "P.gtr.beta > 0.5 && P.gtr.beta < 1.5";                        
      pBetaCut = "H.gtr.beta > 0.5 &&  H.gtr.beta < 1.5";                       
      cerCut = "P.hgcer.npeSum < 0.1 && H.cer.npeSum > 0.1";                    
      calCut = "H.cal.etottracknorm > 0.8 && H.cal.etottracknorm < 1.5 && H.cal.eprtracknorm  > 0.2";         
      hodoTimeCut ="P.hod.goodstarttime == 1 && H.hod.goodstarttime == 1";      
    }                                                                             
  
  TCanvas *canvas1 = new TCanvas("canvas1","canvas1");                           
  tt->Draw("P.hod.starttime", eBetaCut && pBetaCut && cerCut && calCut && hodoTimeCut);  
  //TH1D *h1PhodoStartTime = (TH1D*)gDirectory->Get("SHMShodoStartTime");TCanvas *canvas2 = new TCanvas("canvas2","canvas2");                           
  tt->Draw("H.hod.starttime", eBetaCut && pBetaCut && cerCut && calCut && hodoTimeCut);  
  //TH1D *h1HhodoStartTime = (TH1D*)gDirectory->Get("HMShodoStartTime");           
  
  TH1D *h1PhodoStartTime = new TH1D("SHMS Start Time","SHMS Start Time; cointime [ns]", 100, 0, 200);    
  TH1D *h1HhodoStartTime = new TH1D("HMS Start Time","HMS Start Time; cointime [ns]", 100, 0, 200); 
  
  h1PhodoStartTime->GetXaxis()->SetTitle("SHMS hodo start time [ns]");           
  h1HhodoStartTime->GetXaxis()->SetTitle("HMS hodo start time [ns]");            
  Double_t pOffset = 3.0; //9.5 + 10;  // in ns                                  
  Double_t hOffset = 335;                                                        
  Double_t speedOfLight = 29.9792; // in cm/ns                                   
  Double_t SHMScentralPathLen = 18.1*100;  // SHMS Target to focal plane path length converted to cm  
  Double_t SHMSpathLength = 18.1*100;      // For now assume that it's same as SHMScentralPathLen  
  Double_t HMScentralPathLen = 22*100;     // HMS Target to focal plane path length converted to cm
  Double_t DeltaHMSpathLength;             // For now assume that it's same as HMScentralPathLen 
  Double_t SHMScoinCorr = 0.0;                                                   
  Double_t HMScoinCorr = 0.0;                                                    
  Double_t PhodoStartTimeMean = h1PhodoStartTime->GetMean();                     
  Double_t HhodoStartTimeMean = h1HhodoStartTime->GetMean();                     
  cout << PhodoStartTimeMean <<"mean"<< endl;
  Double_t SHMSrawCoinTimeROC1;                                                  
  Double_t SHMSrawCoinTimeROC2;                                                  
  Double_t HMSrawCoinTimeROC1;                                                   
  Double_t HMSrawCoinTimeROC2;                                                   
                                                                                   
  Double_t SHMScorrCoinTimeROC1;                                                 
  Double_t SHMScorrCoinTimeROC2;                                                 
  Double_t HMScorrCoinTimeROC1;                                                  
  Double_t HMScorrCoinTimeROC2;                                                  
  Double_t Em_mean;
  Double_t Pm_mean;
 
  for (int kk=0; kk<nentriesD; kk++){
    tt->GetEntry(kk);
    if (kk % 50000 == 0) cout << kk*100/nentriesD << "   % of data done" << endl;
      evtType = tt->GetLeaf("fEvtHdr.fEvtType")->GetValue(); 
    if (shms_part == "e") 
    {
     if (pbeta>0.6 && pbeta<1.4 && hbeta>0.8 && hbeta<1.3 && pcernpe>0.1 && hcernpe<0.1 && pcaletot >0.8 && pcaletot<1.5 && pcalepr>0.1 && PhodStatus == 1 && HhodStatus ==1) 
      { //cuts to select the electrons and protons
       DeltaHMSpathLength = 12.462*HgtrTh + 0.1138*HgtrTh*HgtrX - 0.0154*HgtrX - 72.292*HgtrTh*HgtrTh - 0.0000544*HgtrX*HgtrX - 116.52*HgtrPh*HgtrPh;               
      PgtrBetaCalc = PgtrP/sqrt(PgtrP*PgtrP + SHMSpartMass*SHMSpartMass);        
      HgtrBetaCalc = HgtrP/sqrt(HgtrP*HgtrP + HMSpartMass*HMSpartMass);          
      SHMScoinCorr = SHMScentralPathLen / (speedOfLight*PgtrBetaCalc) + (SHMSpathLength - SHMScentralPathLen) / speedOfLight*PgtrBetaCalc + (PhodoStartTimeMean - PhodfpHitsTime);                                                                   
      HMScoinCorr = HMScentralPathLen / (speedOfLight*HgtrBetaCalc) + DeltaHMSpathLength / speedOfLight*HgtrBetaCalc + (HhodoStartTimeMean - HhodfpHitsTime);      
      SHMScorrCoinTimeROC1 = (TcoinpTRIG1_ROC1_tdcTimeRaw*0.1 - SHMScoinCorr) - (TcoinpTRIG4_ROC1_tdcTimeRaw*0.1 - HMScoinCorr) - pOffset; // 0.1 to convert to ns 
      SHMScorrCoinTimeROC2 = (TcoinpTRIG1_ROC2_tdcTimeRaw*0.1 - SHMScoinCorr) - (TcoinpTRIG4_ROC2_tdcTimeRaw*0.1 - HMScoinCorr) - pOffset;                         
      h1PcointimeROC1->Fill(SHMScorrCoinTimeROC1);                               
      h1PcointimeROC2->Fill(SHMScorrCoinTimeROC2);                               
      //if (SHMScorrCoinTimeROC1 > -12.0 && SHMScorrCoinTimeROC1 < 12.0){
      
         h_tmpEmd->Fill(pEm);         
         h_Wd->Fill(pkinW);
         h_tmppmd->Fill(pPm);
	 // }
      }
    } 

    else if (shms_part == "p") 
    {

      if (pbeta>0.6 && pbeta<1.4 && hbeta>0.8 && hbeta<1.2 && pcernpe<0.1 && hcernpe>0.1 && hcaletot >0.8 && hcaletot<1.2 && PhodStatus == 1 && HhodStatus ==1) { //cuts to select the electrons and protons


	DeltaHMSpathLength = 12.462*HgtrTh + 0.1138*HgtrTh*HgtrX - 0.0154*HgtrX - 72.292*HgtrTh*HgtrTh - 0.0000544*HgtrX*HgtrX - 116.52*HgtrPh*HgtrPh;               
      PgtrBetaCalc = PgtrP/sqrt(PgtrP*PgtrP + SHMSpartMass*SHMSpartMass);        
      HgtrBetaCalc = HgtrP/sqrt(HgtrP*HgtrP + HMSpartMass*HMSpartMass);          
      SHMScoinCorr = SHMScentralPathLen / (speedOfLight*PgtrBetaCalc) + (SHMSpathLength - SHMScentralPathLen) / speedOfLight*PgtrBetaCalc + (PhodoStartTimeMean - PhodfpHitsTime);                                                                   
      HMScoinCorr = HMScentralPathLen / (speedOfLight*HgtrBetaCalc) + DeltaHMSpathLength / speedOfLight*HgtrBetaCalc + (HhodoStartTimeMean - HhodfpHitsTime);      
      SHMScorrCoinTimeROC1 = (TcoinpTRIG1_ROC1_tdcTimeRaw*0.1 - SHMScoinCorr) - (TcoinpTRIG4_ROC1_tdcTimeRaw*0.1 - HMScoinCorr) - pOffset; // 0.1 to convert to ns 
      SHMScorrCoinTimeROC2 = (TcoinpTRIG1_ROC2_tdcTimeRaw*0.1 - SHMScoinCorr) - (TcoinpTRIG4_ROC2_tdcTimeRaw*0.1 - HMScoinCorr) - pOffset;                         
      h1PcointimeROC1->Fill(SHMScorrCoinTimeROC1);                               
      h1PcointimeROC2->Fill(SHMScorrCoinTimeROC2);
 
      //      if (SHMScorrCoinTimeROC1 > -12.0 && SHMScorrCoinTimeROC1 < 12.0){
      
        
        h_tmpEmd->Fill(pEm);
        h_Wd->Fill(pkinW);
        h_tmppmd->Fill(pPm);
	//}                               
     }
    }  
  }
  Em_mean = h_tmpEmd->GetMean();
  Pm_mean = h_tmppmd->GetMean();
  cout << Em_mean << "  Em_mean " << endl;
  for (int kk=0; kk<nentriesD; kk++){
    tt->GetEntry(kk);
    if (kk % 50000 == 0) cout << kk*100/nentriesD << "   % of data done" << endl;
      evtType = tt->GetLeaf("fEvtHdr.fEvtType")->GetValue(); 
    if (shms_part == "e") 
    {
     if (pbeta>0.6 && pbeta<1.4 && hbeta>0.8 && hbeta<1.3 && pcernpe>0.1 && hcernpe<0.1 && pcaletot >0.8 && pcaletot<1.5 && pcalepr>0.1 && PhodStatus == 1 && HhodStatus ==1 && (pEm-Em_mean) > -0.25 && (pEm-Em_mean) < 0.25) 
      {
         cnts++;
         h_Emd->Fill(pEm-Em_mean);         
         h_Wd->Fill(pkinW);
         h_pmd->Fill(pPm-Pm_mean);
      }
    }
    else if (shms_part == "p") 
    {
      if (pbeta>0.6 && pbeta<1.4 && hbeta>0.8 && hbeta<1.2 && pcernpe<0.1 && hcernpe>0.1 && hcaletot >0.8 && hcaletot<1.2 && PhodStatus == 1 && HhodStatus ==1 && (pEm-Em_mean) > -0.25 && (pEm-Em_mean) < 0.25) { //cuts to select the electrons and protons

        cnts++;
        h_Emd->Fill(pEm-Em_mean);
        h_Wd->Fill(pkinW);
        h_pmd->Fill(pPm-Pm_mean);
     }
    }
  }
  cout << cnts <<" events passed all cuts" << endl;
  double sEm, sPm, sW, wt, sdelta, sfac, normwt;

  for (int kk=0; kk<nentriesM; kk++){
    ts->GetEntry(kk);
    if (kk % 50000 == 0) cout << kk*100/nentriesM << "   % of simc done" << endl;

    sdelta = ts->GetLeaf("hsdelta")->GetValue();
    wt = ts->GetLeaf("Weight")->GetValue();
    normwt = wt*normfac/nentriesM;
    s_tmp->Fill(sdelta,normwt);
  }
  double intgrl = s_tmp->Integral(1,100); // integral of the SIMC histogram

  sfac = cnts/intgrl;  // scale factor to make the intregral of both Data and SIMC histrograms are equal
  cout << "scalefac =" << sfac << endl;


  for (int kk=0; kk<nentriesM; kk++){
    ts->GetEntry(kk);
    if (kk % 50000 == 0) cout << kk*100/nentriesM << "   % of simc done" << endl;


    sEm = ts->GetLeaf("Em")->GetValue();
    sPm = ts->GetLeaf("Pm")->GetValue();
    sW = ts->GetLeaf("W")->GetValue();
    wt = ts->GetLeaf("Weight")->GetValue();
    normwt = wt*normfac*sfac/nentriesM;
    h_Ems->Fill(sEm,normwt);
    h_pms->Fill(sPm,normwt);
    h_Ws->Fill(sW,normwt);
  }

  cc->Print(filecO1);
  cc->cd(1);
  h_Ws->Draw();
  h_Ws->SetLineColor(kRed);
  h_Wd->Draw("SAME");
  h_Wd->SetLineColor(kBlue);
  cc->Print(filecO);
  cc->cd(1);
  h_Ems->Draw();
  h_Ems->SetLineColor(kRed);
  h_Emd->Draw("same");
  h_Emd->SetLineColor(kBlue);
  cc->Print(filecO);
 
  cc->cd(1);
  h_pms->Draw();
  h_pms->SetLineColor(kRed);
  h_pmd->Draw("same");
  h_pmd->SetLineColor(kBlue);
  cc->Print(filecO);
  
  cc->cd(1);
  h1PcointimeROC1->Draw();
  h1PcointimeROC1->SetLineColor(kRed);
  h1PcointimeROC2->Draw("same");
  h1PcointimeROC1->SetLineColor(kBlue);
  cc->Print(filecO);

  cc->Print(filecO2);

  gROOT->SetBatch(kFALSE);
  TCanvas *c1 = new TCanvas ("cc1","cc1",800,600);
  c1->SetGrid();
  c1->SetLeftMargin(0.2);
  c1->SetRightMargin(0.2);
  c1->Divide(2,2);


  //  cc->Update();
  gStyle->SetPaperSize(20,20);
  c1->cd(1);
  h_Ws->Draw();
  h_Ws->SetLineColor(kRed);
  h_Wd->Draw("SAME");
  h_Wd->SetLineColor(kBlue);

  c1->cd(2);
  h_Ems->Draw();
  h_Ems->SetLineColor(kRed);
  h_Emd->Draw("same");
  h_Emd->SetLineColor(kBlue);

  c1->cd(3);
  h_pms->Draw();
  h_pms->SetLineColor(kRed);
  h_pmd->Draw("same");
  h_pmd->SetLineColor(kBlue);

  c1->cd(4);
  h1PcointimeROC1->Draw();
  h1PcointimeROC1->SetLineColor(kRed);
  h1PcointimeROC2->Draw("same");
  h1PcointimeROC1->SetLineColor(kBlue);
  
  c1->SaveAs("tmp1.pdf");


  
  fout->Write();
  fout->Close();

}
