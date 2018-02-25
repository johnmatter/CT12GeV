void wfit()
{
    TString fileName2 = "/volatile/hallc/comm2017/e1206107/ROOTfiles/";
    fileName2 += "coin_replay_production_2397_-1.root";

    Bool_t runNotFound2 = gSystem->AccessPathName(fileName2);
    if(runNotFound2)
    {	    
	cout << "Requested file " << fileName2 << " does NOT exist" <<endl;
	return;
    }
    
    TFile *f2 = new TFile(fileName2);
    TTree *t2 = (TTree*)f2->Get("T");
    
    //tree->Print();

    TH1D *Hsw = new TH1D("Hsw", "W (GeV); ", 200, 0.0, 10.0);
 

    Double_t HSW1;
          

    t2->SetBranchAddress("H.kin.primary.W", &HSW1);    
    
    


    Int_t nEvents2 = t2->GetEntries();

    for (Int_t event2 = 0; event2 < nEvents2; ++ event2)
    {
	t2->GetEntry(event2);
        Hsw->Fill(HSW1); 
    }
    gStyle->SetOptStat(1);
    gStyle->SetOptFit(1);

    TCanvas *c1 = new TCanvas("c1","c1"); 
    Hsw->SetMarkerColor(kRed);
    Hsw->SetLineColor(kRed);
    Hsw->GetXaxis()->SetTitle("W (GeV)");
    Hsw->Draw("prof");
    Hsw->Fit("gaus");    

    c1->SaveAs("w2404.pdf");

}
