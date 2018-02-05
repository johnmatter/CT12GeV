// Filename: CTRun.cc
// Description: Main CTRun class for the CT experiment. Assumes SHMS as p arm and HMS as e arm.
// Author: Latiful Kabir < latif@jlab.org >
// Created: Fri Apr 10 14:50:00 2017 (-0400)
// URL: jlab.org/~latif

#include "TFile.h"
#include "TTree.h"
#include "TString.h"
#include "TH1F.h"
#include "TSystem.h"
#include "TCut.h"
#include "TEventList.h"
#include "Constants.h"
#include "CTRun.h"

using namespace std;

ClassImp(CTRun)

CTRun::CTRun(int runNumber)
{
    fRunNumber = runNumber;
    fFirstRun = runNumber;
    fLastRun = runNumber;
    Init();
}


CTRun::CTRun(int firstRun, int lastRun)
{
    fRunNumber = -1;
    fFirstRun = firstRun;
    fLastRun = lastRun;
    Init();
}

CTRun::CTRun(TString fileName)
{
    fRunNumber = -1;
    Init(fileName);
}

CTRun::CTRun()
{
    
}
CTRun::~CTRun()
{
    //fFile->Close();
}

void CTRun::Init()
{
    fRunCounter = 0;
    fSimcFileCounter = 0;
    fSimcExist = kFALSE;
    fRunExist = kFALSE;

    //---Cut values ----
    //beta cut
    fPBetaMin = -1;
    fPBetaMax = -1;
    fHBetaMin = -1;
    fHBetaMax = -1;

    //Cerenkov Counter cut
    fPCerMin = -1;
    fPCerMax = -1;
    fHCerMin = -1;
    fHCerMax = -1;

    // Calorimeter Cut
    fPCalMin = -1;
    fPCalMax = -1;
    fHCalMin = -1;
    fHCalMax = -1;

    //Pre-shower counter
    fPPreShMin = -1;
    fPPreShMax = -1;
    fHPreShMin = -1;
    fHPreShMax = -1;

    fChain = new TChain("ct_chain","CT Chain");

    Bool_t runNotFound = kTRUE;
    for(Int_t run = fFirstRun; run <= fLastRun; ++run)
    {
	fFileName = ROOT_FILE_PATH;
	fFileName += FILE_PREFIX;
	fFileName += run;
	fFileName += FILE_SUFFIX;
	fFileName +=  ".root";

	runNotFound = gSystem->AccessPathName(fFileName);
	if(runNotFound)
	{	    
	    cout << "Requested run " << run << " does NOT exist" <<endl;
	    cout << "File name with complete path: "<<fFileName<<endl;
	    continue;
	}
	fFileName +=  "/T";	
	fChain->Add(fFileName);
	++fRunCounter;
	cout << "Adding the run number:"<< run <<endl;
    }
    if(fRunCounter < 1)
	return;

    fRunExist = kTRUE;
    SetBranchAddressCT();
}


