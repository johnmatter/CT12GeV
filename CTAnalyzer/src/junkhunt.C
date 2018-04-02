// Filename: CTkinDist.cc
// Description: Look at some Kinematc distributions (without cut) by looping over each event. The main purpose is to show how to loop over events.
// Author: Latif Kabir < latif@jlab.org >
// Created: Thu Jan  4 17:03:26 2018 (-0500)
// URL: jlab.org/~latif

#include <iostream>
#include <TFile.h>
#include <TTree.h>
#include <TString.h>
#include <TH1D.h>
#include <TCanvas.h>
#include <TSystem.h>
#include "Constants.h"

using namespace std;

void junkhunt()
{
    TString fileName = ROOT_FILE_PATH;
    fileName += "shms_replay_production_1791_50000.root";
    // fileName += "coin_replay_production_1711_evt7.root";

    Bool_t runNotFound = gSystem->AccessPathName(fileName);
    if(runNotFound)
    {	    
	cout << "Requested file " << fileName << " does NOT exist" <<endl;
	return;
    }
    
    TFile *file = new TFile(fileName);
    TH2F *h0 = (TH2F*)file->Get("pdc_xfp_vs_yfp");
    //    h0->Rebin2D(8,8);
    TH2F *h1 = (TH2F*)file->Get("phodo_1x_vs_1y_neg_adc_counter");
    TH2F *h2 = (TH2F*)file->Get("phodo_1x_vs_1y_pos_adc_counter");
    TH2F *h3 = (TH2F*)file->Get("phodo_2x_vs_2y_neg_adc_counter");
    TH2F *h4 = (TH2F*)file->Get("phodo_2x_vs_2y_pos_adc_counter");
    
    h1->Add(h2);
    h3->Add(h4);
    h1->Scale(0.5);
    h3->Scale(0.5);
    //    TH2F *h5 = (TH2F*)h1->Clone();
    //TH2F *h6 = (TH2F*)h3->Clone();

    //    h1->Clone("h5");
    //    h3->Clone("h6");
    //    h5->Add(h0,-1.0);
    //    h6->Add(h0,-1.0);
    
    TCanvas *c1 = new TCanvas("c1","c1");
    c1->Divide(2,2); 
    c1->cd(1);
    h0->Draw("COLZ");
    c1->cd(2);
    h1->Draw("COLZ");
    c1->cd(3);
    h3->Draw("COLZ");
    //    c1->cd(4);
    //h5->Draw("COLZ");
    //c1->cd(5);
    //h6->Draw("COLZ");

}   
