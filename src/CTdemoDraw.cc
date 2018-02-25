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
    
    ct->DefinePBetaCut(0,2);
    ct->DefineHBetaCut(0,2);
    ct->DefinePCerCut(-1,10);
    ct->DefineHCerCut(0,10);
    ct->DefineHCalCut(0,2);
    ct->DefineHPreShCut(0);

    ct->ApplyCut();
    
    TTree * tree = ct->GetTree();

    TCanvas *c1 = new TCanvas("c1","H.cal.eprtracknorm"); 
    tree->Draw("H.cal.eprtracknorm");
    TCanvas *c2 = new TCanvas("c2","H.cal.etottracknorm"); 
    tree->Draw("H.cal.etottracknorm");    
    TCanvas *c3 = new TCanvas("c3","H.cer.npeSum"); 
    tree->Draw("H.cer.npeSum");    
    TCanvas *c4 = new TCanvas("c4","P.hgcer.npeSum"); 
    tree->Draw("P.hgcer.npeSum");    
    TCanvas *c5 = new TCanvas("c5","P.cal.eprtracknorm"); 
    tree->Draw("P.cal.eprtracknorm");
    TCanvas *c6 = new TCanvas("c6","P.cal.etotttracknorm"); 
    tree->Draw("P.cal.etottracknorm");
}
