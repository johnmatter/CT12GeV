// Filename: CTexample.cc
// Description: Look at some distributions by looping over each event. The main purpose is to show how to loop over events.
// Author: Latif Kabir < latif@jlab.org >
// Created: Thu Jan  4 17:03:26 2018 (-0500)
// URL: jlab.org/~latif

#include <iostream>
#include <TFile.h>
#include <TTree.h>
#include <TLeaf.h>
#include <TString.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TCanvas.h>
#include <TSystem.h>
#include <TStyle.h>
#include <TTreeReader.h>
#include <TTreeReaderValue.h>
#include <TTreeReaderArray.h>
#include "Constants.h"

using namespace std;


void CTexample()
{
    TString fileName = ROOT_FILE_PATH;
    // fileName += "coin_replay_production_1487_10000.root";
    fileName += "shms_replay_production_1791_50000.root";
    Bool_t runNotFound = gSystem->AccessPathName(fileName);
    if(runNotFound)
    {	    
	cout << "Requested file " << fileName << " does NOT exist" <<endl;
	return;
    }
    gStyle->SetOptStat(0);

    TFile *file = new TFile(fileName);
    TTree *tree = (TTree*)file->Get("T");    
    //    tree->Print();

    TH1D *hist1x = new TH1D("hist1x","hist1x",15,-48,48);
    TH1D *hist1y = new TH1D("hist1y","hist1y",15,-48,48);
    TH1D *hist2x = new TH1D("hist2x","hist2x",16,-70,70);
    TH1D *hist2y = new TH1D("hist2y","hist2y",18,-80,80);
    TH1D *histnx = new TH1D("histnx","histnx",15,-48,48);
    TH1D *histny = new TH1D("histny","histny",15,-48,48);
    TH1D *histpx = new TH1D("histpx","histpx",15,-48,48);
    TH1D *histpy = new TH1D("histpy","histpy",15,-48,48);
    TH1D *histn2x = new TH1D("histn2x","histn2x",16,-70,70);
    TH1D *histn2y = new TH1D("histn2y","histn2y",18,-80,80);
    TH1D *histp2x = new TH1D("histp2x","histp2x",16,-70,70);
    TH1D *histp2y = new TH1D("histp2y","histp2y",18,-80,80);
     
    hist1x->SetMarkerColor(kRed);
    hist1x->SetLineColor(kRed);
    hist1y->SetMarkerColor(kRed);
    hist1y->SetLineColor(kRed);
    hist2x->SetMarkerColor(kRed);
    hist2x->SetLineColor(kRed);
    hist2y->SetMarkerColor(kRed);
    hist2y->SetLineColor(kRed);

    //    TH1D *hist2x = new TH1D("tempHistx","tempHistx",13,0.5,13.5);
    //TH1D *hist2y = new TH1D("tempHisty","tempHisty",13,0.5,13.5);
    //TH1D *hist3 = new TH1D("tempHisty","tempHisty",13,0.5,13.5);

    TTreeReader reader("T", file);
    TTreeReaderArray <Double_t> negADCx(reader,"P.hod.1x.negAdcCounter");
    TTreeReaderArray <Double_t> negADCy(reader,"P.hod.1y.negAdcCounter");
    TTreeReaderArray <Double_t> posADCx(reader,"P.hod.1x.posAdcCounter");
    TTreeReaderArray <Double_t> posADCy(reader,"P.hod.1y.posAdcCounter");
    TTreeReaderArray <Double_t> negADC2x(reader,"P.hod.2x.negAdcCounter");
    TTreeReaderArray <Double_t> negADC2y(reader,"P.hod.2y.negAdcCounter");
    TTreeReaderArray <Double_t> posADC2x(reader,"P.hod.2x.posAdcCounter");
    TTreeReaderArray <Double_t> posADC2y(reader,"P.hod.2y.posAdcCounter");

    //tree->Draw("P.hod.1x.negAdcCounter>>tempHistx");
    //hist2 = (TH1D*)gDirectory->Get("tempHistx");
    //Int_t nEntries_nonTracked = hist2->GetEntries();    
    //tree->Draw("P.hod.1y.negAdcCounter>>tempHisty");
    //hist3 = (TH1D*)gDirectory->Get("tempHisty");

    Double_t hodo1x_nt,hodo2x_nt; 
    Double_t hodo1y_nt,hodo2y_nt;
    Double_t hodo1x,hodo2x; 
    Double_t hodo1y,hodo2y;
    // Double_t W;

    tree->SetBranchAddress("P.hod.1x.TrackXPos", &hodo1x);
    tree->SetBranchAddress("P.hod.1x.TrackYPos", &hodo1y);
    tree->SetBranchAddress("P.hod.2x.TrackXPos", &hodo2x);
    tree->SetBranchAddress("P.hod.2x.TrackYPos", &hodo2y);
    //    tree->SetBranchAddress("P.hod.1x.negAdcCounter", &hodo1xcn);
    //tree->SetBranchAddress("P.hod.1x.negAdcCounter", &hodo1ycn);
    //tree->SetBranchAddress("P.hod.1x.negAdcCounter", &hodo1xcp);
    //tree->SetBranchAddress("P.hod.1x.negAdcCounter", &hodo1ycp);

    for (Int_t event = 0; event < tree->GetEntries(); ++ event)
    {
       tree->GetEntry(event);
       hist1x->Fill(hodo1x);
       hist1y->Fill(hodo1y);
       hist2x->Fill(hodo2x);
       hist2y->Fill(hodo2y);
    }
       //    for (Int_t event = 0; event < nEntries_nonTracked; ++ event)
       //    {
       //    tree->GetEntry(event);
       //       hodo1xcn = tree->GetLeaf("P.hod.1x.negAdcCounter")->GetValue();
       //       hodo1ycn = tree->GetLeaf("P.hod.1y.negAdcCounter")->GetValue();
       //       hodo1xcp = tree->GetLeaf("P.hod.1x.posAdcCounter")->GetValue();
       //       hodo1ycp = tree->GetLeaf("P.hod.1y.posAdcCounter")->GetValue();
  
    while (reader.Next()) 
      {
	for(int i = 0; i < negADCx.GetSize(); ++i) 
	  {
            hodo1x_nt = -48.0 +1.5*(96./15.)+ negADCx[i]*(96./15.);
	    histnx->Fill(hodo1x_nt);
          }
	for(int i = 0; i < negADCy.GetSize(); ++i) 
	  {
            hodo1y_nt = -48.0 +0.5*(96./15.)+ negADCy[i]*(96./15.);
	    histny->Fill(hodo1y_nt);
          }

	for(int i = 0; i < posADCx.GetSize(); ++i) 
	  {
            hodo1x_nt = -48.0 +0.5*(96./15.)+ posADCx[i]*(96./15.);
	    histpx->Fill(hodo1x_nt);
          }
	for(int i = 0; i < posADCy.GetSize(); ++i) 
	  {
            hodo1y_nt = -48.0 +0.5*(96./15.)+ posADCy[i]*(96./15.);
	    histpy->Fill(hodo1y_nt);
          }


	for(int i = 0; i < negADC2x.GetSize(); ++i) 
	  {
            hodo2x_nt = -70.0 +0.5*(140./16.)+ negADC2x[i]*(140./16.);
	    histn2x->Fill(hodo2x_nt);
          }
	for(int i = 0; i < negADC2y.GetSize(); ++i) 
	  {
            hodo2y_nt = -80.0 -1.5*(160./18.)+ negADC2y[i]*(160./18.);
	    histn2y->Fill(hodo2y_nt);
          }

	for(int i = 0; i < posADC2x.GetSize(); ++i) 
	  {
            hodo2x_nt = -70.0 +0.5*(140./16.)+ posADC2x[i]*(140./16.);
	    histp2x->Fill(hodo2x_nt);
          }
	for(int i = 0; i < posADC2y.GetSize(); ++i) 
	  {
            hodo2y_nt = -80.0 -1.5*(160./18.)+ posADC2y[i]*(160./18.);
	    histp2y->Fill(hodo2y_nt);
          }

    /*       hodo1y_nt = -42.0 -(84./13./2.)+ hodo1ycn*(84./13.);
       histn->Fill(hodo1x_nt,hodo1y_nt);
       hodo1x_nt = -42.0 -(84./13./2.)+ hodo1xcp*(84./13.);
       hodo1y_nt = -42.0 -(84./13./2.)+ hodo1ycp*(84./13.);
       histp->Fill(hodo1x_nt,hodo1y_nt); */

      }
    TCanvas *c1 = new TCanvas("c1","c1");
    c1->Divide(2,2); 
    c1->cd(1);
    histnx->Draw("prof");    
    histpx->Draw("same");
    hist1x->Draw("same");

    c1->cd(2);
    histny->Draw("prof");    
    histpy->Draw("same");    
    hist1y->Draw("same");    
    
    c1->cd(3);
    histn2x->Draw("prof");    
    histp2x->Draw("same");
    hist2x->Draw("same");

    c1->cd(4);
    histn2y->Draw("prof");    
    histp2y->Draw("same");    
    hist2y->Draw("same");    

    c1->Print("hodo.pdf)","pdf");


    TCanvas *c2 = new TCanvas("c2","c2");
    c2->Divide(2,2); 
    c2->cd(1);
    histnx->DrawNormalized("prof");    
    histpx->DrawNormalized("same");
    hist1x->DrawNormalized("same");

    c2->cd(2);
    histny->DrawNormalized("prof");    
    histpy->DrawNormalized("same");    
    hist1y->DrawNormalized("same");    
    
    c2->cd(3);
    histn2x->DrawNormalized("prof");    
    histp2x->DrawNormalized("same");
    hist2x->DrawNormalized("same");

    c2->cd(4);
    histn2y->DrawNormalized("prof");    
    histp2y->DrawNormalized("same");    
    hist2y->DrawNormalized("same");    

    c2->Print("hodo2.pdf)","pdf");


}
