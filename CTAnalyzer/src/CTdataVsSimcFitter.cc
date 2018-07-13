// Filename: CTdataVsSimc.cc
// Description: Compare Data vs Simc. This script exclusively for CT expt and assumes SHMS as proton arm and HMS as electron arm
// Author: Latif Kabir < latif@jlab.org >
// Created: Thu Jan  4 17:03:26 2018 (-0500)
// URL: jlab.org/~latif

/*
Quantities to compare:
delta, ytar, xptar, yptar for Both HMS and SHMS, for both data and Simc. Simc and data histos are super layed on each other. For kinematics Em, W, Pm, Pmz.
In order to compare data and simc histograms must have same range and binning and drawn normalized.
*/

#include <iostream>
#include <TFile.h>
#include <TTree.h>
#include <TString.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TCanvas.h>
#include <TSystem.h>
#include <TStyle.h>
#include <TText.h>
#include <TMatrixD.h>
#include <TVectorD.h>
#include <TDecompSVD.h>
#include "Constants.h"
#include "CTRun.h"
#include "Riostream.h"
using namespace std;


void CTdataVsSimcFitter(Int_t firstRun, Int_t lastRun, TString SimcFileName, TString target, Double_t Q2)
{

  TFile *fout = new TFile("output_plots.root","RECREATE");

  ofstream fCorr;  
  fCorr.open("corrections.txt");

  vector<int> xfpexpon;
  vector<int> xpfpexpon;
  vector<int> yfpexpon;
  vector<int> ypfpexpon;
  vector<int> xtarexpon;

  TString currentline;
  int num_recon_terms = 0;
  ifstream infile("/u/home/hszumila/hallc/CT12GeV/CTAnalyzer/newfit_h1_q_8_skew.dat");
  int nfit_max=500000;
  int nfit = 0;
  //read in the infile to get the number of terms we will extract for fitting
  while( currentline.ReadLine(infile,kFALSE)){
    
    int expontemp[5];

    for(int expon=0; expon<5; expon++){
      TString stemp(currentline(66+expon,1));
      expontemp[expon] = stemp.Atoi();
    }  

    xfpexpon.push_back(expontemp[0]);
    xpfpexpon.push_back(expontemp[1]);
    yfpexpon.push_back(expontemp[2]);
    ypfpexpon.push_back(expontemp[3]);
    xtarexpon.push_back(expontemp[4]);
    cout<<"Terms: "<<expontemp[0]<<expontemp[1]<<expontemp[2]<<expontemp[3]<<expontemp[4]<<endl;
    
    /*
    int expontemp[3];

    for(int expon=0; expon<3; expon++){
      TString stemp(currentline(66+expon,1));
      expontemp[expon] = stemp.Atoi();
    }  

    yfpexpon.push_back(expontemp[0]);
    xpfpexpon.push_back(expontemp[1]);
    ypfpexpon.push_back(expontemp[2]);
    cout<<"Terms: "<<expontemp[0]<<expontemp[1]<<expontemp[2]<<endl;
    */
    num_recon_terms++;
  }
  cout<<"recon terms: "<<num_recon_terms<<endl;
  TMatrixD Ay(num_recon_terms,num_recon_terms);
  TMatrixD lambda(num_recon_terms,nfit_max);
  TVectorD b_delta(num_recon_terms);
  TMatrixD By(num_recon_terms,num_recon_terms);
  TMatrixD lambda2(num_recon_terms,nfit_max);
  TVectorD b_yptar(num_recon_terms);


    gStyle->SetOptStat(1); 
   //-----These offsets (for data) to be removed once SHMS is better calibrated ----------
    Double_t OffsetEm = 0.0;
    Double_t OffsetPdelta = 0.0;
    Double_t OffsetPmz = 0.0;
    Double_t OffsetW = 0.0;
    int cnts = 0;
    Double_t ebeam, etheta, ptheta, eP0, pP0;

     //Q2=8, sm coll
    if (Q2==8){
      ebeam=6.43;
      etheta=-45.11*3.141592/180.;
      ptheta=17.12*3.141592/180.;
      eP0=2.131*0.998;
      pP0=5.122*0.985*0.998;
    }
    else if (Q2==9.5){
      ebeam=10.58792;
      /*    
      //Q2=9.5, sm coll
      etheta=-23.186*3.141592/180.;
      ptheta=21.625*3.141592/180.;
      eP0=5.539*0.9898;
      pP0=5.925*0.985*0.997;
      */
      //Q2=9.5, lg coll
      etheta=-23.19*3.141592/180.;
      ptheta=21.595*3.141592/180.;
      eP0=5.539*0.9942;
      pP0=5.925*0.985*0.997;
    
     
    }
    else if (Q2==11.5){
      ebeam=10.58792;
      etheta=-28.505*3.141592/180.;
      ptheta=17.83*3.141592/180.;
      eP0=4.478*0.9945;
      pP0=7.001*0.985*0.9965;
    }
    else{
      ebeam=10.58792;
      //14.3, bothcoll:
      etheta=-39.28*3.141592/180.;
      ptheta=12.8*3.141592/180.;
      eP0=2.982*0.997;
      pP0=8.505*0.985*0.996;
   
    }

    Double_t mass_p=0.938272;
    Double_t mass_e=0.000511;
    Double_t hms_corsi, shms_corsi, ssim_corsi, hE, hP, pP, psimP, htheta, stheta, ssimtheta;
    Double_t r;
    //--------- Histograms from the data -----------
    //HMS
    //Reconstructed quantities
    TH1D *hDelta = new TH1D("hDelta","HMS DELTA (%); HMS dp (%); Normalized counts",100,-12,12);
    TH1D *hXptar = new TH1D("hXptar","HMS XPTAR (rad); x^{'}_{tar} (rad); Normalized counts",100,-0.1,0.1);
    TH1D *hYptar = new TH1D("hYptar","HMS YPTAR (rad); x^{'}_{tar} (rad); Normalized counts",100,-0.06,0.06);
    TH1D *hYtar = new TH1D("hYtar","HMS YTAR (cm); y_{tar} (cm); Normalized counts",100,-12.0,12.0);
    TH1D *hYtarc = new TH1D("hYtarc","HMS YTAR (cm); y_{tar} (cm); Normalized counts",100,-1.5,1.5);
    TH2D *pxxpfp = new TH2D("pxvsxp","SHMS x vs xp; x_fp (cm) wo cuts; xp_fp (rad)",100,-40,40,100,-0.1,0.1);
    TH2D *pxxpfp2 = new TH2D("pxvsxp2","SHMS x vs xp; x_fp (cm); xp_fp (rad)",100,-40,40,100,-0.1,0.1);
    TH2D *pxxpfp3 = new TH2D("pxvsxp3","SHMS x vs xp; x_fp (cm); xp_fp (rad)",100,-40,40,100,-0.1,0.1);
    //Kinematic quantities
    TH1D *hW = new TH1D("hW","W (GeV); W(GeV); Normalized counts", 150, 0.0, 2.0);
    //SHMS
    //Reconstructed quantities
    TH1D *pDelta = new TH1D("pDelta","SHMS DELTA (%); SHMS dp (%); Normalized counts",100,-12,12);
    TH1D *pXptar = new TH1D("pXptar","SHMS XPTAR (rad);x^{'}_{tar} (rad); Normalized counts",100,-0.06,0.06);
    TH1D *pYptar = new TH1D("pYptar","SHMS YPTAR (rad);y^{'}_{tar}(rad); Normalized counts",100,-0.06,0.06);
    TH1D *pYtar = new TH1D("pYtar","SHMS YTAR (cm); y_{tar} (cm); Normalized counts",100,-12.0,12.0);      
    TH1D *pYtarc = new TH1D("pYtarc","SHMS YTAR (cm); y_{tar} (cm); Normalized counts",100,-1.5,1.5);      
    TH1D *pYptarCorr = new TH1D("pYptarCorr","SHMS YPTAR (rad);y^{'}_{tar}(rad); Normalized counts",100,-0.06,0.06);
    TH1D *pDeltaCorr = new TH1D("pDeltaCorr","SHMS DELTA (%); SHMS dp (%); Normalized counts",100,-12,12);

    //Kinematic quantities
    TH1D *pEm = new TH1D("pEm","Missing Energy (GeV); E_{m} (GeV); Normalized counts",200,-0.15,0.25);
    TH1D *pPm = new TH1D("pPm","Pm (GeV/c); |P_{m}|(GeV/c); Normalized counts", 100, 0.0, 0.6);
    TH1D *pPmz = new TH1D("pPmz","Pmz (GeV/c); P_{mz} (GeV/c); Normalized counts", 100, -0.4, 0.4);

    //--------- Histograms from the Simc -----------
    //HMS
    //Reconstructed quantities
    TH1D *hSimcDelta = new TH1D("hSimcDelta","HMS DELTA (SIMC) (%)",100,-12,12);
    TH1D *hSimcXptar = new TH1D("hSimcXptar","HMS XPTAR (SIMC) (rad)",100,-0.1,0.1);
    TH1D *hSimcYptar = new TH1D("hSimcYptar","HMS YPTAR (SIMC) (rad)",100,-0.06,0.06);
    TH1D *hSimcYtar = new TH1D("hSimcYtar","HMS YTAR (SIMC) (cm)",100,-12.0,12.0);
    TH1D *hSimcYtarc = new TH1D("hSimcYtarc","HMS YTAR (SIMC) (cm)",100,-1.5,1.5);
    //Kinematic quantities
    TH1D *hSimcW = new TH1D("hSimcW","W (GeV)", 150, 0.0, 2.0);
   
    hSimcDelta->SetMarkerColor(kRed);  
    hSimcXptar->SetMarkerColor(kRed);  
    hSimcYptar->SetMarkerColor(kRed);  
    hSimcYtar->SetMarkerColor(kRed);  
    hSimcYtarc->SetMarkerColor(kRed);  
    hSimcW->SetMarkerColor(kRed);  
    hSimcDelta->SetLineColor(kRed);  
    hSimcXptar->SetLineColor(kRed);  
    hSimcYptar->SetLineColor(kRed);  
    hSimcYtar->SetLineColor(kRed);  
    hSimcYtarc->SetLineColor(kRed);  
    hSimcW->SetLineColor(kRed); 
    pYptarCorr->SetMarkerColor(kGreen+3);
    pYptarCorr->SetLineColor(kGreen+3);
    pDeltaCorr->SetMarkerColor(kGreen+3);
    pDeltaCorr->SetLineColor(kGreen+3);

    //SHMS
    //Reconstructed quantities
    TH1D *pSimcDelta = new TH1D("pSimcDelta","SHMS DELTA (SIMC) (%)",100,-12,12);
    TH1D *pSimcXptar = new TH1D("pSimcXptar","SHMS XPTAR (SIMC) (rad)",100,-0.06,0.06);
    TH1D *pSimcYptar = new TH1D("pSimcYptar","SHMS YPTAR (SIMC) (rad)",100,-0.06,0.06);
    TH1D *pSimcYtar = new TH1D("pSimcYtar","SHMS YTAR (SIMC) (cm)",100,-12.0,12.0);
    TH1D *pSimcYtarc = new TH1D("pSimcYtarc","SHMS YTAR (SIMC) (cm)",100,-1.5,1.5);
    //Kinematic quantities
    TH1D *pSimcW = new TH1D("pSimcW","W (GeV)", 150, 0.0, 2.0);
    TH1D *pSimcEm = new TH1D("pSimcEm","Missing Energy (GeV)",200,-0.15,0.25);
    TH1D *pSimcPm = new TH1D("pSimcPm","|P_{m}| (GeV/c)", 100, 0.0, 0.6);
    TH1D *pSimcPmz = new TH1D("pSimcPmz","P_{mz} (GeV/c)", 100, -0.4, 0.4);

    TH1D *hcorsi = new TH1D("hcorsi","(E_hms - E')[GeV]; h_energy diff(GeV); Normalized counts", 100, -0.2, 0.2);
    TH1D *scorsi = new TH1D("scorsi","(P_shms - P')[GeV]; p_mom diff(GeV); Normalized counts", 100, -0.2, 0.2);
    TH1D *hSimcorsi = new TH1D("hSimcorsi","hmsE - E' (GeV); h_energy diff(GeV); Normalized counts", 100, -0.2, 0.2);
    TH1D *sSimcorsi = new TH1D("sSimcorsi","P_shms - P' (GeV); p_mom diff(GeV); Normalized counts", 100, -0.2, 0.2);
    TH2D *pPEm = new TH2D("pPEm","SHMS pP vs Em; pP; Em",150,-0.05,0.15,100,pP0-0.2,pP0+0.2);
    TH2D *pthEm = new TH2D("pthEm","SHMS pth vs Em; pth; Em",150,-0.05,0.15,100,ptheta-0.03,ptheta+0.03);
    TH2D *emdelta = new TH2D("emdelta","SHMS Em vs delta; Em; delta",100,-4,4,150,-0.05,0.15);
    TH2D *emxptar = new TH2D("emxptar","SHMS Em vs xptar; Em; xptar (rad)",100,-0.03,0.03,150,-0.05,0.15);
    TH2D *scorsiEm = new TH2D("scorsiEm","SHMS corsi vs Em; scorsi; Em",150,-0.05,0.15,100,-0.1,0.1);
    TH2D *emyptar = new TH2D("emyptar","SHMS Em vs yptar; Em; yptar (rad)",100,-0.02,0.02,150,-0.05,0.15);
    TH2D *emxpfp = new TH2D("emxpfp","SHMS Em vs xpfp; Em; xpfp (rad)",100,-0.03,0.03,150,-0.05,0.15);

    //plots with corrected delta
    TH2D *pPEmcorr = new TH2D("pPEm","SHMS pP vs Em; pP; Em",150,-0.05,0.15,100,pP0-0.2,pP0+0.2);
    TH2D *pthEmcorr = new TH2D("pthEmcorr","SHMS pth vs Em; pth; Em",150,-0.05,0.15,100,ptheta-0.03,ptheta+0.03);
    TH2D *emdeltacorr = new TH2D("emdeltacorr","SHMS Em vs delta; Em; delta",100,-4,4,150,-0.05,0.15);
    TH2D *emxptarcorr = new TH2D("emxptarcorr","SHMS Em vs xptar; Em; xptar (rad)",100,-0.03,0.03,150,-0.05,0.15);
    TH2D *emyptarcorr = new TH2D("emyptarcorr","SHMS Em vs yptar; Em; yptar (rad)",100,-0.02,0.02,150,-0.05,0.15);
    TH2D *emxpfpcorr = new TH2D("emxpfpcorr","SHMS Em vs xpfp; Em; xpfp (rad)",100,-0.03,0.03,150,-0.05,0.15);


    TH2D *h2_xfp = new TH2D("h2_xfp",";xfp;delta diff [%]",100,-10,10,100,-1,1);
    TH2D *h2_yfp = new TH2D("h2_yfp",";yfp;delta diff [%]",100,-10,10,100,-1,1);
    TH2D *h2_xpfp = new TH2D("h2_xpfp",";xpfp;delta diff [%]",100,-0.04,0.04,100,-1,1);
    TH2D *h2_ypfp = new TH2D("h2_ypfp",";ypfp;delta diff [%]",100,-0.04,0.04,100,-1,1);
    TH2D *h2_xfpCorr = new TH2D("h2_xfpCorr",";xfp;delta diff [%]",100,-10,10,100,-1,1);
    TH2D *h2_yfpCorr = new TH2D("h2_yfpCorr",";yfp;delta diff [%]",100,-10,10,100,-1,1);
    TH2D *h2_xpfpCorr = new TH2D("h2_xpfpCorr",";xpfp;delta diff [%]",100,-0.04,0.04,100,-1,1);
    TH2D *h2_ypfpCorr = new TH2D("h2_ypfpCorr",";ypfp;delta diff [%]",100,-0.04,0.04,100,-1,1);
    TH1D *pEmOpt = new TH1D("pEmOpt","Missing Energy (GeV); E_{m} (GeV); Normalized counts",200,-0.15,0.25);
    TH1D *sSimcorsiOpt = new TH1D("sSimcorsiOpt","P_shms - P_calc (GeV); p_mom diff(GeV); Normalized counts", 100, -0.2, 0.2);
    TH2D *sSimcorsiVyptarOpt = new TH2D("sSimcorsiVyptarOpt","SHMS yptar vs P_shms-P_calc;yptar;P_shms - P_calc (GeV); Normalized counts", 100,-0.02,0.02,100, -0.2, 0.2);
    TH1D *sSimcorsiOpt2 = new TH1D("sSimcorsiOpt2","P_shms - P_calc (GeV); p_mom diff(GeV); Normalized counts", 100, -0.2, 0.2);
    TH2D *sSimcorsiVyptarOpt2 = new TH2D("sSimcorsiVyptarOpt2","SHMS yptar vs P_shms-P_calc;yptar;P_shms - P_calc (GeV); Normalized counts", 100,-0.02,0.02,100, -0.2, 0.2);

    TH2D *h2_sinDiff0 = new TH2D("h2_sinDiff0","HMS yptar vs p_{p}sin#theta_{p} - p_{e}sin#theta_{e};yptar_{HMS}; P_{p}sin#theta_{SHMS} - P_{e}sin#theta_{HMS}",100,-0.06,0.06,100,-0.1,0.1);    
    TH2D *h2_sinDiff_d = new TH2D("h2_sinDiff_d","HMS yptar vs p_{p}sin#theta_{p} - p_{e}sin#theta_{e};yptar_{HMS}; P_{p}sin#theta_{SHMS} - P_{e}sin#theta_{HMS}",100,-0.06,0.06,100,-0.1,0.1);
    TH2D *h2_sinDiff_dyp = new TH2D("h2_sinDiff_dyp","HMS yptar vs p_{p}sin#theta_{p} - p_{e}sin#theta_{e};yptar_{HMS}; P_{p}sin#theta_{SHMS} - P_{e}sin#theta_{HMS}",100,-0.06,0.06,100,-0.1,0.1);
    TH2D *h2_xpDiff = new TH2D("h2_xpDiff","HMS xptar vs p_{p}x'-p_{e}x';xptar_{HMS}; P_{p}x'_{tar} - P_{e}x'_{tar}",100,-0.1,0.1,100,-0.2,0.2);
    TH2D *h2_ytarDiff = new TH2D("h2_ytarDiff","HMS ytar vs yt_{p}/sin#theta_{p} - yt_{e}/sin#theta_{e};ytar_{HMS};y_{tar,SHMS} - y_{tar,HMS}",100,-7,7,100,-5,5);
    TH2D *h2_ytarDiffCorr = new TH2D("h2_ytarDiffCorr","HMS ytar vs yt_{p}/sin#theta_{p} - yt_{e}/sin#theta_{e};ytar_{HMS};y_{tar,SHMS} - y_{tar,HMS}",100,-7,7,100,-5,5);

    pEmOpt->SetMarkerColor(8);
    pEmOpt->SetLineColor(8);
    sSimcorsiOpt->SetMarkerColor(8);
    sSimcorsiOpt->SetLineColor(8);
    sSimcorsiOpt2->SetMarkerColor(1);
    sSimcorsiOpt2->SetLineColor(1);

    hSimcorsi->SetMarkerColor(kRed);  
    sSimcorsi->SetMarkerColor(kRed);  
    pSimcDelta->SetMarkerColor(kRed);  
    pSimcXptar->SetMarkerColor(kRed);  
    pSimcYptar->SetMarkerColor(kRed);  
    pSimcYtar->SetMarkerColor(kRed);  
    pSimcYtarc->SetMarkerColor(kRed);  
    pSimcEm->SetMarkerColor(kRed);  
    pSimcPm->SetMarkerColor(kRed);  
    pSimcPmz->SetMarkerColor(kRed);  
    hSimcorsi->SetLineColor(kRed);  
    sSimcorsi->SetLineColor(kRed);  
    pSimcDelta->SetLineColor(kRed);  
    pSimcXptar->SetLineColor(kRed);  
    pSimcYptar->SetLineColor(kRed);  
    pSimcYtar->SetLineColor(kRed);  
    pSimcYtarc->SetLineColor(kRed);  
    pSimcEm->SetLineColor(kRed);  
    pSimcPm->SetLineColor(kRed);  
    pSimcPmz->SetLineColor(kRed);  
    
    CTRun *ct = new CTRun(firstRun,lastRun);
    if(!ct->fRunExist)
	return;
    //------------------------------------ Fill histograms from data ------- 
    TTree * tree = ct->GetTree();

    Int_t nEvents = tree->GetEntries();

    for (Int_t event = 0; event < nEvents; ++ event)
    {
	if(event%50000 == 0)
	    cout << (Int_t)(100*event/nEvents) <<" % Done"<< endl;

	tree->GetEntry(event);

        pxxpfp->Fill(ct->fP_dc_x_fp,ct->fP_dc_xp_fp);
	if (ct->fP_hod_2ynhits > 0) pxxpfp3->Fill(ct->fP_dc_x_fp,ct->fP_dc_xp_fp);
 
	// PID & Kinematic Cut
	if( !( ct->fP_gtr_beta > 0.6 && ct->fP_gtr_beta < 1.5    // proton beta cut
	       && ct->fH_gtr_beta > 0.8 &&  ct->fH_gtr_beta < 1.2                         // e beta cut
	       && ct->fH_cer_npeSum > 0. 
	       && ct->fP_hgcer_npeSum < 0.1                    // Cerenkov counter cut for e- and proton
	       && ct->fH_cal_etottracknorm > 0.6 && ct->fH_cal_etottracknorm < 2.0 // HMS Calorimeter cut 	
	       && ct->fH_hod_goodstarttime == 1                                           // good hms start time
	       && ct->fP_hod_goodstarttime == 1                                           // good shms stat time
	       && ct->fH_gtr_dp > -10 && ct->fH_gtr_dp < 10                               // e Delta cut
	       && ct->fP_gtr_dp > -15 && ct->fP_gtr_dp < 15 
	       )) {continue;}

	// Target specific kinematic cut
	if(target == "c")       // For carbon-12
	  {
	    if(!(fabs(ct->fP_kin_secondary_pmiss)<0.4))                                    // Kinematic cut
	      continue;
	  }
	else if(target == "h")  // For hydrogen-1
	  {
	    if(!(ct->fH_kin_primary_W>=0.8 && ct->fH_kin_primary_W<=1.15))                // Kinematic cut
	      continue;	    
	  }
	else
	  {
	    cout << "Invalid target type" <<endl;
	    return;
	  }
        
	//HMS
	hDelta->Fill(ct->fH_gtr_dp);
	hXptar->Fill(ct->fH_gtr_th);
	hYptar->Fill(ct->fH_gtr_ph);
	hYtar->Fill(ct->fH_gtr_y);
	//SHMS
	pDelta->Fill(ct->fP_gtr_dp + OffsetPdelta);
	pXptar->Fill(ct->fP_gtr_th);
	pYptar->Fill(ct->fP_gtr_ph);
	pYtar->Fill(ct->fP_gtr_y);
        pxxpfp2->Fill(ct->fP_dc_x_fp,ct->fP_dc_xp_fp);

        if (target == "h")
	 {
	   pPm->GetXaxis()->SetRangeUser(-0.15,0.25);
           pEm->GetXaxis()->SetRangeUser(-0.15,0.25);
         }

	//--- Kinematic quantities ----	
	hW->Fill(ct->fH_kin_primary_W + OffsetW);
	pPm->Fill(fabs(ct->fP_kin_secondary_pmiss));
	pPmz->Fill(ct->fP_kin_secondary_pmiss_z + OffsetPmz);

	if(target == "h")
	  {
	    pEm->Fill(ct->fP_kin_secondary_emiss + OffsetEm);
            hP = eP0*(1.+ (ct->fH_gtr_dp)/100.);
            hE = hP;
            htheta = sqrt(pow(etheta + (ct->fH_gtr_ph),2.0)+pow(ct->fH_gtr_th,2.0));//etheta + (ct->fH_gtr_ph);//acos((cos(etheta) + (ct->fH_gtr_ph)*sin(etheta))/r);
	    hms_corsi = hE - ebeam/(1.+ebeam*(1-cos(htheta))/mass_p);
            pP = pP0*(1.+ (ct->fP_gtr_dp)/100.);
            stheta = sqrt(pow(ptheta + (ct->fP_gtr_ph),2.0)+pow(ct->fP_gtr_th,2.0));
            shms_corsi = pP - 2.*mass_p*ebeam*cos(stheta)/(ebeam+mass_p)/(1-pow(ebeam*cos(stheta)/(ebeam+mass_p),2));
            hcorsi->Fill(hms_corsi);
            scorsi->Fill(shms_corsi);
	  
	    ///Fit some stuff/////////
	    if (ct->fH_kin_primary_W<=1.0 && abs(ct->fP_kin_secondary_emiss+0.43*ct->fP_dc_xp_fp-0.006)<0.01 && target=="h"){
		Double_t etemp, Ep_calc, Pp_calc, delta_calc;
		for( int icoeff_fit=0; icoeff_fit<num_recon_terms; icoeff_fit++ ){
		  etemp= 
		    pow( ct->fP_dc_x_fp / 100.0, xfpexpon[icoeff_fit] ) * 
		    pow( ct->fP_dc_y_fp / 100.0, yfpexpon[icoeff_fit] ) * 
		    pow( ct->fP_dc_xp_fp, xpfpexpon[icoeff_fit] ) * 
		    pow( ct->fP_dc_yp_fp, ypfpexpon[icoeff_fit] ) * 
		    pow( ct->fP_gtr_x/100., xtarexpon[icoeff_fit] );

		  /* 
		     etemp= 
		     pow( ct->fP_gtr_y, yfpexpon[icoeff_fit] ) * 
		     pow( ct->fP_gtr_th, xpfpexpon[icoeff_fit] ) * 
		     pow( ct->fP_gtr_ph, ypfpexpon[icoeff_fit] ); 
		  */
		  if (nfit<nfit_max){
		    lambda[icoeff_fit][nfit] = etemp;
		    Ep_calc = ebeam+mass_p-hE;
		    Pp_calc = sqrt(Ep_calc*Ep_calc - mass_p*mass_p);
		    //delta_calc = ((2.*mass_p*ebeam*cos(stheta)/(ebeam+mass_p)/(1-pow(ebeam*cos(stheta)/(ebeam+mass_p),2)))/pP0-1.0)*100.0;
		    delta_calc = 100.0*(sqrt(pow(ebeam-hP+mass_p,2.0)-pow(mass_p,2.0))/pP0-1.0);

		    //minimize over delta calc - delta recon
		    //b_delta[icoeff_fit] += (delta_calc - ct->fP_gtr_dp) * etemp;
		    //b_delta[icoeff_fit] += (ct->fP_kin_secondary_emiss) * etemp;
		    b_delta[icoeff_fit] += (delta_calc - ct->fP_gtr_dp) * etemp;
		    
		  }//end if nfit
		}//end coeff loop
		if (nfit<nfit_max){
		  nfit++;
		  h2_xfp->Fill(ct->fP_dc_x_fp,delta_calc - ct->fP_gtr_dp);
		  h2_yfp->Fill(ct->fP_dc_y_fp,delta_calc - ct->fP_gtr_dp);
		  h2_xpfp->Fill(ct->fP_dc_xp_fp,delta_calc - ct->fP_gtr_dp);
		  h2_ypfp->Fill(ct->fP_dc_yp_fp,delta_calc - ct->fP_gtr_dp);
		  
		  pPEm->Fill(ct->fP_kin_secondary_emiss,pP);
		  pthEm->Fill(ct->fP_kin_secondary_emiss,stheta);
		  
		  emdelta->Fill(ct->fP_gtr_dp,ct->fP_kin_secondary_emiss);
		  emxptar->Fill(ct->fP_gtr_th,ct->fP_kin_secondary_emiss);
		  scorsiEm->Fill(ct->fP_kin_secondary_emiss,shms_corsi);
		  emyptar->Fill(ct->fP_gtr_ph,ct->fP_kin_secondary_emiss);
		  
		  emxpfp->Fill(ct->fP_dc_xp_fp,ct->fP_kin_secondary_emiss);
		}
	    }
	    //if(abs(ct->fP_gtr_th)<0.01&&abs(ct->fH_gtr_th)<0.01){
	      h2_sinDiff0->Fill(ct->fH_gtr_ph,pP*sin(ptheta+ct->fP_gtr_ph) + hP*sin(etheta+ct->fH_gtr_ph));
	      h2_xpDiff->Fill(ct->fH_gtr_th,pP*ct->fP_gtr_th + hP*ct->fH_gtr_th);
	      h2_ytarDiff->Fill(ct->fH_gtr_y,ct->fP_gtr_y/sin(ptheta+ct->fP_gtr_ph)-ct->fH_gtr_y/sin(etheta+ct->fH_gtr_ph));
	      //}
	    //calc W myself
	    /*
	    double qq2 = 2.0*ebeam*hE*(1-cos(htheta));
	    double xxb = qq2/(2*mass_p*(ebeam-hE));
	    double wW = sqrt(mass_p*mass_p+qq2*(1.0/(xxb*xxb)-1.0));
	    /////////////
	    hW->Fill(wW);
	    */
          }       
	else
          {
	    pEm->Fill(ct->fP_kin_secondary_emiss_nuc + OffsetEm);
	    hYtarc->Fill(ct->fH_gtr_y);
	    pYtarc->Fill(ct->fP_gtr_y);
          }
        cnts++;
	  
    }
    cout << cnts <<" events passed all cuts" << endl;
    //////////////////////////////////////////////////
    if (nfit!=0){
      for(int i=0; i<num_recon_terms; i++){
	for(int j=0; j<num_recon_terms; j++){
	  Ay[i][j] = 0.0;
	}
      }
      for( int ifit=0; ifit<nfit; ifit++){
	if( ifit % 100 == 0 ) cout << ifit << endl;
	for( int ipar=0; ipar<num_recon_terms; ipar++){
	  for( int jpar=0; jpar<num_recon_terms; jpar++){
	    Ay[ipar][jpar] += lambda[ipar][ifit] * lambda[jpar][ifit];
	  }
	}
      }
      TDecompSVD Ay_svd(Ay);
      bool ok;
      ok = Ay_svd.Solve( b_delta );
      cout << "delta solution ok = " << ok << endl;
      b_delta.Print();
      char coeffstring[100];
      Double_t tt;
      fCorr<<"delta correction"<<endl;

      for( int icoeff_fit=0; icoeff_fit<num_recon_terms; icoeff_fit++ ){
        cout<< " ";
	tt=b_delta[icoeff_fit];
	sprintf( coeffstring, "%16.9g", tt );
	cout << coeffstring; 
	fCorr<< coeffstring;

	cout << "      "<< xfpexpon[icoeff_fit]; 
	cout << xpfpexpon[icoeff_fit]; 
	cout << yfpexpon[icoeff_fit]; 
	cout << ypfpexpon[icoeff_fit]; 
	cout << xtarexpon[icoeff_fit]; 
	cout << endl;

	fCorr << "      "<< xfpexpon[icoeff_fit]; 
	fCorr << xpfpexpon[icoeff_fit]; 
	fCorr << yfpexpon[icoeff_fit]; 
	fCorr << ypfpexpon[icoeff_fit]; 
	fCorr << xtarexpon[icoeff_fit]; 
	fCorr << endl;
	/*
	cout << "     "<< yfpexpon[icoeff_fit]; 
	cout << xpfpexpon[icoeff_fit]; 
	cout << ypfpexpon[icoeff_fit]; 
	cout << endl;
	*/
      }
    }
    /////////////////////////////////////////////////

    //loop again, correct delta and emiss, use to correct yptar
    nfit = 0;
    for (int ii=0; ii<nEvents; ii++){
      tree->GetEntry(ii);
      if(  !(ct->fP_gtr_beta > 0.6 && ct->fP_gtr_beta < 1.5
	     && ct->fH_gtr_beta > 0.8 &&  ct->fH_gtr_beta < 1.2
	     && ct->fH_cer_npeSum > 0. 
	     && ct->fP_hgcer_npeSum < 0.1   
	     && ct->fH_cal_etottracknorm > 0.6 && ct->fH_cal_etottracknorm < 2.0 	
	     && ct->fH_hod_goodstarttime == 1                   
	     && ct->fP_hod_goodstarttime == 1    
	     && ct->fH_gtr_dp > -10 && ct->fH_gtr_dp < 10    
	     && ct->fP_gtr_dp > -15 && ct->fP_gtr_dp < 15 
	     && ct->fH_kin_primary_W>=0.8 && ct->fH_kin_primary_W<=1.15 
	     && target=="h"
	     )){continue;}      
	Double_t emissnew=0.0;
	Double_t deltanew=0.0;
	Double_t etemp;
	Double_t delta_calc=0.0;
	Double_t shms_corsi;
	//b = true - sat
	for( int icoeff_fit=0; icoeff_fit<num_recon_terms; icoeff_fit++ ){
	  etemp= 
	    pow( ct->fP_dc_x_fp / 100.0, xfpexpon[icoeff_fit] ) * 
	    pow( ct->fP_dc_y_fp / 100.0, yfpexpon[icoeff_fit] ) * 
	    pow( ct->fP_dc_xp_fp, xpfpexpon[icoeff_fit] ) * 
	    pow( ct->fP_dc_yp_fp, ypfpexpon[icoeff_fit] ) * 
	    pow( ct->fP_gtr_x/100., xtarexpon[icoeff_fit] );
	    deltanew += b_delta[icoeff_fit]* etemp;
	}

	deltanew = ct->fP_gtr_dp + deltanew;
	//emissnew = ct->fP_kin_secondary_emiss - emissnew;
	double hP = eP0*(1.+ (ct->fH_gtr_dp)/100.);
	//deltanew = 100.0*(sqrt(pow(ebeam - emissnew-hP+mass_p,2.0)-pow(mass_p,2.0))/pP0-1.0);
	double stheta = sqrt(pow(ptheta + (ct->fP_gtr_ph),2.0)+pow(ct->fP_gtr_th,2.0));//ptheta + (ct->fP_gtr_ph );
	double pP = pP0*(1.+ (deltanew)/100.);
	emissnew = ebeam - sqrt(pP*pP + mass_p*mass_p) - hP + mass_p;
	delta_calc = 100.0*(sqrt(pow(ebeam-hP+mass_p-emissnew,2.0)-pow(mass_p,2.0))/pP0-1.0);

	shms_corsi = pP - 2.*mass_p*ebeam*cos(stheta)/(ebeam+mass_p)/(1-pow(ebeam*cos(stheta)/(ebeam+mass_p),2));

	sSimcorsiVyptarOpt->Fill(ct->fP_gtr_ph,shms_corsi);

	sSimcorsiOpt->Fill(shms_corsi);
	pEmOpt->Fill(emissnew);
	if (ct->fH_kin_primary_W<=1.0 && abs(ct->fP_kin_secondary_emiss+0.43*ct->fP_dc_xp_fp-0.006)<0.01){
	  pPEmcorr->Fill(emissnew,pP);
	  pthEmcorr->Fill(emissnew,stheta);
	  emdeltacorr->Fill(deltanew,emissnew);
	  emxptarcorr->Fill(ct->fP_gtr_th,emissnew);
	  emyptarcorr->Fill(ct->fP_gtr_ph,emissnew);
	  emxpfpcorr->Fill(ct->fP_dc_xp_fp,emissnew);
	  h2_xfpCorr->Fill(ct->fP_dc_x_fp,delta_calc - deltanew);
	  h2_yfpCorr->Fill(ct->fP_dc_y_fp,delta_calc -deltanew);
	  h2_xpfpCorr->Fill(ct->fP_dc_xp_fp,delta_calc -deltanew);
	  h2_ypfpCorr->Fill(ct->fP_dc_yp_fp,delta_calc -deltanew);
	}

	h2_sinDiff_d->Fill(ct->fH_gtr_ph,pP*sin(stheta) + hP*sin(etheta + ct->fH_gtr_ph));

	//find correction for yptar
	double yptar_calc = acos((-mass_p+sqrt(pow(mass_p,2.0)+pow(pP,2.0)))/(pP*ebeam/(mass_p+ebeam))) - ptheta;
	if (abs(shms_corsi-ct->fP_gtr_ph)<0.03&&abs(ct->fP_gtr_th)<0.01&&ct->fH_kin_primary_W<=1.0 && abs(ct->fP_kin_secondary_emiss+0.43*ct->fP_dc_xp_fp-0.006)<0.01 && target=="h"){
	  for( int icoeff_fit=0; icoeff_fit<num_recon_terms; icoeff_fit++ ){
	    etemp= 
	      pow( ct->fP_dc_x_fp / 100.0, xfpexpon[icoeff_fit] ) * 
	      pow( ct->fP_dc_y_fp / 100.0, yfpexpon[icoeff_fit] ) * 
	      pow( ct->fP_dc_xp_fp, xpfpexpon[icoeff_fit] ) * 
	      pow( ct->fP_dc_yp_fp, ypfpexpon[icoeff_fit] ) * 
	      pow( ct->fP_gtr_x/100., xtarexpon[icoeff_fit] );
	    if (nfit<nfit_max){
	      lambda2[icoeff_fit][nfit] = etemp;
	      b_yptar[icoeff_fit]  += (yptar_calc - ct->fP_gtr_ph)*etemp; 
	    } 
	  }
	  if (nfit<nfit_max){
	    nfit++;
	  }
	}
    }
    //find fit to yptar
     if (nfit!=0){
      for(int i=0; i<num_recon_terms; i++){
	for(int j=0; j<num_recon_terms; j++){
	  By[i][j] = 0.0;
	}
      }
      for( int ifit=0; ifit<nfit; ifit++){
	if( ifit % 100 == 0 ) cout << ifit << endl;
	for( int ipar=0; ipar<num_recon_terms; ipar++){
	  for( int jpar=0; jpar<num_recon_terms; jpar++){
	    By[ipar][jpar] += lambda2[ipar][ifit] * lambda2[jpar][ifit];
	  }
	}
      }
      TDecompSVD By_svd(By);
      bool ok;
      ok = By_svd.Solve( b_yptar );
      cout << "yptar solution ok = " << ok << endl;
      b_yptar.Print();
      char coeffstring2[100];
      Double_t tt2;
      fCorr<<"yptar correction"<<endl;

      for( int icoeff_fit=0; icoeff_fit<num_recon_terms; icoeff_fit++ ){
        cout<< " ";
	tt2=b_yptar[icoeff_fit];
	sprintf( coeffstring2, "%16.9g", tt2 );
	cout << coeffstring2; 
	fCorr << coeffstring2; 

	cout << "      "<< xfpexpon[icoeff_fit]; 
	cout << xpfpexpon[icoeff_fit]; 
	cout << yfpexpon[icoeff_fit]; 
	cout << ypfpexpon[icoeff_fit]; 
	cout << xtarexpon[icoeff_fit]; 
	cout << endl;

	fCorr << "      "<< xfpexpon[icoeff_fit]; 
	fCorr << xpfpexpon[icoeff_fit]; 
	fCorr << yfpexpon[icoeff_fit]; 
	fCorr << ypfpexpon[icoeff_fit]; 
	fCorr << xtarexpon[icoeff_fit]; 
	fCorr << endl;

      }
     }
     fCorr.close();

     //Third loop to look at corrected yptar and delta
     for (int ii=0; ii<nEvents; ii++){
       tree->GetEntry(ii);
       if(  !(ct->fP_gtr_beta > 0.6 && ct->fP_gtr_beta < 1.5
	      && ct->fH_gtr_beta > 0.8 &&  ct->fH_gtr_beta < 1.2
	      && ct->fH_cer_npeSum > 0. 
	      && ct->fP_hgcer_npeSum < 0.1   
	      && ct->fH_cal_etottracknorm > 0.6 && ct->fH_cal_etottracknorm < 2.0 	
	      && ct->fH_hod_goodstarttime == 1                   
	      && ct->fP_hod_goodstarttime == 1    
	      && ct->fH_gtr_dp > -10 && ct->fH_gtr_dp < 10    
	      && ct->fP_gtr_dp > -15 && ct->fP_gtr_dp < 15 
	      && ct->fH_kin_primary_W>=0.8 && ct->fH_kin_primary_W<=1.15 
	      && target=="h"
	      )){continue;} 
       	Double_t emissnew=0.0;
	Double_t deltanew=0.0;
	Double_t yptarNew = 0.0;
	Double_t etemp;
	Double_t delta_calc=0.0;
	Double_t shms_corsi;
	//b = true - sat
	for( int icoeff_fit=0; icoeff_fit<num_recon_terms; icoeff_fit++ ){
	  etemp= 
	    pow( ct->fP_dc_x_fp / 100.0, xfpexpon[icoeff_fit] ) * 
	    pow( ct->fP_dc_y_fp / 100.0, yfpexpon[icoeff_fit] ) * 
	    pow( ct->fP_dc_xp_fp, xpfpexpon[icoeff_fit] ) * 
	    pow( ct->fP_dc_yp_fp, ypfpexpon[icoeff_fit] ) * 
	    pow( ct->fP_gtr_x/100., xtarexpon[icoeff_fit] );
	    deltanew += b_delta[icoeff_fit]* etemp;
	    yptarNew += b_yptar[icoeff_fit]*etemp;
	}

	yptarNew = ct->fP_gtr_ph + yptarNew;
	deltanew = ct->fP_gtr_dp + deltanew;
	double hP = eP0*(1.+ (ct->fH_gtr_dp)/100.);
	double stheta = sqrt(pow(ptheta + (yptarNew),2.0)+pow(ct->fP_gtr_th,2.0));//ptheta + (yptarNew);
	double pP = pP0*(1.+ (deltanew)/100.);
	emissnew = ebeam - sqrt(pP*pP + mass_p*mass_p) - hP + mass_p;
	delta_calc = 100.0*(sqrt(pow(ebeam-hP+mass_p-emissnew,2.0)-pow(mass_p,2.0))/pP0-1.0);

	shms_corsi = pP - 2.*mass_p*ebeam*cos(stheta)/(ebeam+mass_p)/(1-pow(ebeam*cos(stheta)/(ebeam+mass_p),2));

	sSimcorsiVyptarOpt2->Fill(yptarNew,shms_corsi);
	sSimcorsiOpt2->Fill(shms_corsi);

	h2_sinDiff_dyp->Fill(ct->fH_gtr_ph,pP*sin(stheta) + hP*sin(etheta + ct->fH_gtr_ph));
	h2_ytarDiffCorr->Fill(ct->fH_gtr_y,ct->fP_gtr_y/sin(stheta)-ct->fH_gtr_y/sin(etheta + ct->fH_gtr_ph));
	pYptarCorr->Fill(yptarNew);
	pDeltaCorr->Fill(deltanew);

     }


    //------------------ Plot from Simc --------------------------
    ct->AddSimc(SimcFileName);

    if(!ct->fSimcExist)
	return;
    
    TTree *simc = ct->GetSimcTree();

    Int_t nEventsSimc = simc->GetEntries();

    for (Int_t event = 0; event < nEventsSimc; ++event)
    {
	if(event%50000 == 0)
	    cout << (Int_t)(100*event/nEventsSimc) <<" % Done"<< endl;

	simc->GetEntry(event);
	//Kinematic Cut
	if( !(ct->fHSdelta > -10 && ct->fHSdelta < 10
	      && ct->fPSdelta > -15 && ct->fPSdelta < 15))
	    continue;	
	if(target == "c")       // For carbon-12
	{
	    if(!(fabs(ct->fPm)<0.4))                                    // Kinematic cut
		continue;
	}
	else if(target == "h")  // For hydrogen-1
	{
	    if(!(ct->fW>=0.8 && ct->fW<=1.15))                // Kinematic cut
		continue;	    
	}




	//------- Reconstructed quantities ------

	//HMS
	hSimcDelta->Fill(ct->fHSdelta, ct->fWeight);
	hSimcYtar->Fill(ct->fHSytar, ct->fWeight);
	hSimcYptar->Fill(ct->fHSyptar, ct->fWeight);
	hSimcXptar->Fill(ct->fHSxptar, ct->fWeight);

	//SHMS
	pSimcDelta->Fill(ct->fPSdelta, ct->fWeight);
	pSimcYtar->Fill(ct->fPSytar, ct->fWeight);
	pSimcYptar->Fill(ct->fPSyptar, ct->fWeight);
	pSimcXptar->Fill(ct->fPSxptar, ct->fWeight);


	if (target == "c")
	 {
	  hSimcYtarc->Fill(ct->fHSytar, ct->fWeight);
	  pSimcYtarc->Fill(ct->fPSytar, ct->fWeight);
	 }
        

        if (target == "h")
	 {
	   pSimcPm->GetXaxis()->SetRangeUser(-0.15,0.25);
           pSimcEm->GetXaxis()->SetRangeUser(-0.15,0.25);
           psimP = pP0*(1.+ (ct->fPSdelta)/100.);
           ssimtheta = sqrt(pow(ptheta + (ct->fPSyptar),2.0)+pow(ct->fPSxptar,2.0));//ptheta + (ct->fPSyptar);
           ssim_corsi = psimP - 2.*mass_p*ebeam*cos(ssimtheta)/(ebeam+mass_p)/(1-pow(ebeam*cos(ssimtheta)/(ebeam+mass_p),2));        
           sSimcorsi->Fill(ssim_corsi,ct->fWeight);
           hSimcorsi->Fill(ct->fHcorsi,ct->fWeight);
         
         }
	//------ Kinematic quanties ---------
	hSimcW->Fill(ct->fW, ct->fWeight);
	pSimcEm->Fill(ct->fEm, ct->fWeight);
	pSimcPm->Fill(fabs(ct->fPm), ct->fWeight);
	pSimcPmz->Fill(ct->fPmz, ct->fWeight);	
    }

    TText *textSimc = new TText();
    textSimc->SetTextColor(kRed);
    TText *textData = new TText();
    textData->SetTextColor(kBlue);
    
    TCanvas *c1 = new TCanvas("HMS","HMS");
    c1->Divide(2,2);

    c1->cd(1);
    hDelta->DrawNormalized("prof");
    hSimcDelta->DrawNormalized("same");
    // textSimc->DrawText(8.0,0.05,"Simc");
    // textData->DrawText(8.0,0.045,"Data");
    c1->cd(2);
    hXptar->DrawNormalized("prof");
    hSimcXptar->DrawNormalized("same");
    c1->cd(3);
    hYptar->DrawNormalized("prof");
    hSimcYptar->DrawNormalized("same");
    c1->cd(4);
    if(target == "c")       // For carbon-12
     {
        hSimcYtarc->DrawNormalized();
        hYtarc->DrawNormalized("prof same");
     }
    else if(target == "h")  // For hydrogen-1
     {
        hSimcYtar->DrawNormalized();
        hYtar->DrawNormalized("prof same");
     }

    c1->Update();
    c1->Draw();
    c1->Write();
    fout->Save();
    c1->Print("Report.pdf(","pdf");

    TCanvas *c2 = new TCanvas("SHMS","SHMS");
    c2->Divide(2,2);

    c2->cd(1);
    pDelta->DrawNormalized("prof");
    pSimcDelta->DrawNormalized("same");
    // textSimc->DrawText(8.0,0.05,"Simc");
    // textData->DrawText(8.0,0.045,"Data");
    pDeltaCorr->DrawNormalized("prof same");
    c2->cd(2);
    pXptar->DrawNormalized("prof");
    pSimcXptar->DrawNormalized("same");
    c2->cd(3);
    pYptar->DrawNormalized("prof");
    pSimcYptar->DrawNormalized("same");
    pYptarCorr->DrawNormalized("prof same");
    c2->cd(4);
    if(target == "c")       // For carbon-12
     {
        pSimcYtarc->DrawNormalized();
        pYtarc->DrawNormalized("prof same");
     }
    else if(target == "h")  // For hydrogen-1
     {
        pSimcYtar->DrawNormalized();
        pYtar->DrawNormalized("prof same");
     }

    c2->Update();
    c2->Draw();
    c2->Write();
    fout->Save();
    c2->Print("Report.pdf","pdf");


    TCanvas *c3 = new TCanvas("Kinematic","Kinematic");
    c3->Divide(2,2);
    c3->cd(1);
    //    hW->Fit("gaus");
    hSimcW->DrawNormalized("hist");
    hW->DrawNormalized("hist same");

    // textSimc->DrawText(1.6,0.07,"Simc");
    // textData->DrawText(1.6,0.065,"Data");
    c3->cd(2);
    pSimcEm->DrawNormalized();
    pEm->DrawNormalized("prof same");
    pEmOpt->DrawNormalized("prof same");
    c3->cd(3);
    pPm->DrawNormalized("prof");
    pSimcPm->DrawNormalized("same");
    c3->cd(4);
    pSimcPmz->DrawNormalized();
    pPmz->DrawNormalized("prof same");

    c3->Update();
    c3->Draw();
    c3->Write();
    fout->Save();
    c3->Print("Report.pdf","pdf");

    if(target == "h")  // For hydrogen-1
     {
       TCanvas *c4 = new TCanvas("Singles","Singles");
       c4->Divide(2,2);

       c4->cd(1);
       hSimcorsi->DrawNormalized("L");
       hcorsi->DrawNormalized("prof same");//
    // textSimc->DrawText(8.0,0.05,"Simc");
    // textData->DrawText(8.0,0.045,"Data");
       c4->cd(2);
       sSimcorsi->DrawNormalized("L");//
       scorsi->DrawNormalized("prof same");
       sSimcorsiOpt->DrawNormalized("prof same");
       sSimcorsiOpt2->DrawNormalized("prof same");
       c4->cd(3);
       sSimcorsiVyptarOpt->Draw("colz");
       c4->cd(4);
       sSimcorsiVyptarOpt2->Draw("colz");
      
       c4->Update();
       c4->Draw();
       c4->Write();
       fout->Save();
       c4->Print("Report.pdf","pdf");

     }

    TCanvas *c5 = new TCanvas("SHMS_fp","SHMS_fp");
    c5->Divide(3,1);
    c5->cd(1);
    pxxpfp->Draw("COLZ");
    c5->cd(2);
    pxxpfp3->Draw("COLZ");
    c5->cd(3);
    pxxpfp2->Draw("COLZ");
    c5->Update();
    c5->Draw();
    c5->Write();
    fout->Save();
    c5->Print("Report.pdf","pdf");

    TCanvas *c6 = new TCanvas("Em2Ddists","Em2Ddist");
       c6->Divide(3,2);

       c6->cd(1);
       //scorsiEm->Draw("COLZ");
       emxpfp->Draw("COLZ");
       c6->cd(2);
       pPEm->Draw("COLZ");
       c6->cd(3);
       pthEm->Draw("COLZ");
       c6->cd(4);
       emdelta->Draw("COLZ");
       c6->cd(5);
       emxptar->Draw("COLZ");
       //              f1->Draw("same");
       c6->cd(6);
       emyptar->Draw("COLZ");
       c6->Update();
       c6->Draw();
       c6->Write();
       fout->Save();
       c6->Print("Report.pdf","pdf");


 TCanvas *c7 = new TCanvas("Em2DdistsCorr","Em2DdistCorr");
       c7->Divide(3,2);

       c7->cd(1);
       //scorsiEm->Draw("COLZ");
       emxpfpcorr->Draw("COLZ");
       c7->cd(2);
       pPEmcorr->Draw("COLZ");
       c7->cd(3);
       pthEmcorr->Draw("COLZ");
       c7->cd(4);
       emdeltacorr->Draw("COLZ");
       c7->cd(5);
       emxptarcorr->Draw("COLZ");
       //              f1->Draw("same");
       c7->cd(6);
       emyptarcorr->Draw("COLZ");
       c7->Update();
       c7->Draw();
       c7->Write();
       fout->Save();
       c7->Print("Report.pdf","pdf");


       TCanvas *c8 = new TCanvas("Em2DdistsFP","Em2DdistsFP");
       c8->Divide(4,2);
       c8->cd(1);
       h2_xfp->Draw("colz");
       c8->cd(2);
       h2_yfp->Draw("colz");
       c8->cd(3);
       h2_xpfp->Draw("colz");
       c8->cd(4);
       h2_ypfp->Draw("colz");
       c8->cd(5);
       h2_xfpCorr->Draw("colz");
       c8->cd(6);
       h2_yfpCorr->Draw("colz");
       c8->cd(7);
       h2_xpfpCorr->Draw("colz");
       c8->cd(8);
       h2_ypfpCorr->Draw("colz");
       c8->Update();
       c8->Draw();
       c8->Write();
       fout->Save();
       c8->Print("Report.pdf","pdf");

       TCanvas *c9 = new TCanvas("AngleComp","AngleComp");
       c9->Divide(3,2);
       c9->cd(1);
       h2_sinDiff0->Draw("colz");
       c9->cd(2);
       h2_sinDiff_d->Draw("colz");
       c9->cd(3);
       h2_sinDiff_dyp->Draw("colz");
       c9->cd(4);
       h2_xpDiff->Draw("colz");
       c9->cd(5);
       h2_ytarDiff->Draw("colz");
       c9->cd(6);
       h2_ytarDiffCorr->Draw("colz");

       c9->Update();
       c9->Draw();
       c9->Write();
       fout->Save();
       c9->Print("Report.pdf)","pdf");


       fout->Write();
       fout->Close();
}
