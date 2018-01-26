// Filename: CTdemoDraw.cc
// Description: Demo
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
#include "CTRun.h"
using namespace std;

void CTdemoDraw(Int_t runNumber)
{
    CTRun *ct = new CTRun(runNumber);
    if(!ct->fRunExist)
	return;
    
    ct->DefinePBetaCut(0.5,1.5);
    ct->DefineHBetaCut(0.5,1.5);
    ct->DefinePCerCut(-1,0.1);
    ct->DefineHCerCut(0.1);
    ct->DefineHCalCut(0.8,1.5);
    ct->DefineHPreShCut(0.2);

    ct->ApplyCut();
    
    TTree * tree = ct->GetTree();

    TCanvas *c1 = new TCanvas("c1","H.gtr.beta"); 
    tree->Draw("H.gtr.beta");
    TCanvas *c2 = new TCanvas("c2","H.kin.primary.W"); 
    tree->Draw("H.kin.primary.W");    
    TCanvas *c3 = new TCanvas("c3","H.kin.primary.Q2"); 
    tree->Draw("H.kin.primary.Q2");    
    TCanvas *c4 = new TCanvas("c4","P.gtr.beta"); 
    tree->Draw("P.gtr.beta");    
    TCanvas *c5 = new TCanvas("c5","P.kin.secondary.emiss"); 
    tree->Draw("P.kin.secondary.emiss");
    TCanvas *c6 = new TCanvas("c6","P.kin.secondary.pmiss"); 
    tree->Draw("P.kin.secondary.pmiss");
}
