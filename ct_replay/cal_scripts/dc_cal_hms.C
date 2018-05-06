#include <iostream>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <TSystem.h>
#include <TString.h>
#include <TCut.h>
#include <TFile.h>
#include <TH1F.h>
#include <TChain.h>
#include <TTree.h>
#include <TF1.h>

void dc_cal_hms(Int_t runNo, Int_t eventNo)
{

 TFile *f =  new TFile(Form("/volatile/hallc/comm2017/e1206107/ROOTfiles/detector_calibration/hms_replay_production_all_%d_%d.root", runNo,eventNo));
  TTree *t = (TTree*)f->Get("T");
  
  TFile *output = new TFile(Form("DD&DT_%d_%d.root", runNo, eventNo), "RECREATE");

  TH1F * drift_dist[12];
  TH1F * drift_time[12];

  string plane[12] = {"1u1", "1u2", "1x1", "1x2", "1v1", "1v2", "2v2", "2v1", "2x2", "2x1", "2u2", "2u1"};
 
  double slope [12];
  double slopeErr [12];
  double planes [12];

  TCut time_nhit;
  TCut nhit;
  
  for (int ip = 0; ip<12; ip++)
    {
      time_nhit =  Form("H.dc.%s.time>0.0&&H.dc.%s.time<190.0&&H.dc.%s.nhit==1", plane[ip].c_str(),plane[ip].c_str(),plane[ip].c_str());
 
      drift_dist[ip] = new TH1F(Form("drift_dist_%s", plane[ip].c_str()),  "", 100, -0.01, 0.6 );
      drift_time[ip] = new TH1F(Form("drift_time_%s", plane[ip].c_str()),  "", 100, -0.01, 300 );


      t->Draw(Form("H.dc.%s.dist>>drift_dist_%s", plane[ip].c_str(), plane[ip].c_str()), time_nhit);
      t->Draw(Form("H.dc.%s.time>>drift_time_%s", plane[ip].c_str(), plane[ip].c_str()));

     
      TF1 *myfunc = new TF1 ("myfunc", "[0]+x*[1]", 0.01, 0.49);
      
      drift_dist[ip]->Fit("myfunc","R");
      slope[ip] = myfunc->GetParameter(1);
      slopeErr[ip]= myfunc->GetParError(1);
    
      planes [ip] = ip+1;                
    }

  TCanvas *c1 = new TCanvas("c1", "HMS Drift Distance", 1000, 1000);
  c1->Divide(2,3);
  for(int j=0; j<6; j++){
    c1->cd(j+1);
    drift_dist[j]->Draw();
    c1->SaveAs("drift_dist_hms.pdf");
   
  }
  /*
  TCanvas *c2 = new TCanvas("c2", "HMS Drift Distance", 1000, 1000);
  c1->Divide(2,3);
  for(int j=6; j<12; j++){
    c1->cd(j+1);
    drift_dist[j]->Draw();
    c1->SaveAs("drift_dist_hms2.pdf");
   
  }
   TCanvas *c3 = new TCanvas("c3", "HMS Drift Time", 1000, 1000);
  c2->Divide(2,3); 
  for(int j=0; j<6; j++){
    c2->cd(j+1);
    drift_time[j]->Draw();
    c2->SaveAs("drift_time_hms.pdf");

   
    }*/
     output->Write();
 
}
