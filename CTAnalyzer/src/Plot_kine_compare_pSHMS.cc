// Description: W, Em, Pm and Pm_per distributions for SHMS and HMS to compare between data and SIMC (except for Pm_per).
// This is for the case when the proton is in the SHMS. 
// Needs momentum and angle settings for the particular run being compared.
// These are declared as constants at the top of the code. Also needs root file name.
// and the factor (sfac) to take into account the amount charge collected in data and 
// the various efficiencied, relative to 1 mC and 100% efficiency in SIMC.
// Also needs normfac from SIMC and the simc root file name

#include <iostream>
#include <TFile.h>
#include <TTree.h>
#include <TLeaf.h>
#include <TString.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TCanvas.h>
#include <math.h>
#include <cmath>
#include <TStyle.h>
#include <TROOT.h>
#include "Constants.h"

using namespace std;

const double Ebeam = 2.214; // beam energy is GeV
const double Mp = 0.93827; // proton mass in GeV
const double Me = 0.000511; // electron mass in GeV
const double normfac = 0.149363E+08; //SIMC normfac
const double sfacp = 1.1;//scale factor for the charge, effs of data: SIMC = 1mC and 100% eff.
const double theta_ee = 65.1*3.141592/180.; //SHMS angle
const double theta_pp = -25.0*3.141592/180.; //HMS angle
const double ep_cen = 0.936; //SHMS central momentum
const double pp_cen = 2.012; //HMS central momentum  
const char* filepD = "ep_1722.root"; // Data filename
const char* filepM = "h1_2.root"; // SIMC file name
const char* filepO = "run_1722_kine.pdf"; //output pdf file name


