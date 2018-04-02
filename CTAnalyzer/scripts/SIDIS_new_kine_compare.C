void SIDIS_new_kine_compare(TString shms_part, TString simfile, Double_t normfac){


  //  const Int_t runnumber[numruns]={2023, 2046, 2047, 2048, 2050, 2051, 2052, 2054, 2066};

  //  const Int_t runnumber[numruns]={2278, 2279, 2280};
  //  const Int_t runnumber[numruns]={2406,2407,2408,2409,2410,2411};
  const Int_t numruns = 1;
  const Int_t runnumber[numruns]={2050};                     

  TString fileNameM = "/work/hallc/e1206107/CT12GeV/ct_replay/ROOTfiles/";
  fileNameM += simfile; //read the root file from SIMC
  TFile *f2 = new TFile(fileNameM);
  TTree *ts = (TTree*)f2->Get("h666");
  //get the relevant branch
  int nentriesM = ts->GetEntries();
  cout<<"Simc Entries:\t"<<nentriesM<<endl;
  
  TFile *fout = new TFile("run_sidis_kine.pdf","RECREATE");
  TString filecO = "sidis_kine.pdf";
  TString filecO1 = "sidis_kine.pdf[";
  TString filecO2 = "sidis_kine.pdf]";

  gROOT->SetBatch(kTRUE);
  gROOT->ForceStyle(); 
  gStyle->SetPalette(kOcean);

  Double_t pionMass = 0.1395701835;    //Gev/c^2       //added
  Double_t z_had;                                         //added
  Double_t E_pi;                                    //added
  Double_t P_t2;   //Transverse Mom Pt  //ad
  
  //make histograms:
  THStack *hs1 = new THStack("hs1","W (GeV)");
  THStack *hs2 = new THStack("hs2","Q2 (GeV2)");
  THStack *hs3 = new THStack("hs3","x_bj");
  THStack *hs4 = new THStack("hs4","z_had");
  THStack *hs5 = new THStack("hs5","P_t2");    //added

   TH1D *h_Q2d  = new TH1D("h_Q2d","Q2 (GeV2)",100,0,15);
   TH1D *h_Q2s  = new TH1D("h_Q2s","Q2 (GeV2",100,0,15);
   TH1D *h_Wd   = new TH1D("h_Wdh","W (GeV)", 150, 0.0, 2.0);
   TH1D *h_Ws   = new TH1D("h_Wsh","W (GeV)", 150, 0.0, 2.0);
   TH1D *h_xd   = new TH1D("h_xd","X_bj", 100,0, 1);   //added
   TH1D *h_xs   = new TH1D("h_xs","X_bj", 100,0, 2);    //added see range should be 1
   TH1D *h_zd   = new TH1D("h_zd","z_had", 100,0, 2);   //added see range
   TH1D *h_zs   = new TH1D("h_zs","z_had", 100,0, 1);   //added
   TH1D * h_Pt2d = new TH1D("h_Pt2d","h_Pt2d", 100,0,100); //ad
   TH1D * h_Pt2s = new TH1D("h_Pt2s","h_Pt2s", 100,0,100); //ad
   TH1D *s_tmp  = new TH1D("h_s_tmp","stmp", 100,-20, 20);


  TChain *tt = new TChain("T");

  for (int i=0;i<numruns;i++){
    cout << "opening run number: " <<  runnumber[i] <<"\n";
 //read the input file from data
    TString fileNameD = "/work/hallc/e1206107/CT12GeV/ct_replay/ROOTfiles/";
    fileNameD += "coin_replay_production_"; //read the root file from data
    fileNameD += runnumber[i]; //read the root file from data
    fileNameD += "_-1.root"; //read the root file from data

    tt->Add(fileNameD);
  }
  //  TFile *f1 = new TFile(fileNameD);
  //  TTree *tt = (TTree*)f1->Get("T");
    //get the relevant branch
   int nentriesD = tt->GetEntries();
   cout<<"Entries:\t"<<nentriesD<<endl;

  Double_t HgtrX, HgtrTh, HgtrY, HgtrPh, hdelta, pdelta;// Ep, Epp, p_e, p_p, defEm;
  Double_t HgtrBetaCalc, PgtrBetaCalc, evtType, PgtrP, HgtrP, PhodStatus, PhodStartTime, PhodfpHitsTime;
  Double_t cointime, HhodStatus, HhodStartTime, HhodfpHitsTime, SHMSpartMass, HMSpartMass;
  Double_t pkinW, pkinomega, pkinQ2, pkinxbj, pEm, pPm, pbeta, hbeta, hcalepr, hcaletot, hcernpe, pcaletot, pcalepr, pcernpe;
  Double_t pt2,pkinthq;                //ad
    // Foal plane coords
    Double_t PdcXfp;
    Double_t PdcXpfp;
    Double_t PdcYfp;
    Double_t PdcYpfp;

    Double_t HdcXfp;
    Double_t HdcXpfp;
    Double_t HdcYfp;
    Double_t HdcYpfp;


  int cnts=0;
  TCut hpdelta;
  TCut evtCut;                                                                   
  TCut eBetaCut;                                                                 
  TCut pBetaCut;                                                                 
  TCut cerCut;                                                                   
  TCut calCut;                                                                   
  TCut hodoTimeCut;
  TCut Wcut;

  tt->SetBranchAddress("P.gtr.p", &PgtrP); 
  tt->SetBranchAddress("H.gtr.p", &HgtrP); 
  tt->SetBranchAddress("P.gtr.beta", &pbeta);                                                                                 
  tt->SetBranchAddress("H.gtr.beta", &hbeta); 
  tt->SetBranchAddress("H.gtr.dp", &hdelta);                                                                                
  tt->SetBranchAddress("P.gtr.dp", &pdelta);                                                                                
                                                                                                                            
  tt->SetBranchAddress("P.cal.eprtracknorm", &pcalepr);                                                                    
  tt->SetBranchAddress("P.cal.etottracknorm", &pcaletot);                                                                  
  tt->SetBranchAddress("P.hgcer.npeSum", &pcernpe);                                                 
                                                                                                             
  tt->SetBranchAddress("H.cal.eprtracknorm", &hcalepr);                                            
  tt->SetBranchAddress("H.cal.etottracknorm", &hcaletot);                                          
  tt->SetBranchAddress("H.cer.npeSum", &hcernpe); 
  tt->SetBranchAddress("H.kin.primary.W", &pkinW);
  tt->SetBranchAddress("H.kin.primary.omega", &pkinomega);                        
  tt->SetBranchAddress("H.kin.primary.Q2", &pkinQ2);
  tt->SetBranchAddress("H.kin.primary.x_bj", &pkinxbj);
  tt->SetBranchAddress("H.kin.primary.th_q", &pkinthq);  //ad
  
                                                  
  tt->SetBranchAddress("P.kin.secondary.emiss", &pEm);                                                    
  tt->SetBranchAddress("P.kin.secondary.pmiss", &pPm);                                                    
                                                                                                              
  tt->SetBranchAddress("P.hod.goodstarttime", &PhodStatus);                                              
  tt->SetBranchAddress("P.hod.starttime", &PhodStartTime);                                               
  tt->SetBranchAddress("P.hod.fpHitsTime", &PhodfpHitsTime);                                             
                                                                                                             
  tt->SetBranchAddress("H.hod.goodstarttime", &HhodStatus);                                              
  tt->SetBranchAddress("H.hod.starttime", &HhodStartTime);                                               
  tt->SetBranchAddress("H.hod.fpHitsTime", &HhodfpHitsTime);                                             
                                                                                                             
  tt->SetBranchAddress("H.gtr.x", &HgtrX);                                                               
  tt->SetBranchAddress("H.gtr.th", &HgtrTh);                                                             
  tt->SetBranchAddress("H.gtr.y", &HgtrY);                                                               
  tt->SetBranchAddress("H.gtr.ph", &HgtrPh);                                                             

    
    tt->SetBranchAddress("H.dc.x_fp", &HdcXfp);
    tt->SetBranchAddress("H.dc.xp_fp", &HdcXpfp);
    tt->SetBranchAddress("H.dc.y_fp", &HdcYfp);
    tt->SetBranchAddress("H.dc.yp_fp", &HdcYpfp);    

    tt->SetBranchAddress("P.dc.x_fp", &PdcXfp);
    tt->SetBranchAddress("P.dc.xp_fp", &PdcXpfp);
    tt->SetBranchAddress("P.dc.y_fp", &PdcYfp);
    tt->SetBranchAddress("P.dc.yp_fp", &PdcYpfp);    

                                                                                                             
  Double_t Em_mean, W_mean;
  Double_t Pm_mean;
  
   if(shms_part == "e")                                                      
    {                                                                              
      SHMSpartMass = 0.000510998; // electron mass in GeV/c^2                
      HMSpartMass = 0.93827208; // proton mass in GeV/c^2                  
      hpdelta = "P.gtr.dp > -10 && P.gtr.dp < 10 && H.gtr.dp > -8 && H.gtr.dp < 8";
      eBetaCut = "P.gtr.beta > 0.5 && P.gtr.beta < 1.5";                         
      pBetaCut = "H.gtr.beta > 0.5 &&  H.gtr.beta < 1.5";                        
      cerCut = "P.hgcer.npeSum > 0.1 && H.cer.npeSum < 0.1";                      
      calCut = "P.cal.etottracknorm > 0.8 && P.cal.etottracknorm < 1.5 && P.cal.eprtracknorm  > 0.2";        
      hodoTimeCut ="P.hod.goodstarttime == 1 && H.hod.goodstarttime == 1";
      Wcut = "H.kin.primary.W >0.5 && H.kin.primary.W < 1.5";
 
    }                                                                              
   else                                                                          
    {                                                                             
      SHMSpartMass = 0.93827208; // proton mass in GeV/c^2                 
      HMSpartMass = 0.000510998; // electron mass in GeV/c^2                 
      hpdelta = "P.gtr.dp > -10 && P.gtr.dp < 10 && H.gtr.dp > -8 && H.gtr.dp < 8";
      eBetaCut = "P.gtr.beta > 0.8 && P.gtr.beta < 1.3";                        
      pBetaCut = "H.gtr.beta > 0.8 &&  H.gtr.beta < 1.3";                       
      cerCut = "P.hgcer.npeSum < 0.1 && H.cer.npeSum > 0.5";                    
      calCut = "H.cal.etottracknorm > 0.8 && H.cal.etottracknorm < 1.5 && H.cal.eprtracknorm  > 0.2";         
      hodoTimeCut ="P.hod.goodstarttime == 1 && H.hod.goodstarttime == 1";      
      Wcut = "H.kin.primary.W >0.5 && H.kin.primary.W < 1.5";
    }                                                                             
 
  for (int kk=0; kk<nentriesD; kk++){
    tt->GetEntry(kk);
    if (kk % 10000 == 0) cout << kk*100/nentriesD << "   % of data done" << endl;
      evtType = tt->GetLeaf("fEvtHdr.fEvtType")->GetValue(); 
    if (shms_part == "e") 
    {
      if (pbeta>0.8 && pbeta<1.3 && hbeta>0.8 && hbeta<1.3 && pcernpe>0.1 && hcernpe<0.1 && pcaletot >0.8 && pcaletot<1.5 && pcalepr>0.1 && PhodStatus == 1 && HhodStatus ==1 && hdelta > -8 && hdelta < 8 && pdelta > -10 && pdelta < 10) 
      {
         cnts++;
         h_Q2d->Fill(pkinQ2);         
         h_Wd->Fill(pkinW);
         h_xd->Fill(pkinxbj);
         E_pi = sqrt(pow(pionMass,2) + pow(PgtrP,2));
         z_had = E_pi/pkinomega;
         h_zd->Fill(z_had);
         Pt2 = (((PgtrP)*(PgtrP))*(1-(H.kin.primary.th_q)*(H.kin.primary.th_q)));  //ad Transverse Mom
	 pt2->Fill(pt2);  //ad
      
      }
    }
    else if (shms_part == "p") 
    {
      if (pbeta>0.6 && pbeta<1.4 && hbeta>0.8 && hbeta<1.2 && pcernpe<0.1 && hcernpe>0. && hcaletot >0.6 && hcaletot<2.0 && PhodStatus == 1 && HhodStatus ==1 && hdelta > -10 && hdelta < 10 && pdelta > -12 && pdelta < 12 && pkinW > 0.75 && pkinW < 1.15)     { //cuts to select the electrons and protons
        cnts++;
         h_Q2d->Fill(pkinQ2);         
         h_Wd->Fill(pkinW);
         h_xd->Fill(pkinxbj);
        E_pi = sqrt(pow(pionMass,2) + pow(PgtrP,2));
        z_had = E_pi/pkinomega;
         h_zd->Fill(z_had);  
        Pt2 = (((PgtrP)*(PgtrP))*(1-(H.kin.primary.th_q)*(H.kin.primary.th_q)));  //ad
	pt2->Fill(pt2); //ad
           
      }
    }
  }
  h_Q2d->GetXaxis()->SetTitle("Q2 (GeV2)");           
  h_xd->GetXaxis()->SetTitle("x_bj");
  h_Wd->GetXaxis()->SetTitle("W (GeV)");      
  h_zd->GetXaxis()->SetTitle("z_had"); 
  pt2->GetXaxis()->SetTitle("p_transv_sq");     

  cout << cnts <<" events passed all cuts" << endl;

  double spdelta, sQ2, sxbj, sW, wt, sdelta, sfac, normwt, szhad, spt2; //ad spt2

  for (int kk=0; kk<nentriesM; kk++){
    ts->GetEntry(kk);
    if (kk % 50000 == 0) cout << kk*100/nentriesM << "   % of simc done" << endl;

    sdelta = ts->GetLeaf("hsdelta")->GetValue();
    wt = ts->GetLeaf("Weight")->GetValue();
    normwt = wt*normfac/nentriesM;
    s_tmp->Fill(sdelta,normwt);
  }
  double intgrl = s_tmp->Integral(1,100); // integral of the SIMC histogram

  sfac = cnts/intgrl;  // scale factor to make the intregral of both Data and SIMC histrograms are equal
  cout << "scalefac =" << sfac << endl;


  for (int kk=0; kk<nentriesM; kk++){
    ts->GetEntry(kk);
    if (kk % 50000 == 0) cout << kk*100/nentriesM << "   % of simc done" << endl;

    sdelta = ts->GetLeaf("hsdelta")->GetValue();
    spdelta = ts->GetLeaf("ssdelta")->GetValue();
    sQ2 = ts->GetLeaf("Q2")->GetValue();
    sxbj = ts->GetLeaf("xbj")->GetValue();
    szhad = ts->GetLeaf("z")->GetValue();
    spt2 = ts->GetLeaf("pt2")->GetValue();  //ad
    sW = ts->GetLeaf("W")->GetValue();
    wt = ts->GetLeaf("Weight")->GetValue();
    normwt = wt*normfac*sfac/nentriesM;
    if (sW > 0.75 && sW < 1.15 && sdelta < 10 && sdelta > -10 && spdelta > -12 && spdelta < 12 )
    {
     h_Q2s->Fill(sQ2,normwt);
     h_xs->Fill(sxbj,normwt);
     h_Ws->Fill(sW,normwt);
     h_zs->Fill(szhad,normwt);

    }
  }
  h_Q2s->GetXaxis()->SetTitle("Q2 (GeV2)");           
  h_xs->GetXaxis()->SetTitle("x_bj");           
  h_Ws->GetXaxis()->SetTitle("W (GeV)");           
  h_zs->GetXaxis()->SetTitle("z_had");           
  
  //  hs2->GetXaxis()->SetTitle("Missing energy (GeV)");           
  //hs2->GetXaxis()->SetTitle("Missing momentum (GeV/c)");           
  //hs1->GetXaxis()->SetTitle("W (GeV)");           
  //hs4->GetXaxis()->SetTitle("Corrected Cointime (ns)");           


  TCanvas *cc = new TCanvas ("cc","cc",800,600);
  cc->SetGrid();
  cc->SetLeftMargin(0.2);
  cc->SetRightMargin(0.2);
  gStyle->SetOptStat(0);

  cc->Print(filecO1);
  cc->cd(1);
  h_Wd->SetFillColor(kBlue);
  h_Wd->SetMarkerColor(kBlue);
  h_Wd->SetLineColor(kBlue);
  hs1->Add(h_Wd);
  h_Ws->SetFillStyle(1001);
  h_Ws->SetMarkerColor(kRed);
  h_Ws->SetLineColor(kRed);
  hs1->Add(h_Ws);
  hs1->Draw("pfc nostack");

  cc->Print(filecO);

  cc->cd(1);
  h_Q2d->SetFillColor(kBlue);
  h_Q2d->SetMarkerColor(kBlue);
  h_Q2d->SetLineColor(kBlue);
  hs2->Add(h_Q2d);
  h_Q2s->SetFillStyle(1001);
  h_Q2s->SetLineColor(kRed);
  h_Q2s->SetMarkerColor(kRed);
  hs2->Add(h_Q2s);
  hs2->Draw("pfc nostack");
  cc->Print(filecO);
 
  cc->cd(1);
  h_xd->SetFillColor(kBlue);
  h_xd->SetMarkerColor(kBlue);
  h_xd->SetLineColor(kBlue);
  hs3->Add(h_xd);
  h_xs->SetFillStyle(1001);
  h_xs->SetMarkerColor(kRed);
  h_xs->SetLineColor(kRed);
  hs3->Add(h_xs);
  hs3->Draw("pfc nostack");
  cc->Print(filecO);
  
  cc->cd(1);
  h_zd->SetFillColor(kBlue);
  h_zd->SetMarkerColor(kBlue);
  h_zd->SetLineColor(kBlue);
  hs4->Add(h_zd);
  h_zs->SetFillStyle(1001);
  h_zs->SetMarkerColor(kRed);
  h_zs->SetLineColor(kRed);
  hs4->Add(h_zs);
  hs4->Draw("pfc nostack");

  cc->Print(filecO);

  cc->Print(filecO2);

  gROOT->SetBatch(kFALSE);
  gStyle->SetPaperSize(20,20);
  TCanvas *c1 = new TCanvas ("cc1","cc1",800,600);
  c1->SetGrid();
  c1->SetLeftMargin(0.2);
  c1->SetRightMargin(0.2);
  c1->Divide(2,2);

  c1->cd(1);
  hs1->Draw("pfc nostack");
  
  c1->cd(2);
  hs2->Draw("pfc nostack");
 
 
  c1->cd(3);
  hs3->Draw("pfc nostack");


  c1->cd(4);
  hs4->Draw("pfc nostack");


  c1->SaveAs("tmp1.pdf");

  fout->Write();
  //  fout->Close();

}
