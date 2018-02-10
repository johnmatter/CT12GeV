// Filename: Plot_Data_Simc_eSHMS.cc
// Description: Plot, delta, xptar and yptar distributions for SHMS and HMS to compare between data and SIMC
// This is for the case when electron is in the SHMS. 

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

const double normfac = 0.950677E+07; //SIMC normfac
const char* fileeD = "ep_1851.root"; // Data filename
const char* fileeM = "h1_3.root"; // SIMC file name
const char* fileeO = "run_1851_data_simc_comp.pdf"; //output pdf file name
void Plot_Data_Simc_eSHMS(){

  TString fileNameD = ROOT_FILE_PATH;
  fileNameD += fileeD; //read the root file from data
  TFile *f1 = new TFile(fileNameD);
  TTree *tt = (TTree*)f1->Get("T");
  //get the relevant branch
  int nentriesD = tt->GetEntries();
  cout<<"Entries:\t"<<nentriesD<<endl;

  TString fileNameM = ROOT_FILE_PATH;
  fileNameM += fileeM; //read the root file from SIMC
  TFile *f2 = new TFile(fileNameM);
  TTree *ts = (TTree*)f2->Get("h666");
  //get the relevant branch
  int nentriesM = ts->GetEntries();
  cout<<"Simc Entries:\t"<<nentriesM<<endl;

  TFile *fout = new TFile("run_hists_comp.root","RECREATE");
  //  gROOT->SetBatch(kTRUE);
  gROOT->ForceStyle(); 
  gStyle->SetPalette(kOcean);

  TCanvas *cc = new TCanvas ("cc","cc",800,600);
  cc->SetGrid();
  cc->SetLeftMargin(0.2);
  cc->SetRightMargin(0.2);
  cc->Divide(4,2);
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
 
  double hcernpe, hbeta, deltap, pxptar, pyptar, pcalepr, pcaletot, pcernpe;
  double pbeta, deltae, exptar, eyptar, pytar, eytar;
  int cnts=0;

  for (int kk=0; kk<nentriesD; kk++){
    tt->GetEntry(kk);
    if (kk % 50000 == 0) cout << kk*100/nentriesD << "   % of data done" << endl;

    Double_t evtType = tt->GetLeaf("fEvtHdr.fEvtType")->GetValue(); 
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
  hs8->Add(h_pyptar);
  hs8->Add(s_pyptar);
  hs8->Draw("pfc nostack");

  cc->SaveAs(fileeO);


  
  fout->Write();
  fout->Close();

}

