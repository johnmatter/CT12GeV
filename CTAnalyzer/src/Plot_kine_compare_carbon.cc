// Filename: Plot_kine_compare_carbon.cc
// Description: W, Em, Pm and Cointime distributions for SHMS and HMS to compare between data and SIMC (except for Pm_per).
// Needs number of runs, particle in SHMS, target
// Also needs normfac from SIMC and the simc root file name

#include <iostream>
#include <TFile.h>
#include <TTree.h>
#include <TChain.h>
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
#include <THStack.h>
#include "Constants.h"

using namespace std;


void Plot_kine_compare_carbon(Int_t numruns, TString shms_part, TString simfile, Double_t normfac){


  //  const Int_t runnumber[numruns]={2187, 2188, 2189, 2190, 2191, 2192, 2193, 2194, 2195, 2196, 2197, 2198, 2199, 2200, 2201, 2202, 2203, 2204, 2205, 2206, 2207, 2208, 2209, 2211, 2212};
  //  const Int_t runnumber[numruns]={2284, 2285, 2286, 2290, 2291, 2292, 2293, 2294, 2295, 2296, 2297, 2298, 2299, 2300, 2301, 2303, 2304, 2305, 2306, 2308, 2309, 2310, 2311, 2312, 2313, 2314, 2315, 2316, 2317, 2318, 2319, 2320, 2321, 2322, 2324, 2325, 2343, 2344, 2345, 2346, 2347};
  const Int_t runnumber[numruns]={2441, 2442, 2443, 2444, 2445, 2446, 2447, 2448, 2449};

  TString fileNameM = ROOT_FILE_PATH;
  fileNameM += simfile; //read the root file from SIMC
  TFile *f2 = new TFile(fileNameM);
  TTree *ts = (TTree*)f2->Get("h666");
  //get the relevant branch
  int nentriesM = ts->GetEntries();
  cout<<"Simc Entries:\t"<<nentriesM<<endl;
  
  TFile *fout = new TFile("run_C12_kine.pdf","RECREATE");
  TString filecO = "C12_kine.pdf";
  TString filecO1 = "C12_kine.pdf[";
  TString filecO2 = "C12_kine.pdf]";

  gROOT->SetBatch(kTRUE);
  gROOT->ForceStyle(); 
  gStyle->SetPalette(kOcean);

  
  //make histograms:
  THStack *hs1 = new THStack("hs1","W (GeV)");
  THStack *hs2 = new THStack("hs2","Missing Energy (GeV)");
  THStack *hs3 = new THStack("hs3","Missing Momentum (GeV/c)");
  THStack *hs4 = new THStack("hs4","Corrected Cointime (ns) ");

   TH1D *h_Emd = new TH1D("h_Emd","Missing Energy (GeV)",200,-0.15,0.25);
   TH1D *h_Emshow = new TH1D("h_Emshow","Missing Energy (GeV)",200,-0.15,0.25);
   TH1D *h_Ems = new TH1D("h_Ems","Missing Energy (GeV)",200,-0.15,0.25);
   TH1D *h_Wd = new TH1D("h_Wdh","W (GeV)", 150, 0.0, 2.0);
   TH1D *h_Ws = new TH1D("h_Wsh","W (GeV)", 150, 0.0, 2.0);
   TH1D *h_pmd = new TH1D("h_pmd","Pm (GeV/c)", 100, -0.02, 0.6);
   TH1D *h_pms = new TH1D("h_pms","Pm (GeV/c)", 100, -0.02, 0.6);
   TH1D *h1PcointimeROC1 = new TH1D("SHMS ROC1 Corrected Coin Time","SHMS ROC1 Corrected Coin Time; cointime [ns]", 200, -10, 10);    
  TH1D *h1PcointimeROC2 = new TH1D("SHMS ROC2 Corrected Coin Time","SHMS ROC2 Corrected Coin Time; cointime [ns]", 200, -10, 10); 
 
  TH1D *s_tmp = new TH1D("s_delta","stmp",100,-12,12);  //tmp hist to get integral    

  TChain *tt = new TChain("T");

  for (int i=0;i<numruns;i++){
    cout << "opening run number: " <<  runnumber[i] <<"\n";
 //read the input file from data
    TString fileNameD = ROOT_FILE_PATH;
    fileNameD += "coin_replay_production_"; //read the root file from data
    fileNameD += runnumber[i]; //read the root file from data
    fileNameD += "_-1.root"; //read the root file from data

    tt->Add(fileNameD);
  }
  //  TFile *f1 = new TFile(fileNameD);
  //  TTree *tt = (TTree*)f1->Get("T");
    //get the relevant branch
   int nentriesD = tt->GetEntries();
   cout<<"Entries:\t"<<nentriesD<<endl;

  Double_t HgtrX, HgtrTh, HgtrY, HgtrPh, hdelta, pdelta;// Ep, Epp, p_e, p_p, defEm;
  Double_t HgtrBetaCalc, PgtrBetaCalc, evtType, PgtrP, HgtrP, PhodStatus, PhodStartTime, PhodfpHitsTime;
  Double_t cointime, HhodStatus, HhodStartTime, HhodfpHitsTime, SHMSpartMass, HMSpartMass;
  Double_t pkinW, pEm, pPm, pbeta, hbeta, hcalepr, hcaletot, hcernpe, pcaletot, pcalepr, pcernpe;
  Double_t TcoinpTRIG1_ROC1_tdcTimeRaw, TcoinpTRIG4_ROC1_tdcTimeRaw, TcoinpTRIG1_ROC2_tdcTimeRaw;
  Double_t TcoinhTRIG1_ROC1_tdcTimeRaw, TcoinhTRIG1_ROC2_tdcTimeRaw, TcoinhTRIG4_ROC1_tdcTimeRaw;
  Double_t TcoinhTRIG4_ROC2_tdcTimeRaw, TcoinpTRIG4_ROC2_tdcTimeRaw;

    // Foal plane coords
    Double_t PdcXfp;
    Double_t PdcXpfp;
    Double_t PdcYfp;
    Double_t PdcYpfp;

    Double_t HdcXfp;
    Double_t HdcXpfp;
    Double_t HdcYfp;
    Double_t HdcYpfp;



  int cnts=0;
  TCut hpdelta;
  TCut evtCut;                                                                   
  TCut eBetaCut;                                                                 
  TCut pBetaCut;                                                                 
  TCut cerCut;                                                                   
  TCut calCut;                                                                   
  TCut hodoTimeCut;
  TCut Wcut;

  tt->SetBranchAddress("P.gtr.p", &PgtrP); 
  tt->SetBranchAddress("H.gtr.p", &HgtrP); 
  tt->SetBranchAddress("P.gtr.beta", &pbeta);                                                                                 
  tt->SetBranchAddress("H.gtr.beta", &hbeta); 
  tt->SetBranchAddress("H.gtr.dp", &hdelta);                                                                                
  tt->SetBranchAddress("P.gtr.dp", &pdelta);                                                                                
                                                                                                                            
  tt->SetBranchAddress("P.cal.eprtracknorm", &pcalepr);                                                                    
  tt->SetBranchAddress("P.cal.etottracknorm", &pcaletot);                                                                  
  tt->SetBranchAddress("P.hgcer.npeSum", &pcernpe);                                                 
                                                                                                             
  tt->SetBranchAddress("H.cal.eprtracknorm", &hcalepr);                                            
  tt->SetBranchAddress("H.cal.etottracknorm", &hcaletot);                                          
  tt->SetBranchAddress("H.cer.npeSum", &hcernpe); 
  tt->SetBranchAddress("H.kin.primary.W", &pkinW);                                                    
  tt->SetBranchAddress("P.kin.secondary.emiss_nuc", &pEm);                                                    
  tt->SetBranchAddress("P.kin.secondary.pmiss", &pPm);                                                    
                                                                                                              
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
    
    tt->SetBranchAddress("H.dc.x_fp", &HdcXfp);
    tt->SetBranchAddress("H.dc.xp_fp", &HdcXpfp);
    tt->SetBranchAddress("H.dc.y_fp", &HdcYfp);
    tt->SetBranchAddress("H.dc.yp_fp", &HdcYpfp);    

    tt->SetBranchAddress("P.dc.x_fp", &PdcXfp);
    tt->SetBranchAddress("P.dc.xp_fp", &PdcXpfp);
    tt->SetBranchAddress("P.dc.y_fp", &PdcYfp);
    tt->SetBranchAddress("P.dc.yp_fp", &PdcYpfp);    

                                                                                                             
  tt->SetBranchAddress("T.coin.pTRIG1_ROC1_tdcTimeRaw", &TcoinpTRIG1_ROC1_tdcTimeRaw);                   
  tt->SetBranchAddress("T.coin.pTRIG4_ROC1_tdcTimeRaw", &TcoinpTRIG4_ROC1_tdcTimeRaw);
  tt->SetBranchAddress("T.coin.pTRIG1_ROC2_tdcTimeRaw", &TcoinpTRIG1_ROC2_tdcTimeRaw);                   
  tt->SetBranchAddress("T.coin.pTRIG4_ROC2_tdcTimeRaw", &TcoinpTRIG4_ROC2_tdcTimeRaw);                   
                                                                                                             
  tt->SetBranchAddress("T.coin.hTRIG1_ROC1_tdcTimeRaw", &TcoinhTRIG1_ROC1_tdcTimeRaw);                   
  tt->SetBranchAddress("T.coin.hTRIG4_ROC1_tdcTimeRaw", &TcoinhTRIG4_ROC1_tdcTimeRaw);                   
  tt->SetBranchAddress("T.coin.hTRIG1_ROC2_tdcTimeRaw", &TcoinhTRIG1_ROC2_tdcTimeRaw);                   
  tt->SetBranchAddress("T.coin.hTRIG4_ROC2_tdcTimeRaw", &TcoinhTRIG4_ROC2_tdcTimeRaw);                   
  Double_t Em_mean, W_mean;
  Double_t Pm_mean;
  
   if(shms_part == "e")                                                      
    {                                                                              
      SHMSpartMass = 0.000510998; // electron mass in GeV/c^2                
      HMSpartMass = 0.93827208; // proton mass in GeV/c^2                  
      hpdelta = "P.gtr.dp > -10 && P.gtr.dp < 10 && H.gtr.dp > -8 && H.gtr.dp < 8";
      eBetaCut = "P.gtr.beta > 0.5 && P.gtr.beta < 1.5";                         
      pBetaCut = "H.gtr.beta > 0.5 &&  H.gtr.beta < 1.5";                        
      cerCut = "P.hgcer.npeSum > 0.1 && H.cer.npeSum < 0.1";                      
      calCut = "P.cal.etottracknorm > 0.8 && P.cal.etottracknorm < 1.5 && P.cal.eprtracknorm  > 0.2";        
      hodoTimeCut ="P.hod.goodstarttime == 1 && H.hod.goodstarttime == 1";
      Wcut = "H.kin.primary.W >0.5 && H.kin.primary.W < 1.5";
 
    }                                                                              
   else                                                                          
    {                                                                             
      SHMSpartMass = 0.93827208; // proton mass in GeV/c^2                 
      HMSpartMass = 0.000510998; // electron mass in GeV/c^2                 
      hpdelta = "P.gtr.dp > -10 && P.gtr.dp < 10 && H.gtr.dp > -8 && H.gtr.dp < 8";
      eBetaCut = "P.gtr.beta > 0.8 && P.gtr.beta < 1.3";                        
      pBetaCut = "H.gtr.beta > 0.8 &&  H.gtr.beta < 1.3";                       
      cerCut = "P.hgcer.npeSum < 0.1 && H.cer.npeSum > 0.5";                    
      calCut = "H.cal.etottracknorm > 0.8 && H.cal.etottracknorm < 1.5 && H.cal.eprtracknorm  > 0.2";         
      hodoTimeCut ="P.hod.goodstarttime == 1 && H.hod.goodstarttime == 1";      
      Wcut = "H.kin.primary.W >0.5 && H.kin.primary.W < 1.5";
    }                                                                             
  TCanvas *ca1 = new TCanvas("ca1","ca1");                           
  tt->Draw("P.kin.secondary.emiss >> histEm", hpdelta && eBetaCut && pBetaCut && cerCut && calCut && hodoTimeCut && Wcut);  
  TH1D *h1Em = (TH1D*)gDirectory->Get("histEm");
  h1Em->GetXaxis()->SetTitle("Missing energy (GeV)");           
  Em_mean = h1Em->GetMean();                     
  TCanvas *ca2 = new TCanvas("ca2","ca2");                           
  tt->Draw("P.kin.secondary.pmiss >> histPm", hpdelta && eBetaCut && pBetaCut && cerCut && calCut && hodoTimeCut && Wcut);  
  TH1D *h1Pm = (TH1D*)gDirectory->Get("histPm");
  h1Pm->GetXaxis()->SetTitle("Missing momentum (GeV/c)");           
  Pm_mean = h1Pm->GetMean();           
  TCanvas *ca3 = new TCanvas("ca3","ca3");                                     
  tt->Draw("H.kin.primary.W >> histW", hpdelta && eBetaCut && pBetaCut && cerCut && calCut && hodoTimeCut && Wcut);  
  TH1D *h1W = (TH1D*)gDirectory->Get("histW");
  h1W->GetXaxis()->SetTitle("W (GeV)");           
  W_mean = h1W->GetMean();                     
  cout << Em_mean <<" Em mean "<< Pm_mean  << " Pm mean " << W_mean <<" W mean " << endl;  

  TCanvas *canvas1 = new TCanvas("canvas1","canvas1");                           
  tt->Draw("P.hod.starttime >> SHMShodoStartTime", eBetaCut && pBetaCut && cerCut && calCut && hodoTimeCut);  
  TH1D *h1PhodoStartTime = (TH1D*)gDirectory->Get("SHMShodoStartTime");
  h1PhodoStartTime->GetXaxis()->SetTitle("SHMS hodo start time [ns]");           
  Double_t PhodoStartTimeMean = h1PhodoStartTime->GetMean();                     
  TCanvas *canvas2 = new TCanvas("canvas2","canvas2");                           
  tt->Draw("H.hod.starttime >> HMShodoStartTime", eBetaCut && pBetaCut && cerCut && calCut && hodoTimeCut);  
  TH1D *h1HhodoStartTime = (TH1D*)gDirectory->Get("HMShodoStartTime");           
  h1HhodoStartTime->GetXaxis()->SetTitle("HMS hodo start time [ns]");            
  Double_t HhodoStartTimeMean = h1HhodoStartTime->GetMean();                     

  cout << PhodoStartTimeMean <<" shms mean "<< HhodoStartTimeMean  << " hms mean " <<endl;
 
  Double_t pOffset = 1.5; //9.5 + 10;  // in ns                                  
  Double_t hOffset = 335;                                                        
  Double_t speedOfLight = 29.9792; // in cm/ns                                   
  Double_t SHMScentralPathLen = 18.1*100;  // SHMS Target to focal plane path length converted to cm  
  Double_t SHMSpathLength = 18.1*100;      // For now assume that it's same as SHMScentralPathLen  
  Double_t HMScentralPathLen = 22*100;     // HMS Target to focal plane path length converted to cm
  Double_t DeltaHMSpathLength;             // For now assume that it's same as HMScentralPathLen 
  Double_t SHMScoinCorr = 0.0;                                                   
  Double_t HMScoinCorr = 0.0;                                                    
  Double_t SHMSrawCoinTimeROC1;                                                  
  Double_t SHMSrawCoinTimeROC2;                                                  
  Double_t HMSrawCoinTimeROC1;                                                   
  Double_t HMSrawCoinTimeROC2;                                                   
                                                                                   
  Double_t SHMScorrCoinTimeROC1;                                                 
  Double_t SHMScorrCoinTimeROC2;                                                 
  Double_t HMScorrCoinTimeROC1;                                                  
  Double_t HMScorrCoinTimeROC2;                                                  
 
  for (int kk=0; kk<nentriesD; kk++){
    tt->GetEntry(kk);
    if (kk % 50000 == 0) cout << kk*100/nentriesD << "   % of data done" << endl;
      evtType = tt->GetLeaf("fEvtHdr.fEvtType")->GetValue(); 
    if (shms_part == "e") 
    {
     if (pbeta>0.8 && pbeta<1.3 && hbeta>0.8 && hbeta<1.3 && pcernpe>0.1 && hcernpe<0.1 && pcaletot >0.8 && pcaletot<1.5 && pcalepr>0.1 && PhodStatus == 1 && HhodStatus ==1 && hdelta > -8 && hdelta < 8 && pdelta > -10 && pdelta < 10) 
     { //cuts to select the electrons and protons
       DeltaHMSpathLength = 12.462*HgtrTh + 0.1138*HgtrTh*HgtrX - 0.0154*HgtrX - 72.292*HgtrTh*HgtrTh - 0.0000544*HgtrX*HgtrX - 116.52*HgtrPh*HgtrPh;               
      PgtrBetaCalc = PgtrP/sqrt(PgtrP*PgtrP + SHMSpartMass*SHMSpartMass);        
      HgtrBetaCalc = HgtrP/sqrt(HgtrP*HgtrP + HMSpartMass*HMSpartMass);          
      SHMScoinCorr = SHMScentralPathLen / (speedOfLight*PgtrBetaCalc) + (SHMSpathLength - SHMScentralPathLen) / (speedOfLight*PgtrBetaCalc) + (PhodoStartTimeMean - PhodfpHitsTime);                                                                   
      HMScoinCorr = HMScentralPathLen / (speedOfLight*HgtrBetaCalc) + DeltaHMSpathLength / (speedOfLight*HgtrBetaCalc) + (HhodoStartTimeMean - HhodfpHitsTime);      
      SHMScorrCoinTimeROC1 = (TcoinpTRIG1_ROC1_tdcTimeRaw*0.1 - SHMScoinCorr) - (TcoinpTRIG4_ROC1_tdcTimeRaw*0.1 - HMScoinCorr) - pOffset; // 0.1 to convert to ns 
      SHMScorrCoinTimeROC2 = (TcoinpTRIG1_ROC2_tdcTimeRaw*0.1 - SHMScoinCorr) - (TcoinpTRIG4_ROC2_tdcTimeRaw*0.1 - HMScoinCorr) - pOffset;                         
      h1PcointimeROC1->Fill(SHMScorrCoinTimeROC1);                               
      h1PcointimeROC2->Fill(SHMScorrCoinTimeROC2);                               
     }
    } 

    else if (shms_part == "p") 
    {

      if (pbeta>0.75 && pbeta<1.35 && hbeta>0.8 && hbeta<1.2 && pcernpe<0.1 && hcernpe>0.5 && hcaletot >0.8 && hcaletot<1.2 && PhodStatus == 1 && HhodStatus ==1 && hdelta > -8 && hdelta < 8 && pdelta > -10 && pdelta < 10) 
      { 
	DeltaHMSpathLength = 12.462*HdcXpfp + 0.1138*HdcXpfp*HdcXfp - 0.0154*HdcXfp - 72.292*HdcXpfp*HdcXpfp - 0.0000544*HdcXfp*HdcXfp - 116.52*HdcYpfp*HdcYpfp;
	//	DeltaHMSpathLength = 12.462*HgtrTh + 0.1138*HgtrTh*HgtrX - 0.0154*HgtrX - 72.292*HgtrTh*HgtrTh - 0.0000544*HgtrX*HgtrX - 116.52*HgtrPh*HgtrPh;               
      PgtrBetaCalc = PgtrP/sqrt(PgtrP*PgtrP + SHMSpartMass*SHMSpartMass);        
      HgtrBetaCalc = HgtrP/sqrt(HgtrP*HgtrP + HMSpartMass*HMSpartMass);          
      SHMScoinCorr = SHMScentralPathLen / (speedOfLight*PgtrBetaCalc) + (SHMSpathLength - SHMScentralPathLen) / (speedOfLight*PgtrBetaCalc) + (PhodoStartTimeMean - PhodfpHitsTime);                                                                   
      HMScoinCorr = HMScentralPathLen / (speedOfLight*HgtrBetaCalc) + DeltaHMSpathLength / (speedOfLight*HgtrBetaCalc) + (HhodoStartTimeMean - HhodfpHitsTime);      
      SHMScorrCoinTimeROC1 = (TcoinpTRIG1_ROC1_tdcTimeRaw*0.1 - SHMScoinCorr) - (TcoinpTRIG4_ROC1_tdcTimeRaw*0.1 - HMScoinCorr) - pOffset; // 0.1 to convert to ns 
      SHMScorrCoinTimeROC2 = (TcoinpTRIG1_ROC2_tdcTimeRaw*0.1 - SHMScoinCorr) - (TcoinpTRIG4_ROC2_tdcTimeRaw*0.1 - HMScoinCorr) - pOffset;                         
      h1PcointimeROC1->Fill(SHMScorrCoinTimeROC1);                               
      h1PcointimeROC2->Fill(SHMScorrCoinTimeROC2);
      }
    }  
  }
  h1PcointimeROC1->GetXaxis()->SetTitle("Corrected Coincidence Time (ns)");
  Double_t ctimemean = h1PcointimeROC1->GetMean();
  Double_t modPm;
  for (int kk=0; kk<nentriesD; kk++){
    tt->GetEntry(kk);
    if (kk % 10000 == 0) cout << kk*100/nentriesD << "   % of data done" << endl;
      evtType = tt->GetLeaf("fEvtHdr.fEvtType")->GetValue(); 
    if (shms_part == "e") 
    {
      if (pbeta>0.8 && pbeta<1.3 && hbeta>0.8 && hbeta<1.3 && pcernpe>0.1 && hcernpe<0.1 && pcaletot >0.8 && pcaletot<1.5 && pcalepr>0.1 && PhodStatus == 1 && HhodStatus ==1 && hdelta > -8 && hdelta < 8 && pdelta > -10 && pdelta < 10) 
      {
         cnts++;
         modPm = sqrt(pPm*pPm);
         h_Emd->Fill(pEm);         
         h_Wd->Fill(pkinW);
         h_pmd->Fill(modPm);
      }
    }
    else if (shms_part == "p") 
    {
      if (pbeta>0.6 && pbeta<1.4 && hbeta>0.8 && hbeta<1.2 && pcernpe<0.1 && hcernpe>0. && hcaletot>0.6 && hcaletot<2.0 && PhodStatus == 1 && HhodStatus ==1 && hdelta > -10 && hdelta < 10 && pdelta > -15 && pdelta < 15 && sqrt(pPm*pPm) < 0.6)     { //cuts to select the electrons and protons
        cnts++;
        modPm = sqrt(pPm*pPm);
        h_Emd->Fill(pEm+0.03);
        h_Emshow->Fill(pEm+0.03);
        h_Wd->Fill(pkinW+0.03);
        h_pmd->Fill(modPm);
      }
    }
  }
  h_Emd->GetXaxis()->SetTitle("Missing energy (GeV)");           
  h_pmd->GetXaxis()->SetTitle("Missing momentum (GeV/c)");
  h_Wd->GetXaxis()->SetTitle("W (GeV)");      

  cout << cnts <<" events passed all cuts" << endl;

  double spdelta, sEm, sPm, sW, wt, sdelta, sfac, normwt;

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

    sdelta = ts->GetLeaf("hsdelta")->GetValue();
    spdelta = ts->GetLeaf("ssdelta")->GetValue();
    sEm = ts->GetLeaf("Em")->GetValue();
    sPm = ts->GetLeaf("Pm")->GetValue();
    sW = ts->GetLeaf("W")->GetValue();
    wt = ts->GetLeaf("Weight")->GetValue();
    normwt = wt*normfac*sfac/nentriesM;
    if (sPm < 0.6 && sdelta < 10 && sdelta > -10 && spdelta > -15 && spdelta < 15 )
    {
     h_Ems->Fill(sEm,normwt);
     h_pms->Fill(sPm,normwt);
     h_Ws->Fill(sW,normwt);
    }
  }
  h_Ems->GetXaxis()->SetTitle("Missing energy (GeV)");           
  h_pms->GetXaxis()->SetTitle("Missing momentum (GeV/c)");           
  h_Ws->GetXaxis()->SetTitle("W (GeV)");           
  
  //  hs2->GetXaxis()->SetTitle("Missing energy (GeV)");           
  //hs2->GetXaxis()->SetTitle("Missing momentum (GeV/c)");           
  //hs1->GetXaxis()->SetTitle("W (GeV)");           
  //hs4->GetXaxis()->SetTitle("Corrected Cointime (ns)");           

  TCanvas *ccshow = new TCanvas ("ccs","ccs",800,600);
  ccshow->SetGrid();
  ccshow->SetLeftMargin(0.2);
  ccshow->SetRightMargin(0.2);
  gStyle->SetOptStat(0);
  ccshow->cd(1);
  h_Emshow->SetFillColor(kBlue);
  h_Emshow->SetMarkerColor(kBlue);
  h_Emshow->SetLineColor(kBlue);
  h_Emshow->Draw();
  ccshow->SaveAs("showEm_c12.pdf");

  TCanvas *cc = new TCanvas ("cc","cc",800,600);
  cc->SetGrid();
  cc->SetLeftMargin(0.2);
  cc->SetRightMargin(0.2);
  gStyle->SetOptStat(0);

  cc->Print(filecO1);
  cc->cd(1);
  h_Wd->SetFillColor(kBlue);
  h_Wd->SetMarkerColor(kBlue);
  h_Wd->SetLineColor(kBlue);
  hs1->Add(h_Wd);
  h_Ws->SetFillStyle(1001);
  h_Ws->SetMarkerColor(kRed);
  h_Ws->SetLineColor(kRed);
  hs1->Add(h_Ws);
  hs1->Draw("pfc nostack");

  cc->Print(filecO);

  cc->cd(1);
  h_Emd->SetFillColor(kBlue);
  h_Emd->SetMarkerColor(kBlue);
  h_Emd->SetLineColor(kBlue);
  hs2->Add(h_Emd);
  h_Ems->SetFillStyle(1001);
  h_Ems->SetLineColor(kRed);
  h_Ems->SetMarkerColor(kRed);
  hs2->Add(h_Ems);
  hs2->Draw("pfc nostack");
  cc->Print(filecO);
 
  cc->cd(1);
  h_pmd->SetFillColor(kBlue);
  h_pmd->SetMarkerColor(kBlue);
  h_pmd->SetLineColor(kBlue);
  hs3->Add(h_pmd);
  h_pms->SetFillStyle(1001);
  h_pms->SetMarkerColor(kRed);
  h_pms->SetLineColor(kRed);
  hs3->Add(h_pms);
  hs3->Draw("pfc nostack");
  cc->Print(filecO);
  
  cc->cd(1);
  h1PcointimeROC1->SetFillStyle(1001);
  h1PcointimeROC1->SetMarkerColor(kBlue);
  h1PcointimeROC1->SetLineColor(kBlue);
  hs4->Add(h1PcointimeROC1);
  //  h1PcointimeROC2->SetLineColor(kBlue);
  //hs4->Add(h1PcointimeROC2);
  hs4->Draw("pfc nostack");

  cc->Print(filecO);

  cc->Print(filecO2);

  gROOT->SetBatch(kFALSE);
  gStyle->SetPaperSize(20,20);
  TCanvas *c1 = new TCanvas ("cc1","cc1",800,600);
  c1->SetGrid();
  c1->SetLeftMargin(0.2);
  c1->SetRightMargin(0.2);
  c1->Divide(2,2);

  c1->cd(1);
  hs1->Draw("pfc nostack");
  
  c1->cd(2);
  hs2->Draw("pfc nostack");
 
 
  c1->cd(3);
  hs3->Draw("pfc nostack");


  c1->cd(4);
  hs4->Draw("pfc nostack");


  c1->SaveAs("tmpc12.pdf");

  fout->Write();
  //  fout->Close();

}
