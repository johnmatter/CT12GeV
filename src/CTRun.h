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

    //==========================================================================
    //                      All branch/Leaf Variables
    //==========================================================================
    //------------ Kinematics ----------------------------
    //SHMS
    Double_t fP_kin_Primary_W;
    Double_t fP_kin_Primary_Q2;
    Double_t fP_kin_Primary_nu;

    //HMS
    Double_t fH_kin_secondary_pmiss;
    Double_t fH_kin_secondary_pmiss_x;
    Double_t fH_kin_secondary_emiss;
    
    //------------- Golden Track Quantities -----------------
    //SHMS
    Double_t fP_gtr_p;
    Double_t fP_gtr_dp;
    Double_t fP_gtr_beta;
    //HMS
    Double_t fH_gtr_p;
    Double_t fH_gtr_dp;
    Double_t fH_gtr_beta;

    // --------- Drift Chamber ----------------
    //SHMS
    Double_t fP_dc_x_fp;
    Double_t fP_dc_xp_fp;
    Double_t fP_dc_y_fp;
    Double_t fP_dc_yp_fp;

    //HMS
    Double_t fH_dc_x_fp;
    Double_t fH_dc_xp_fp;
    Double_t fH_dc_y_fp;
    Double_t fH_dc_yp_fp;

    //------------ Cerenkov Counter ----------
    //SHMS
    Double_t fP_hgcer_npeSum;
    
    //HMS
    Double_t fH_cer_npeSum;
    
    //------------ Hodoscope ---------------
    //SHMS
    Double_t fP_hod_goodstarttime;
    Double_t fP_hod_starttime;
    Double_t fP_hod_fpHitsTime;

    //HMS
    Double_t fH_hod_goodstarttime;    
    Double_t fH_hod_starttime;
    Double_t fH_hod_fpHitsTime;

    //--------- Calorimeter ---------------
    //SHMS
    Double_t fP_cal_eprtracknorm;
    Double_t fP_cal_etottracknorm;

    //HMS
    Double_t fH_cal_eprtracknorm;
    Double_t fH_cal_etottracknorm;
        
    //--------- Trigger ----------------
    //SHMS
    Double_t fT_coin_pTRIG1_ROC1_tdcTimeRaw;
    Double_t fT_coin_pTRIG4_ROC1_tdcTimeRaw;
    Double_t fT_coin_pTRIG1_ROC2_tdcTimeRaw;
    Double_t fT_coin_pTRIG4_ROC2_tdcTimeRaw;

    //HMS
    Double_t fT_coin_hTRIG1_ROC1_tdcTimeRaw;
    Double_t fT_coin_hTRIG4_ROC1_tdcTimeRaw;
    Double_t fT_coin_hTRIG1_ROC2_tdcTimeRaw;
    Double_t fT_coin_hTRIG4_ROC2_tdcTimeRaw;

    //----------- Event Type ---------------
    //Int_t EvtType;
    Double_t EvtType;  // GetLeaf()->GetValue() returns double only
    //==========================================================================


    
    
    CTRun(Int_t runNumber);
    CTRun();
    CTRun(Int_t firstRun, Int_t lastRun);
    ~CTRun();

    Bool_t fRunExist;
    
    void Print();
    TTree* GetTree();
    TChain* GetChain();
    Int_t GetRunNumber();
    void ActivateCTBranches();
    
    ClassDef(CTRun,0)
};

#endif
