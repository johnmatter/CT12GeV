// Filename: Plot_Data_Simc.cc
// Description: Plot, delta, xptar and yptar distributions for SHMS and HMS to compare between data and SIMC

// Needs input of runnumber, target h/c, e/p in SHMS, root filename and  normfac from SIMC


#include <iostream>
#include <TFile.h>
#include <TTree.h>
#include <TLeaf.h>
#include <TChain.h>
#include <TString.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TCanvas.h>
#include <TStyle.h>
#include <TLeaf.h>
#include <TROOT.h>
#include <THStack.h>
#include "Constants.h"

using namespace std;


void Plot_Data_Simc(Int_t numruns, TString targ, TString shms_part, TString simfile, Double_t normfac){


  //     const Int_t runnumber[numruns]={2023, 2046, 2047, 2048, 2050, 2051, 2052};
  //  const Int_t runnumber[numruns]={2406, 2407, 2408, 2409, 2410, 2411};
  const Int_t runnumber[numruns]={2452, 2453, 2464};
  //  const Int_t runnumber[numruns]={2187, 2188, 2189, 2190, 2191, 2192, 2193, 2194, 2195, 2196, 2197, 2198, 2199, 2200, 2201, 2202, 2203, 2204, 2205, 2206, 2207, 2208, 2209, 2211, 2212};
  //  const Int_t runnumber[numruns]={2278, 2279, 2280};
  //    const Int_t runnumber[numruns]={2283};
       //  const Int_t runnumber[numruns]={2284, 2285, 2286, 2290, 2291, 2292, 2293, 2294, 2295, 2296, 2297, 2298, 2299, 2300, 2301, 2303, 2304, 2305, 2306, 2308, 2309, 2310, 2311, 2312, 2313, 2314, 2315, 2316, 2317, 2318, 2319, 2320, 2321, 2322, 2324, 2325, 2343, 2344, 2345, 2346, 2347};

  TString fileNameM = ROOT_FILE_PATH;
  fileNameM += simfile; //read the root file from SIMC
  TFile *f2 = new TFile(fileNameM);
  TTree *ts = (TTree*)f2->Get("h666");
  //get the relevant branch
  int nentriesM = ts->GetEntries();
  cout<<"Simc Entries:\t"<<nentriesM<<endl;
  TString ofilename = targ; 
  ofilename += "_hists_comp.root";
  TString fileO = targ;
  TString fileO1 = targ;
  TString fileO2 = targ;
  fileO += "_data_simc_comp.pdf";
  fileO1 += "_data_simc_comp.pdf[";
  fileO2 += "_data_simc_comp.pdf]";
  TFile *fout = new TFile(ofilename,"RECREATE");


  gROOT->SetBatch(kTRUE);
  gROOT->ForceStyle(); 
  gStyle->SetPalette(kOcean);
  TCanvas *cc = new TCanvas ("cc","cc",800,600);
  cc->SetGrid();
  cc->SetLeftMargin(0.2);
  cc->SetRightMargin(0.2);
  gStyle->SetOptStat(0);
  
  //make histograms:
  THStack *hs1 = new THStack("hs1","HMS DELTA (%)");
  THStack *hs2 = new THStack("hs2","HMS XPTAR (rad)");
  THStack *hs3 = new THStack("hs3","HMS YPTAR (rad)");
  THStack *hs4 = new THStack("hs4","HMS YTAR (cm)");
  THStack *hs5 = new THStack("hs5","SHMS DELTA (%)");
  THStack *hs6 = new THStack("hs6","SHMS XPTAR (rad)");
  THStack *hs7 = new THStack("hs7","SHMS YPTAR (rad)");
  THStack *hs8 = new THStack("hs8","SHMS YTAR (cm)");

  TH1D *h_hdelta = new TH1D("h_hdelta","HMS DELTA (%)",100,-12,12);
  TH1D *h_hxptar = new TH1D("h_hxptar","HMS XPTAR (rad)",100,-0.1,0.1);
  TH1D *h_hyptar = new TH1D("h_hyptar","HMS YPTAR (rad)",100,-0.06,0.06);
  TH1D *h_hytar = new TH1D("h_hytar","HMS YTAR (cm)",100,-12.0,12.0);

  TH1D *h_pdelta = new TH1D("h_pdelta","SHMS DELTA (%)",100,-12,12);
  TH1D *h_pxptar = new TH1D("h_sxptar","SHMS XPTAR (rad)",100,-0.06,0.06);
  TH1D *h_pyptar = new TH1D("h_syptar","SHMS YPTAR (rad)",100,-0.06,0.06);
  TH1D *h_pytar = new TH1D("h_sytar","SHMS YTAR (cm)",100,-12.0,12.0);

  TH1D *s_hdelta = new TH1D("s_hdelta","HMS DELTA (%)",100,-12,12);
  TH1D *s_hxptar = new TH1D("s_hxptar","HMS XPTAR (rad)",100,-0.1,0.1);
  TH1D *s_hyptar = new TH1D("s_hyptar","HMS YPTAR (rad)",100,-0.06,0.06);
  TH1D *s_hytar = new TH1D("s_hytar","HMS YTAR (cm)",100,-12.0,12.0);
  TH1D *s_pdelta = new TH1D("s_pdelta","SHMS DELTA (%)",100,-12,12);
  TH1D *s_pxptar = new TH1D("s_sxptar","SHMS XPTAR (rad)",100,-0.06,0.06);
  TH1D *s_pyptar = new TH1D("s_syptar","SHMS YPTAR (rad)",100,-0.06,0.06);
  TH1D *s_pytar = new TH1D("s_sytar","SHMS YTAR (cm)",100,-12.0,12.0);

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

 
  double hcernpe, hbeta, deltap, pxptar, pyptar, pcalepr, pcaletot, pcernpe;
  double pbeta, deltae, exptar, eyptar, pytar, eytar, hcaletot, hcalepr;
  int cnts=0;

  for (int kk=0; kk<nentriesD; kk++){
    tt->GetEntry(kk);
    if (kk % 50000 == 0) cout << kk*100/nentriesD << "   % of data done" << endl;
     Double_t evtType = tt->GetLeaf("fEvtHdr.fEvtType")->GetValue(); 
    if (shms_part == "e") 
    {
     hcernpe = tt->GetLeaf("H.cer.npeSum")->GetValue();
     hbeta = tt->GetLeaf("H.gtr.beta")->GetValue();
     deltap = tt->GetLeaf("H.gtr.dp")->GetValue();
     pxptar = tt->GetLeaf("H.gtr.th")->GetValue();
     pyptar = tt->GetLeaf("H.gtr.ph")->GetValue();
     pytar = tt->GetLeaf("H.gtr.y")->GetValue();

     pcalepr = tt->GetLeaf("P.cal.eprtracknorm")->GetValue();
     pcaletot = tt->GetLeaf("P.cal.etottracknorm")->GetValue();
     pcernpe = tt->GetLeaf("P.hgcer.npeSum")->GetValue();
     pbeta = tt->GetLeaf("P.gtr.beta")->GetValue();
     deltae = tt->GetLeaf("P.gtr.dp")->GetValue();
     exptar = tt->GetLeaf("P.gtr.th")->GetValue();
     eyptar = tt->GetLeaf("P.gtr.ph")->GetValue();    
     eytar = tt->GetLeaf("P.gtr.y")->GetValue();

     if (pbeta>0.6 && pbeta<1.4 && hbeta>0.8 && hbeta<1.3 && pcernpe>0.1 && hcernpe<0.1 && pcaletot >0.8 && pcaletot<1.5 && pcalepr>0.1) { //cuts to select the electrons and protons
      cnts++;
      h_hdelta->Fill(deltap);
      h_hxptar->Fill(pxptar);
      h_hyptar->Fill(pyptar);
      h_hytar->Fill(pytar);
      h_pdelta->Fill(deltae);
      h_pxptar->Fill(exptar);
      h_pyptar->Fill(eyptar);     
      h_pytar->Fill(eytar);
     }
    } 
    else if (shms_part == "p") 
    {
     hcalepr = tt->GetLeaf("H.cal.eprtracknorm")->GetValue();
     hcaletot = tt->GetLeaf("H.cal.etottracknorm")->GetValue();
     hcernpe = tt->GetLeaf("H.cer.npeSum")->GetValue();
     hbeta = tt->GetLeaf("H.gtr.beta")->GetValue();
     deltae = tt->GetLeaf("H.gtr.dp")->GetValue();
     exptar = tt->GetLeaf("H.gtr.th")->GetValue();
     eyptar = tt->GetLeaf("H.gtr.ph")->GetValue();
     eytar = tt->GetLeaf("H.gtr.y")->GetValue();

     pcernpe = tt->GetLeaf("P.hgcer.npeSum")->GetValue();
     pbeta = tt->GetLeaf("P.gtr.beta")->GetValue();
     deltap = tt->GetLeaf("P.gtr.dp")->GetValue();
     pxptar = tt->GetLeaf("P.gtr.th")->GetValue();
     pyptar = tt->GetLeaf("P.gtr.ph")->GetValue();    
     pytar = tt->GetLeaf("P.gtr.y")->GetValue();

     if (pbeta>0.6 && pbeta<1.4 && hbeta>0.8 && hbeta<1.2 && pcernpe<0.5 && hcernpe>0. && hcaletot >0.7 && hcaletot<1.65) { //cuts to select the electrons and protons
      cnts++;
      h_hdelta->Fill(deltae);
      h_hxptar->Fill(exptar);
      h_hyptar->Fill(eyptar);
      h_hytar->Fill(eytar);
      h_pdelta->Fill(deltap);
      h_pxptar->Fill(pxptar);
      h_pyptar->Fill(pyptar);
      h_pytar->Fill(pytar);
     } 
    }
  }
  cout << cnts <<" events passed all cuts" << endl;

  double shxptar, shyptar, spxptar, spyptar, wt, sdeltap, sdeltae, scalefac; 
  double normwt, shytar, spytar;

  for (int kk=0; kk<nentriesM; kk++){
    ts->GetEntry(kk);
    if (kk % 50000 == 0) cout << kk*100/nentriesM << "   % of simc done" << endl;

    sdeltap = ts->GetLeaf("hsdelta")->GetValue();
    wt = ts->GetLeaf("Weight")->GetValue();
    normwt = wt*normfac/nentriesM;
    s_tmp->Fill(sdeltap,normwt);
  }
  double intgrl = s_tmp->Integral(1,100); // integral of the SIMC histogram

  scalefac = cnts/intgrl;  // scale factor to make the intregral of both Data and SIMC histrograms are equal
  cout << "scalefac =" << scalefac << endl;

  for (int kk=0; kk<nentriesM; kk++){
    ts->GetEntry(kk);
    if (kk % 50000 == 0) cout << kk*100/nentriesM << "   % of simc done 2nd time" << endl;

    sdeltap = ts->GetLeaf("hsdelta")->GetValue();
    shxptar = ts->GetLeaf("hsxptar")->GetValue();
    shyptar = ts->GetLeaf("hsyptar")->GetValue();
    shytar = ts->GetLeaf("hsytar")->GetValue();
    sdeltae = ts->GetLeaf("ssdelta")->GetValue();
    spxptar = ts->GetLeaf("ssxptar")->GetValue();
    spyptar = ts->GetLeaf("ssyptar")->GetValue();
    spytar = ts->GetLeaf("ssytar")->GetValue();
    wt = ts->GetLeaf("Weight")->GetValue();
    normwt = wt*normfac*scalefac/nentriesM;

    s_hdelta->Fill(sdeltap,normwt);
    s_hxptar->Fill(shxptar,normwt);
    s_hyptar->Fill(shyptar,normwt);
    s_hytar->Fill(shytar,normwt);
    s_pdelta->Fill(sdeltae,normwt);
    s_pxptar->Fill(spxptar,normwt);
    s_pyptar->Fill(spyptar,normwt);
    s_pytar->Fill(spytar,normwt);
  }


  cc->Print(fileO1);

 
  cc->cd(1);
  h_hdelta->SetFillStyle(1001);
  h_hdelta->SetMarkerColor(kBlue);
  h_hdelta->SetLineColor(kBlue);
  hs1->Add(h_hdelta);
  s_hdelta->SetFillColor(kRed);
  s_hdelta->SetMarkerColor(kRed);
  hs1->Add(s_hdelta);
  hs1->Draw("pfc nostack");
  
  cc->Print(fileO);
  cc->cd(1);

  h_hxptar->SetFillStyle(1);
  h_hxptar->SetMarkerColor(kBlue);
  h_hxptar->SetLineColor(kBlue);
  hs2->Add(h_hxptar);
  s_hxptar->SetFillColor(kRed);
  s_hxptar->SetMarkerColor(kRed);
  hs2->Add(s_hxptar);
  hs2->Draw("pfc nostack");
  cc->Print(fileO);
 
  cc->cd(1);
  h_hyptar->SetFillStyle(1);
  h_hyptar->SetMarkerColor(kBlue);
  h_hyptar->SetLineColor(kBlue);
  hs3->Add(h_hyptar);
  s_hyptar->SetFillColor(kRed);
  s_hyptar->SetMarkerColor(kRed);
  hs3->Add(s_hyptar);
  hs3->Draw("pfc nostack");
  cc->Print(fileO);

  cc->cd(1);
  h_hytar->SetFillStyle(1);
  h_hytar->SetMarkerColor(kBlue);
  h_hytar->SetLineColor(kBlue);
  hs4->Add(h_hytar);
  s_hytar->SetFillColor(kRed);
  s_hytar->SetMarkerColor(kRed);
  hs4->Add(s_hytar);
  hs4->Draw("pfc nostack");
  cc->Print(fileO);


  cc->cd(1);
  h_pdelta->SetFillStyle(1);
  h_pdelta->SetMarkerColor(kBlue);
  h_pdelta->SetLineColor(kBlue);
  hs5->Add(h_pdelta);
  s_pdelta->SetFillColor(kRed);
  s_pdelta->SetMarkerColor(kRed);
  hs5->Add(s_pdelta);
  hs5->Draw("pfc nostack");
  cc->Print(fileO);


  cc->cd(1);
  h_pxptar->SetFillStyle(1);
  h_pxptar->SetMarkerColor(kBlue);
  h_pxptar->SetLineColor(kBlue);
  hs6->Add(h_pxptar);
  s_pxptar->SetFillColor(kRed);
  s_pxptar->SetMarkerColor(kRed);
  hs6->Add(s_pxptar);
  hs6->Draw("pfc nostack");
  cc->Print(fileO);

  cc->cd(1);
  h_pyptar->SetFillStyle(1);
  h_pyptar->SetMarkerColor(kBlue);
  h_pyptar->SetLineColor(kBlue);
  hs7->Add(h_pyptar);
  s_pyptar->SetFillColor(kRed);
  s_pyptar->SetMarkerColor(kRed);
  hs7->Add(s_pyptar);
  hs7->Draw("pfc nostack");
  cc->Print(fileO);

  cc->cd(1);
  h_pytar->SetFillStyle(1);
  h_pytar->SetMarkerColor(kBlue);
  h_pytar->SetLineColor(kBlue);
  hs8->Add(h_pytar);
  s_pytar->SetFillColor(kRed);
  s_pytar->SetMarkerColor(kRed);
  hs8->Add(s_pytar);
  hs8->Draw("pfc nostack");
  cc->Print(fileO);

  cc->Print(fileO2);

  gROOT->SetBatch(kFALSE);
  TCanvas *c1 = new TCanvas ("cc1","cc1",800,600);
  c1->SetGrid();
  c1->SetLeftMargin(0.2);
  c1->SetRightMargin(0.2);
  c1->Divide(4,2);

  c1->cd(1);
  hs1->Draw("pfc nostack");
  
  c1->cd(2);
  hs2->Draw("pfc nostack");
 
 
  c1->cd(3);
  hs3->Draw("pfc nostack");


  c1->cd(4);
  hs4->Draw("pfc nostack");
 


  c1->cd(5);
  hs5->Draw("pfc nostack");



  c1->cd(6);
  hs6->Draw("pfc nostack");


  c1->cd(7);
  hs7->Draw("pfc nostack");

  c1->cd(8);
  hs8->Draw("pfc nostack");

  c1->SaveAs("tmp.pdf");
  
  fout->Write();
  fout->Close();

}