void CTRun::Init(TString file_name)
{
    fRunCounter = 0;
    fSimcFileCounter = 0;
    fSimcExist = kFALSE;
    fRunExist = kFALSE;

    //---Cut values ----
    //beta cut
    fPBetaMin = -1;
    fPBetaMax = -1;
    fHBetaMin = -1;
    fHBetaMax = -1;

    //Cerenkov Counter cut
    fPCerMin = -1;
    fPCerMax = -1;
    fHCerMin = -1;
    fHCerMax = -1;

    // Calorimeter Cut
    fPCalMin = -1;
    fPCalMax = -1;
    fHCalMin = -1;
    fHCalMax = -1;

    //Pre-shower counter
    fPPreShMin = -1;
    fPPreShMax = -1;
    fHPreShMin = -1;
    fHPreShMax = -1;
    
    fChain = new TChain("ct_chain","CT Chain");

    Bool_t runNotFound = kTRUE;    
    fFileName = file_name;

    runNotFound = gSystem->AccessPathName(fFileName);
    if(runNotFound)
    {	    
	cout << "Requested file: " << fFileName << " does NOT exist" <<endl;
	return;	
    }
    fFileName +=  "/T";	
    fChain->Add(fFileName);
    ++fRunCounter;
    cout << "Adding the file:"<< fFileName <<endl;
    if(fRunCounter < 1)
	return;

    fRunExist = kTRUE;
    SetBranchAddressCT();
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

void CTRun::AddSimc(TString SimcFileName, Int_t makeFriend)
{
    if(!fRunExist)
	return;
    
    fSimcChain = new TChain("simc_chain","Simc Chain");
    Bool_t runNotFound = kTRUE;    
    fSimcFileName = SimcFileName;

    fSimcFileName = ROOT_FILE_PATH;
    fSimcFileName += SimcFileName;
    runNotFound = gSystem->AccessPathName(fSimcFileName);
    if(runNotFound)
    {	    
	cout << "Requested file: " << fSimcFileName << " does NOT exist" <<endl;
	return;	
    }
    fSimcFileName += "/h666"; 
    fSimcChain->Add(fSimcFileName);
    ++fSimcFileCounter;

    if(fSimcFileCounter == 1)
    {
	fSimcExist = kTRUE;
	SetBranchAddressSimc();
	if(fRunExist && makeFriend != -1)
	    fChain->AddFriend(fSimcChain);
    }
}

TChain* CTRun::GetSimcChain()
{
    if(!fRunExist)
	return NULL;
    return fSimcChain;
}


TTree* CTRun::GetSimcTree()
{
    if(!fRunExist)
	return NULL;
    return (TTree*)fSimcChain;
}

void CTRun::DefinePBetaCut(Double_t min, Double_t max)
{
    fPBetaMin = min;
    fPBetaMax = max;
}

void CTRun::DefineHBetaCut(Double_t min, Double_t max)
{
    fHBetaMin = min;
    fHBetaMax = max;
}

void CTRun::DefinePCerCut(Double_t min, Double_t max)
{
    fPCerMax = max;
    fPCerMin = min;
}

void CTRun::DefineHCerCut(Double_t min, Double_t max)
{
    fHCerMax = max;
    fHCerMin = min;
}

void CTRun::DefinePCalCut(Double_t min, Double_t max)
{
    fPCalMax = max;
    fPCalMin = min;
}

void CTRun::DefineHCalCut(Double_t min, Double_t max)
{
    fHCalMax = max;
    fHCalMin = min;
}

void CTRun::DefinePPreShCut(Double_t min, Double_t max)
{
    fPPreShMax = max;
    fPPreShMin = min;
}

void CTRun::DefineHPreShCut(Double_t min, Double_t max)
{
    fHPreShMax = max;
    fHPreShMin = min;
}


void CTRun::ApplyCut()
{
    if(!fRunExist)
	return;

    fChain->SetEventList(0); // Unset previously set event list
    fCTcut.Clear();    
    
    fCTcut = " 1 ";

    //--------- Beta Cut -------------------
    if(fPBetaMin != -1)
    {
	fCTcut += " && P.gtr.beta >=";
	fCTcut += fPBetaMin;
    }
    if(fPBetaMax != -1)
    {
	fCTcut += " && P.gtr.beta <=";
	fCTcut += fPBetaMax;
    }
    if(fHBetaMin != -1)
    {
	fCTcut += " && H.gtr.beta >=";
	fCTcut += fHBetaMin;
    }
    if(fHBetaMax != -1)
    {
	fCTcut += " && H.gtr.beta <=";
	fCTcut += fHBetaMax;
    }

    //--------- Cerenkov Counter Cut -----------
    if(fPCerMin != -1)
    {
	fCTcut += "&& P.hgcer.npeSum >=";
	fCTcut += fPCerMin;
    }
    if(fPCerMax != -1)
    {
	fCTcut += " && P.hgcer.npeSum <=";
	fCTcut += fPCerMax;
    }
    if(fHCerMin != -1)
    {
	fCTcut += " && H.cer.npeSum >=";
	fCTcut += fHCerMin;
    }
    if(fHCerMax != -1)
    {
	fCTcut += " && H.cer.npeSum <=";
	fCTcut += fHCerMax;
    }

    //--------- Calorimeter Cut -----------
    if(fPCalMin != -1)
    {
	fCTcut += "&& P.cal.etottracknorm >=";
	fCTcut += fPCalMin;
    }
    if(fPCalMax != -1)
    {
	fCTcut += " && P.cal.etottracknorm <=";
	fCTcut += fPCalMax;
    }
    if(fHCalMin != -1)
    {
	fCTcut += " && H.cal.etottracknorm >=";
	fCTcut += fHCalMin;
    }
    if(fHCalMax != -1)
    {
	fCTcut += " && H.cal.etottracknorm <=";
	fCTcut += fHCalMax;
    }

    //--------- Preshower Cut -----------
    if(fPPreShMin != -1)
    {
	fCTcut += "&& P.cal.eprtracknorm >=";
	fCTcut += fPPreShMin;
    }
    if(fPPreShMax != -1)
    {
	fCTcut += " && P.cal.eprtracknorm <=";
	fCTcut += fPPreShMax;
    }
    if(fHPreShMin != -1)
    {
	fCTcut += " && H.cal.eprtracknorm >=";
	fCTcut += fHPreShMin;
    }
    if(fHPreShMax != -1)
    {
	fCTcut += " && H.cal.eprtracknorm <=";
	fCTcut += fHPreShMax;
    }

    cout << "Applied Cut: "<< fCTcut<<endl;
    
    fChain->Draw(">>list_temp1",fCTcut,"eventlist");
    fCTEvents = (TEventList*)gDirectory->Get("list_temp1");
    
    fChain->SetEventList(fCTEvents);
}

void CTRun::SetDefaultCut()
{
    DefinePBetaCut(0.5,1.5);
    DefineHBetaCut(0.5,1.5);
    DefinePCerCut(-1,0.1);
    DefineHCerCut(0.1);
    DefineHCalCut(0.8,1.5);
    DefineHPreShCut(0.2);

    ApplyCut();
}

void CTRun::PrintCut()
{
    cout<<fCTcut<<endl;
}
void CTRun::ClearCut()
{
    if(!fRunExist)
	return;

    fChain->SetEventList(0); // Unset previously set event list
    fCTcut.Clear();
    
    //---Cut values ----
    //beta cut
    fPBetaMin = -1;
    fPBetaMax = -1;
    fHBetaMin = -1;
    fHBetaMax = -1;

    //Cerenkov Counter cut
    fPCerMin = -1;
    fPCerMax = -1;
    fHCerMin = -1;
    fHCerMax = -1;

    // Calorimeter Cut
    fPCalMin = -1;
    fPCalMax = -1;
    fHCalMin = -1;
    fHCalMax = -1;

    //Pre-shower counter
    fPPreShMin = -1;
    fPPreShMax = -1;
    fHPreShMin = -1;
    fHPreShMax = -1;

}
void CTRun::ActivateCTBranches()
{
    if(!fRunExist)
	return;

    fChain->SetBranchStatus("*",0);
    fChain->SetBranchStatus("P.kin.primary.*",1);
    fChain->SetBranchStatus("H.kin.secondary.*",1);
    fChain->SetBranchStatus("P.gtr.*",1);
    fChain->SetBranchStatus("H.gtr.*",1);

    fChain->SetBranchStatus("P.dc.x*",1);
    fChain->SetBranchStatus("P.dc.y*",1);

    fChain->SetBranchStatus("H.dc.x*",1);
    fChain->SetBranchStatus("H.dc.y*",1);

    fChain->SetBranchStatus("P.hgcer.*",1);
    fChain->SetBranchStatus("H.cer.*",1);

    fChain->SetBranchStatus("P.hod.*",1);
    fChain->SetBranchStatus("H.hod.*",1);

    fChain->SetBranchStatus("P.cal.*",1);
    fChain->SetBranchStatus("H.cal.*",1);

    fChain->SetBranchStatus("T.coin.pTRIG*",1);
    fChain->SetBranchStatus("T.coin.hTRIG*",1);
}


void CTRun::SetBranchAddressSimc()
{
    //HMS
    fSimcChain->SetBranchAddress("hsdelta", &fHSdelta);
    fSimcChain->SetBranchAddress("hsxptar", &fHSxptar);
    fSimcChain->SetBranchAddress("hsyptar", &fHSyptar);
    fSimcChain->SetBranchAddress("hsytar", &fHSytar);

    //SHMS
    fSimcChain->SetBranchAddress("ssdelta", &fPSdelta);
    fSimcChain->SetBranchAddress("ssxptar", &fPSxptar);
    fSimcChain->SetBranchAddress("ssyptar", &fPSyptar);
    fSimcChain->SetBranchAddress("ssytar", &fPSytar);

    fSimcChain->SetBranchAddress("Weight", &fWeight);
    fSimcChain->SetBranchAddress("W", &fW);
    fSimcChain->SetBranchAddress("Em", &fEm);
    fSimcChain->SetBranchAddress("Pm", &fPm);
    fSimcChain->SetBranchAddress("Pmx", &fPmx);
    fSimcChain->SetBranchAddress("Pmy", &fPmy);
    fSimcChain->SetBranchAddress("Pmz", &fPmz);    
}

// For the buffer fCTEventData
void CTRun::SetBranchAddressCT()
{
    if(!fRunExist)
	return;

   //=============================== Set branch addresses.==================
    //------------ Kinematics ----------------------------
    //SHMS
    fChain->SetBranchAddress("P.kin.secondary.pmiss", &fP_kin_secondary_pmiss);
    fChain->SetBranchAddress("P.kin.secondary.pmiss_x", &fP_kin_secondary_pmiss_x);
    fChain->SetBranchAddress("P.kin.secondary.pmiss_y", &fP_kin_secondary_pmiss_y);
    fChain->SetBranchAddress("P.kin.secondary.pmiss_z", &fP_kin_secondary_pmiss_z);
    fChain->SetBranchAddress("P.kin.secondary.emiss", &fP_kin_secondary_emiss);
    fChain->SetBranchAddress("P.kin.secondary.emiss_nuc", &fP_kin_secondary_emiss_nuc);

    //HMS
    fChain->SetBranchAddress("H.kin.primary.W", &fH_kin_primary_W);
    fChain->SetBranchAddress("H.kin.primary.Q2", &fH_kin_primary_Q2);
    fChain->SetBranchAddress("H.kin.primary.nu", &fH_kin_primary_nu);


    //------------ Golden Track Quantities ----------------
    //SHMS 
    fChain->SetBranchAddress("P.gtr.p", &fP_gtr_p);
    fChain->SetBranchAddress("P.gtr.dp", &fP_gtr_dp);
    fChain->SetBranchAddress("P.gtr.beta", &fP_gtr_beta);            
    
    //HMS
    fChain->SetBranchAddress("H.gtr.p", &fH_gtr_p);
    fChain->SetBranchAddress("H.gtr.dp", &fH_gtr_dp);
    fChain->SetBranchAddress("H.gtr.beta", &fH_gtr_beta);

    //--------- Vertex/Target Coord --------------  
    //SHMS
    fChain->SetBranchAddress("P.gtr.x",  &fP_gtr_x);
    fChain->SetBranchAddress("P.gtr.th", &fP_gtr_th);    
    fChain->SetBranchAddress("P.gtr.y",  &fP_gtr_y);
    fChain->SetBranchAddress("P.gtr.ph", &fP_gtr_ph);

     //HMS
    fChain->SetBranchAddress("H.gtr.x", &fH_gtr_x);
    fChain->SetBranchAddress("H.gtr.th", &fH_gtr_th);    
    fChain->SetBranchAddress("H.gtr.y", &fH_gtr_y);
    fChain->SetBranchAddress("H.gtr.ph", &fH_gtr_ph);

    
    // ---------------- Drift Chamber ---------------
    //SHMS
    fChain->SetBranchAddress("P.dc.x_fp", &fP_dc_x_fp);
    fChain->SetBranchAddress("P.dc.xp_fp", &fP_dc_xp_fp);
    fChain->SetBranchAddress("P.dc.y_fp", &fP_dc_y_fp);
    fChain->SetBranchAddress("P.dc.yp_fp", &fP_dc_yp_fp);    

    //HMS
    fChain->SetBranchAddress("H.dc.x_fp", &fH_dc_x_fp);
    fChain->SetBranchAddress("H.dc.xp_fp", &fH_dc_xp_fp);
    fChain->SetBranchAddress("H.dc.y_fp", &fH_dc_y_fp);
    fChain->SetBranchAddress("H.dc.yp_fp", &fH_dc_yp_fp);    

    //------------ Cerenkov Counter ----------    
    fChain->SetBranchAddress("P.hgcer.npeSum", &fP_hgcer_npeSum);
    fChain->SetBranchAddress("H.cer.npeSum", &fH_cer_npeSum);

    //------------ Hodoscope ----------
    //SHMS
    fChain->SetBranchAddress("P.hod.goodstarttime", &fP_hod_goodstarttime);
    fChain->SetBranchAddress("P.hod.starttime", &fP_hod_starttime);
    fChain->SetBranchAddress("P.hod.fpHitsTime", &fP_hod_fpHitsTime);

    //HMS
    fChain->SetBranchAddress("H.hod.goodstarttime", &fH_hod_goodstarttime);  
    fChain->SetBranchAddress("H.hod.starttime", &fH_hod_starttime);    
    fChain->SetBranchAddress("H.hod.fpHitsTime", &fH_hod_fpHitsTime);

    //------------ Calorimeter ----------    
    //SHMS
    fChain->SetBranchAddress("P.cal.eprtracknorm", &fP_cal_eprtracknorm);
    fChain->SetBranchAddress("P.cal.etottracknorm", &fP_cal_etottracknorm);
    
    //HMS
    fChain->SetBranchAddress("H.cal.eprtracknorm", &fH_cal_eprtracknorm);
    fChain->SetBranchAddress("H.cal.etottracknorm", &fH_cal_etottracknorm);

    
    //--------- Trigger ----------------
    //SHMS
    fChain->SetBranchAddress("T.coin.pTRIG1_ROC1_tdcTimeRaw", &fT_coin_pTRIG1_ROC1_tdcTimeRaw);
    fChain->SetBranchAddress("T.coin.pTRIG4_ROC1_tdcTimeRaw", &fT_coin_pTRIG4_ROC1_tdcTimeRaw);
    fChain->SetBranchAddress("T.coin.pTRIG1_ROC2_tdcTimeRaw", &fT_coin_pTRIG1_ROC2_tdcTimeRaw);
    fChain->SetBranchAddress("T.coin.pTRIG4_ROC2_tdcTimeRaw", &fT_coin_pTRIG4_ROC2_tdcTimeRaw);

    //HMS
    fChain->SetBranchAddress("T.coin.hTRIG1_ROC1_tdcTimeRaw", &fT_coin_hTRIG1_ROC1_tdcTimeRaw);
    fChain->SetBranchAddress("T.coin.hTRIG4_ROC1_tdcTimeRaw", &fT_coin_hTRIG4_ROC1_tdcTimeRaw);
    fChain->SetBranchAddress("T.coin.hTRIG1_ROC2_tdcTimeRaw", &fT_coin_hTRIG1_ROC2_tdcTimeRaw);
    fChain->SetBranchAddress("T.coin.hTRIG4_ROC2_tdcTimeRaw", &fT_coin_hTRIG4_ROC2_tdcTimeRaw);

    //fChain->SetBranchAddress("fEvtHdr.fEvtType", &EvtData);  // This branch has some problem at this moment, SetBranchAddress() does not work. It's a nested branch.
   
}
