// Filename: Plot_Data_Simc.cc
// Description: Plot, delta, xptar and yptar distributions for SHMS and HMS to compare between data and SIMC

// Needs input of runnumber, e/p in SHMS, root filename and  normfac from SIMC


#include <iostream>
#include <TFile.h>
#include <TTree.h>
#include <TLeaf.h>
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

//const double normfac = 0.950677E+07; //SIMC normfac
//const char* fileeM2 = "h1_3.root"; // SIMC file name

void Plot_Data_Simc(Int_t runNumber,  TString shms_part, TString simfile, Double_t normfac){

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

  TFile *fout = new TFile("run_hists_comp.root","RECREATE");

  TString fileO;
   fileO += "run_"; //read the root file from data
   fileO += runNumber; //read the root file from data
   fileO += "_data_simc_comp.pdf"; //read the root file from data
  TString fileO1;
   fileO1 += "run_"; //read the root file from data
   fileO1 += runNumber; //read the root file from data
   fileO1 += "_data_simc_comp.pdf["; //read the root file from data
  TString fileO2;
   fileO2 += "run_"; //read the root file from data
   fileO2 += runNumber; //read the root file from data
   fileO2 += "_data_simc_comp.pdf]"; //read the root file from data


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
  h_hdelta->SetMarkerColor(kRed);
  h_hdelta->SetLineColor(kRed);
  hs1->Add(h_hdelta);
  s_hdelta->SetFillColor(kBlue);
  s_hdelta->SetMarkerColor(kBlue);
  hs1->Add(s_hdelta);
  hs1->Draw("pfc nostack");
  
  cc->Print(fileO);
  cc->cd(1);

  h_hxptar->SetFillStyle(1);
  h_hxptar->SetMarkerColor(kRed);
  h_hxptar->SetLineColor(kRed);
  hs2->Add(h_hxptar);
  s_hxptar->SetFillColor(kBlue);
  s_hxptar->SetMarkerColor(kBlue);
  hs2->Add(s_hxptar);
  hs2->Draw("pfc nostack");
  cc->Print(fileO);
 
  cc->cd(1);
  h_hyptar->SetFillStyle(1);
  h_hyptar->SetMarkerColor(kRed);
  h_hyptar->SetLineColor(kRed);
  hs3->Add(h_hyptar);
  s_hyptar->SetFillColor(kBlue);
  s_hyptar->SetMarkerColor(kBlue);
  hs3->Add(s_hyptar);
  hs3->Draw("pfc nostack");
  cc->Print(fileO);

  cc->cd(1);
  h_hytar->SetFillStyle(1);
  h_hytar->SetMarkerColor(kRed);
  h_hytar->SetLineColor(kRed);
  hs4->Add(h_hytar);
  s_hytar->SetFillColor(kBlue);
  s_hytar->SetMarkerColor(kBlue);
  hs4->Add(s_hytar);
  hs4->Draw("pfc nostack");
  cc->Print(fileO);


  cc->cd(1);
  h_pdelta->SetFillStyle(1);
  h_pdelta->SetMarkerColor(kRed);
  h_pdelta->SetLineColor(kRed);
  hs5->Add(h_pdelta);
  s_pdelta->SetFillColor(kBlue);
  s_pdelta->SetMarkerColor(kBlue);
  hs5->Add(s_pdelta);
  hs5->Draw("pfc nostack");
  cc->Print(fileO);


  cc->cd(1);
  h_pxptar->SetFillStyle(1);
  h_pxptar->SetMarkerColor(kRed);
  h_pxptar->SetLineColor(kRed);
  hs6->Add(h_pxptar);
  s_pxptar->SetFillColor(kBlue);
  s_pxptar->SetMarkerColor(kBlue);
  hs6->Add(s_pxptar);
  hs6->Draw("pfc nostack");
  cc->Print(fileO);

  cc->cd(1);
  h_pyptar->SetFillStyle(1);
  h_pyptar->SetMarkerColor(kRed);
  h_pyptar->SetLineColor(kRed);
  hs7->Add(h_pyptar);
  s_pyptar->SetFillColor(kBlue);
  s_pyptar->SetMarkerColor(kBlue);
  hs7->Add(s_pyptar);
  hs7->Draw("pfc nostack");
  cc->Print(fileO);

  cc->cd(1);
  h_pytar->SetFillStyle(1);
  h_pytar->SetMarkerColor(kRed);
  h_pytar->SetLineColor(kRed);
  hs8->Add(h_pytar);
  s_pytar->SetFillColor(kBlue);
  s_pytar->SetMarkerColor(kBlue);
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

