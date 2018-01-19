// Filename: CTRun.h
// Description: 
// Author: Latif Kabir < latif@jlab.org >
// Created: Tue Oct 17 18:10:57 2017 (-0400)
// URL: jlab.org/~latif

#ifndef CTRUN_H
#define CTRUN_H

#include <iostream>
#include "TObject.h"
#include "TFile.h"
#include "TTree.h"
#include "TChain.h"
#include "TString.h"
#include "TH1F.h"

class CTRun: public TObject
{
    TString fFileName;   
    Int_t fRunNumber;
    Int_t fLastRun;
    Int_t fFirstRun;
    void Init();
    void SetBranchAddressCT();
    TFile *fFile;
    TChain *fChain;
    Int_t fRunCounter;
public:

    //------------- All Branch/Leaf variables ---------------------------
    //Golden Track Quantities
    Double_t fP_gtr_p;
    Double_t fP_gtr_beta;    
    Double_t fH_gtr_beta;        
    Double_t fH_gtr_p;
    
    //Calorimeter
    Double_t fP_cal_eprtracknorm;
    Double_t fP_cal_rtottracknorm;
    Double_t fP_hgcer_npeSum;

    Double_t fH_cal_eprtracknorm;
    Double_t fH_cal_etottracknorm;
    Double_t fH_cer_npeSum;

    //Hodoscope start time 
    Double_t HhodStartTime;
    Double_t PhodStartTime;
    Double_t PhodStatus;
    Double_t HhodStatus;
    
    Double_t PhodfpHitsTime;
    Double_t HhodfpHitsTime;

    //Focal Plane coord
    Double_t HgtrX;
    Double_t HgtrTh;
    Double_t HgtrY;
    Double_t HgtrPh;
        
    //Trigger 
    Double_t TcoinpTRIG1_ROC1_tdcTimeRaw;
    Double_t TcoinpTRIG4_ROC1_tdcTimeRaw;
    Double_t TcoinpTRIG1_ROC2_tdcTimeRaw;
    Double_t TcoinpTRIG4_ROC2_tdcTimeRaw;
    
    Double_t TcoinhTRIG1_ROC1_tdcTimeRaw;
    Double_t TcoinhTRIG4_ROC1_tdcTimeRaw;
    Double_t TcoinhTRIG1_ROC2_tdcTimeRaw;
    Double_t TcoinhTRIG4_ROC2_tdcTimeRaw;
        
    //Int_t EvtType;
    Double_t EvtType;  // GetLeaf()->GetValue() returns double only


    
    
    CTRun(Int_t runNumber);
    CTRun();
    CTRun(Int_t firstRun, Int_t lastRun);
    ~CTRun();

    Bool_t fRunExist;
    
    void Print();
    TTree* GetTree();
    TChain* GetChain();
    Int_t GetRunNumber();
    
    ClassDef(CTRun,0)
};

#endif
