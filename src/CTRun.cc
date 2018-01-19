// Filename: CTRun.cc
// Description: Main CTRun class. 
// Author: Latiful Kabir < latif@jlab.org >
// Created: Fri Apr 10 14:50:00 2017 (-0400)
// URL: jlab.org/~latif

#include "TFile.h"
#include "TTree.h"
#include "TString.h"
#include "TH1F.h"
#include "TSystem.h"
#include "Constants.h"
#include "CTRun.h"

using namespace std;

ClassImp(CTRun)

CTRun::CTRun(int runNumber)
{
    fRunNumber = runNumber;
    fFirstRun = runNumber;
    fLastRun = runNumber;
    fRunCounter = 0;
    Init();
}


CTRun::CTRun(int firstRun, int lastRun)
{
    fRunNumber = -1;
    fFirstRun = firstRun;
    fLastRun = lastRun;
    fRunCounter = 0;
    Init();
}


CTRun::CTRun()
{
    
}
CTRun::~CTRun()
{
    fFile->Close();
}

void CTRun::Init()
{
    fChain = new TChain("ct_chain","CT Chain");

    Bool_t runNotFound = kTRUE;
    fRunExist = kFALSE;
    for(Int_t run = fFirstRun; run <= fLastRun; ++run)
    {
	fFileName = ROOT_FILE_PATH;
	fFileName += run;
	fFileName +=  ".root";

	runNotFound = gSystem->AccessPathName(fFileName);
	if(runNotFound)
	{	    
	    cout << "Requested run " << run << " does NOT exist" <<endl;
	    continue;
	}
	fChain->Add(fFileName);
	++fRunCounter;
	cout << "Adding the run number:"<< run <<endl;
    }
    if(fRunCounter < 1)
	return;

    SetBranchAddressCT();
    fRunExist = kTRUE;
}

void CTRun::Print()
{
    if(!fRunExist)
	return;
    fChain->Print();
}

TChain* CTRun::GetChain()
{
    if(!fRunExist)
	return NULL;
    return fChain;
}


TTree* CTRun::GetTree()
{
    if(!fRunExist)
	return NULL;
    return (TTree*)fChain;
}


Int_t CTRun::GetRunNumber()
{
    return fRunNumber;
}

// For the buffer fCTEventData
void CTRun::SetBranchAddressCT()
{
   // Set branch addresses.
   fChain->SetBranchAddress("P.gtr.p", &fP_gtr_p);
   fChain->SetBranchAddress("H.gtr.p", &fH_gtr_p);
   fChain->SetBranchAddress("P.gtr.beta", &fP_gtr_beta);
   fChain->SetBranchAddress("P.hgcer.npeSum", &fP_hgcer_npeSum);
   fChain->SetBranchAddress("H.cer.npeSum", &fH_cer_npeSum);
}
