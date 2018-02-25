// Filename: Plot_Data_Simc_eSHMS.cc
// Description: Plot, delta, xptar and yptar distributions for SHMS and HMS to compare between data and SIMC
// This is for the case when proton is in the SHMS. 

// Needs root file name.
// Also needs normfac from SIMC and the simc root file name
// Output file name also declared at the top of the code.

#include <iostream>
#include <TFile.h>
#include <TTree.h>
#include <TLeaf.h>
#include <TString.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TCanvas.h>
#include <TStyle.h>
#include <TROOT.h>
#include <THStack.h>
#include "Constants.h"

using namespace std;

const double normfac = 0.972812E+07; //SIMC normfac
const char* fileD = "ep_1872.root"; // Data filename
const char* fileM = "h1_4.root"; // SIMC file name
const char* fileO = "run_1872_data_simc_comp.pdf"; //output pdf file name

void Plot_Data_Simc_pSHMS(){

  TString fileNameD = ROOT_FILE_PATH;
  fileNameD += fileD; //read the root file from data
  TFile *f1 = new TFile(fileNameD);
  TTree *tt = (TTree*)f1->Get("T");
  //get the relevant branch
  int nentriesD = tt->GetEntries();
  cout<<"Entries:\t"<<nentriesD<<endl;

  TString fileNameM = ROOT_FILE_PATH;
  fileNameM += fileM; //read the root file from SIMC
  TFile *f2 = new TFile(fileNameM);
  TTree *ts = (TTree*)f2->Get("h666");
  //get the relevant branch
  int nentriesM = ts->GetEntries();
  cout<<"Simc Entries:\t"<<nentriesM<<endl;

  TFile *fout = new TFile("run_hists_comp.root","RECREATE");
  //  gROOT->SetBatch(kTRUE);

  
  //make histograms:
  THStack *hs1 = new THStack("hs1","HMS DELTA (%)");
  THStack *hs2 = new THStack("hs1","HMS XPTAR (rad)");
  THStack *hs3 = new THStack("hs1","HMS YPTAR (rad)");
  THStack *hs4 = new THStack("hs4","HMS YTAR (cm)");
  THStack *hs5 = new THStack("hs5","SHMS DELTA (%)");
  THStack *hs6 = new THStack("hs6","SHMS XPTAR (rad)");
  THStack *hs7 = new THStack("hs7","SHMS YPTAR (rad)");
  THStack *hs8 = new THStack("hs8","SHMS YTAR (cm)");

  TH1F *h_hdelta = new TH1F("h_hdelta","HMS DELTA (%)",100,-12,12);
  TH1F *h_hxptar = new TH1F("h_hxptar","HMS XPTAR (rad)",100,-0.1,0.1);
  TH1F *h_hyptar = new TH1F("h_hyptar","HMS YPTAR (rad)",100,-0.06,0.06);
  TH1F *h_hytar = new TH1F("h_hytar","HMS YTAR (cm)",100,-12.0,12.0);

  TH1F *h_pdelta = new TH1F("h_pdelta","SHMS DELTA (%)",100,-12,12);
  TH1F *h_pxptar = new TH1F("h_sxptar","SHMS XPTAR (rad)",100,-0.06,0.06);
  TH1F *h_pyptar = new TH1F("h_syptar","SHMS YPTAR (rad)",100,-0.06,0.06);
  TH1F *h_pytar = new TH1F("h_sytar","SHMS YTAR (cm)",100,-12.0,12.0);

  TH1F *s_hdelta = new TH1F("s_hdelta","HMS DELTA (%)",100,-12,12);
  TH1F *s_hxptar = new TH1F("s_hxptar","HMS XPTAR (rad)",100,-0.1,0.1);
  TH1F *s_hyptar = new TH1F("s_hyptar","HMS YPTAR (rad)",100,-0.06,0.06);
  TH1F *s_hytar = new TH1F("s_hytar","HMS YTAR (cm)",100,-12.0,12.0);
  TH1F *s_pdelta = new TH1F("s_pdelta","SHMS DELTA (%)",100,-12,12);
  TH1F *s_pxptar = new TH1F("s_sxptar","SHMS XPTAR (rad)",100,-0.06,0.06);
  TH1F *s_pyptar = new TH1F("s_syptar","SHMS YPTAR (rad)",100,-0.06,0.06);
  TH1F *s_pytar = new TH1F("s_sytar","SHMS YTAR (cm)",100,-12.0,12.0);

  TH1F *s_tmp = new TH1F("s_delta","stmp",100,-12,12);  //tmp hist to get integral
 
  double hcernpe, hbeta, deltap, pxptar, pyptar, hcalepr, hcaletot, pcernpe;
  double pbeta, deltae, exptar, eyptar, pytar, eytar;
  int cnts=0;

  for (int kk=0; kk<nentriesD; kk++){
    tt->GetEntry(kk);
    if (kk % 50000 == 0) cout << kk*100/nentriesD << "   % of data done" << endl;

    Double_t evtType = tt->GetLeaf("fEvtHdr.fEvtType")->GetValue(); 
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

    if (pbeta>0.6 && pbeta<1.4 && hbeta>0.8 && hbeta<1.2 && pcernpe<0.1 && hcernpe>0.1 && hcaletot >0.8 && hcaletot<1.2) { //cuts to select the electrons and protons
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

  for (int kk=0; kk<nentriesM; kk++){
    ts->GetEntry(kk);
    if (kk % 50000 == 0) cout << kk*100/nentriesM << "   % of simc done 2nd time" << endl;

    sdeltae = ts->GetLeaf("hsdelta")->GetValue();
    shxptar = ts->GetLeaf("hsxptar")->GetValue();
    shyptar = ts->GetLeaf("hsyptar")->GetValue();
    shytar = ts->GetLeaf("hsytar")->GetValue();
    sdeltap = ts->GetLeaf("ssdelta")->GetValue();
    spxptar = ts->GetLeaf("ssxptar")->GetValue();
    spyptar = ts->GetLeaf("ssyptar")->GetValue();
    spytar = ts->GetLeaf("ssytar")->GetValue();
    wt = ts->GetLeaf("Weight")->GetValue();
    normwt = wt*normfac*scalefac/nentriesM;

    s_hdelta->Fill(sdeltae,normwt);
    s_hxptar->Fill(shxptar,normwt);
    s_hyptar->Fill(shyptar,normwt);
    s_hytar->Fill(shytar,normwt);
    s_pdelta->Fill(sdeltap,normwt);
    s_pxptar->Fill(spxptar,normwt);
    s_pyptar->Fill(spyptar,normwt);
    s_pytar->Fill(spytar,normwt);
  }
  
  gROOT->ForceStyle(); 
  gStyle->SetPalette(kOcean);
  TCanvas *cc = new TCanvas ("cc","cc",800,600);
  cc->SetGrid();
  cc->SetLeftMargin(0.2);
  cc->SetRightMargin(0.2);
  cc->Divide(4,2);
  gStyle->SetOptStat(0);
  cc->cd(1);
  hs1->Add(h_hdelta);
  hs1->Add(s_hdelta);
  hs1->Draw("pfc nostack");
  

  cc->cd(2);
  hs2->Add(h_hxptar);
  hs2->Add(s_hxptar);
  hs2->Draw("pfc nostack");
 
  cc->cd(3);
  hs3->Add(h_hyptar);
  hs3->Add(s_hyptar);
  hs3->Draw("pfc nostack");

  cc->cd(4);
  hs4->Add(h_hytar);
  hs4->Add(s_hytar);
  hs4->Draw("pfc nostack");

  cc->cd(5);
  hs5->Add(h_pdelta);
  hs5->Add(s_pdelta);
  hs5->Draw("pfc nostack");

  cc->cd(6);
  hs6->Add(h_pxptar);
  hs6->Add(s_pxptar);
  hs6->Draw("pfc nostack");

  cc->cd(7);
  hs7->Add(h_pyptar);
  hs7->Add(s_pyptar);
  hs7->Draw("pfc nostack");


  cc->cd(8);
  hs8->Add(h_pytar);
  hs8->Add(s_pytar);
  hs8->Draw("pfc nostack");


  cc->SaveAs(fileO);


  
  fout->Write();
  fout->Close();

}

