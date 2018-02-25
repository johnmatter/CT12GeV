void ytar_test()
{
    TString fileName1 = "/volatile/hallc/comm2017/e1206107/ROOTfiles/";
    fileName1 += "coin_replay_production_2282_-1.root";

    Bool_t runNotFound = gSystem->AccessPathName(fileName1);
    if(runNotFound)
    {	    
	cout << "Requested file " << fileName1 << " does NOT exist" <<endl;
	return;
    }
    
    TFile *f1 = new TFile(fileName1);
    TTree *t1 = (TTree*)f1->Get("T");

    TString fileName2 = "/volatile/hallc/comm2017/e1206107/ROOTfiles/";
    fileName2 += "coin_replay_production_2404_-1.root";

    Bool_t runNotFound2 = gSystem->AccessPathName(fileName2);
    if(runNotFound)
    {	    
	cout << "Requested file " << fileName2 << " does NOT exist" <<endl;
	return;
    }
    
    TFile *f2 = new TFile(fileName2);
    TTree *t2 = (TTree*)f2->Get("T");
    
    //tree->Print();

    TH1D *Hytar1 = new TH1D("Hytar1", "HMS Y-tar (cm); ", 200, -15, 15);
    TH1D *Hytar2 = new TH1D("Hytar2", "HMS Y-tar (cm); ", 200, -15, 15);


    Double_t HSytar1, HSytar2;
          
    t1->SetBranchAddress("H.gtr.y", &HSytar1);    
    t2->SetBranchAddress("H.gtr.y", &HSytar2);    
    
    

    Int_t nEvents1 = t1->GetEntries();

    for (Int_t event = 0; event < nEvents1; ++ event)
    {
	t1->GetEntry(event);
        Hytar1->Fill(HSytar1); 
    }

    Int_t nEvents2 = t2->GetEntries();

    for (Int_t event2 = 0; event2 < nEvents2; ++ event2)
    {
	t2->GetEntry(event2);
        Hytar2->Fill(HSytar2); 
    }

    TCanvas *c1 = new TCanvas("c1","c1"); 
    Hytar1->SetMarkerColor(kRed);
    Hytar1->SetLineColor(kRed);
    Hytar1->GetXaxis()->SetTitle("HMS Y-tar (cm)");
    Hytar1->DrawNormalized("prof");
   
    Hytar2->SetLineColor(kBlue);    
    Hytar2->SetMarkerColor(kBlue);
    Hytar2->DrawNormalized("same");
    TText *xlabel = new TText();
    xlabel -> SetTextFont(1);
    xlabel -> SetTextColor(1);
    xlabel -> SetTextSize(0.05);
    xlabel -> DrawText(10.5,-0.0035,"HMS y-tar (cm)");

    c1->SaveAs("ytar2282.pdf");

}