void Plot_kine_compare_pSHMS(){

 //read the input file from data
  TString fileNameD = ROOT_FILE_PATH;
  fileNameD += filepD; //read the root file from data
  TFile *f1 = new TFile(fileNameD);
  TTree *tt = (TTree*)f1->Get("T");
  //get the relevant branch
  int nentriesD = tt->GetEntries();
  cout<<"Entries:\t"<<nentriesD<<endl;

  TString fileNameM = ROOT_FILE_PATH;
  fileNameM += filepM; //read the root file from SIMC
  TFile *f2 = new TFile(fileNameM);
  TTree *ts = (TTree*)f2->Get("h666");
  //get the relevant branch
  int nentriesM = ts->GetEntries();
  cout<<"Simc Entries:\t"<<nentriesM<<endl;

  TFile *fout = new TFile("run_kine_hists.root","RECREATE");
  //  gROOT->SetBatch(kTRUE);
 

  TCanvas *cc = new TCanvas ("cc","cc",800,600);
  cc->SetGrid();
  cc->SetLeftMargin(0.2);
  cc->SetRightMargin(0.2);
  cc->Divide(2,2);
  gStyle->SetOptStat(0);
  
  //make histograms:
  TH1F *h_Emd = new TH1F("h_Emd","Missing Energy (GeV)",200,-0.1,0.1);
  TH1F *h_Ems = new TH1F("h_Ems","Missing Energy (GeV)",200,-0.1,0.1);
  TH1F *h_Wd = new TH1F("h_Wd","W (GeV)", 150, 0.4, 1.6);
  TH1F *h_pmd = new TH1F("h_pmd","Pm (GeV/c)", 100, -0.15, 0.15);
  TH1F *h_perd = new TH1F("h_perd","Pm_per (GeV/c)",100, -0.1, 0.1);  
  TH1F *h_Ws = new TH1F("h_Ws","W (GeV)", 150, 0.4, 1.6);
  TH1F *h_pms = new TH1F("h_pms","Pm (GeV/c)", 100, -0.15, 0.15);
  TH1F *h_pers = new TH1F("h_pers","Pm_per (GeV/c)",100, -0.1, 0.1);  
    


  for (int kk=0; kk<nentriesD; kk++){
    tt->GetEntry(kk);
    if (kk % 50000 == 0) cout << kk*100/nentriesD << "   % of data done" << endl;
    //    TLeaf *hcaleprLd = tt->GetLeaf("H.cal.eprtracknorm");
    //TLeaf *hcaletotLd = tt->GetLeaf("H.cal.etottracknorm");
    Double_t evtType = tt->GetLeaf("fEvtHdr.fEvtType")->GetValue(); 
    double hcalepr = tt->GetLeaf("H.cal.eprtracknorm")->GetValue();
    double hcaletot = tt->GetLeaf("H.cal.etottracknorm")->GetValue();
    double hcernpe = tt->GetLeaf("H.cer.npeSum")->GetValue();
    double hbeta = tt->GetLeaf("H.gtr.beta")->GetValue();
    double hep = tt->GetLeaf("H.gtr.p")->GetValue();
    double deltae = tt->GetLeaf("H.gtr.dp")->GetValue();
    double ekinPm = tt->GetLeaf("H.kin.secondary.pmiss")->GetValue();
    double exptar = tt->GetLeaf("H.gtr.ph")->GetValue();
    double eyptar = tt->GetLeaf("H.gtr.th")->GetValue();


    double pcernpe = tt->GetLeaf("P.hgcer.npeSum")->GetValue();
    double pbeta = tt->GetLeaf("P.gtr.beta")->GetValue();
    double ppp = tt->GetLeaf("P.gtr.p")->GetValue();
    double deltap = tt->GetLeaf("P.gtr.dp")->GetValue();
    double pkinW = tt->GetLeaf("P.kin.primary.W")->GetValue();
    double pxptar = tt->GetLeaf("P.gtr.ph")->GetValue();
    double pyptar = tt->GetLeaf("P.gtr.th")->GetValue();



    

    Double_t q_x, q_y, q_z, Ep, Epp, Em, Q_2, Q_22, W_2, p_beam, Pm_par, Pm_per, Pm_t, E_corr;
    Double_t  esp_z, psp_z, p_ex, p_ey, p_ez, p_px, p_py, p_pz, estheta, pstheta;
    Double_t p_e, p_p, esphi, psphi; //variable of momentum range for e and p

    if (pbeta>0.6 && pbeta<1.4 && hbeta>0.8 && hbeta<1.2 && hcernpe>0.1 && pcernpe<0.1 && hcaletot >0.8 && hcaletot<1.2) {

     //*********************************************

      p_e = (1. + deltae/100) * ep_cen ;//calculating the momentum
      p_p = (1. + deltap/100) * pp_cen;

      E_corr = Ebeam; // beam energy 
      Ep = sqrt(p_e*p_e + Me*Me);//scattered electron energy
      Epp = sqrt(p_p*p_p + Mp*Mp);// energy of scattered proton

      esp_z = p_e/sqrt(1.0 + exptar*exptar + eyptar*eyptar);
      psp_z = p_p/sqrt(1.0 + pxptar*pxptar + pyptar*pyptar);

      //***************Component of momentum of e and p******************
      p_ex =  esp_z * exptar; 
      p_ey =  esp_z * (eyptar * cos(theta_ee) - sin(theta_ee));
      p_px =  psp_z * pxptar; 
      p_py =  psp_z * (pyptar * cos(theta_pp) - sin(theta_pp));

      p_ez =  esp_z * (eyptar * sin(theta_ee) + cos(theta_ee));
      p_pz =  psp_z * (-pyptar * sin(theta_pp) + cos(theta_pp));
      p_beam = sqrt(E_corr*E_corr-Me*Me); //momentum of initial electron
      q_x = -p_ex;
      q_y = -p_ey;
      q_z =  p_beam - p_ez;
  
      //************** recalculate scattering and azimuthal angles

      if (abs(p_ez/p_e)<=1.0)
	{
	  estheta = acos(p_ez/p_e); 
	}     
      else
	{
	  estheta = -10.0;
	}


      if (abs(p_pz/p_p)<=1.0)
	{
	  pstheta = acos(p_pz/p_p);
	}      
      else
	{
	  pstheta = -10.0;
	}      
        
      esphi = atan(p_ez/p_ey);      
      psphi = atan(p_pz/p_py);
         

      // calculate the physics variables Q^2, W^2, Em, Pmpar, Pmper 
      Q_2 = -((E_corr-Ep) * (E_corr-Ep) - (q_x*q_x + q_y*q_y + q_z*q_z));
      Q_22 = 4* E_corr* Ep * sin(estheta/2) * sin(estheta/2);
      W_2 = sqrt(Mp * Mp + 2 * Mp * (E_corr - Ep) - Q_22);           
      Em = E_corr + Mp - (Ep + Epp); // missing Energy.
      Pm_par = q_z - p_pz; //  missing parallel momentum
      Pm_per = sqrt((q_x-p_px)*(q_x-p_px) + (q_y-p_py)*(q_y-p_py));// missing perpendicular momentum.
      Pm_t = sqrt(Pm_par*Pm_par + Pm_per*Pm_per);//total missing momentum

      h_Emd->Fill(Em);
      h_Wd->Fill(W_2);
      h_pmd->Fill(Pm_t);
      h_perd->Fill(Pm_per);     
    } 
  }

  for (int kk=0; kk<nentriesM; kk++){
    ts->GetEntry(kk);
    if (kk % 50000 == 0) cout << kk*100/nentriesM << "   % of simc done" << endl;


    double sEm = ts->GetLeaf("Em")->GetValue();
    double sPm = ts->GetLeaf("Pm")->GetValue();
    double sW = ts->GetLeaf("W")->GetValue();
    double wt = ts->GetLeaf("Weight")->GetValue();
    double normwt = wt*normfac/nentriesM/sfacp;
    h_Ems->Fill(sEm,normwt);
    h_pms->Fill(sPm,normwt);
    h_Ws->Fill(sW,normwt);
  }

  //  cc->Update();
  gStyle->SetPaperSize(20,20);
  cc->cd(1);
  h_Ws->Draw();
  h_Ws->SetLineColor(kRed);
  h_Wd->Draw("SAME");
  h_Wd->SetLineColor(kBlue);

  cc->cd(2);
  h_Ems->Draw();
  h_Ems->SetLineColor(kRed);
  h_Emd->Draw("same");
  h_Emd->SetLineColor(kBlue);

  cc->cd(3);
  h_pms->Draw();
  h_pms->SetLineColor(kRed);
  h_pmd->Draw("same");
  h_pmd->SetLineColor(kBlue);

  cc->cd(4);
  h_perd->Draw();
  h_perd->SetLineColor(kBlue);

  cc->SaveAs(filepO);


  
  fout->Write();
  fout->Close();

}
