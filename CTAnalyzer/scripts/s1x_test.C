void s1x_test()
{
    TString fileName = "/volatile/hallc/comm2017/e1206107/ROOTfiles/";
    fileName += "coin_replay_production_2294_-1.root";

    Bool_t runNotFound = gSystem->AccessPathName(fileName);
    if(runNotFound)
    {	    
	cout << "Requested file " << fileName << " does NOT exist" <<endl;
	return;
    }
    
    TFile *file = new TFile(fileName);
    TTree *tree = (TTree*)file->Get("TSP");    
    //tree->Print();

    TH1D *PS1xplus = new TH1D("PS1X_rate", "PS1X_rate; ", 500, 600000, 1500000);
    TH1D *PS1xminus = new TH1D("PS1xminus", "PS1xminus; ", 500, 600000, 1500000);
    TH1D *PS1X = new TH1D("PS1X", "PS1X; ", 500, 600000, 1500000);

    Double_t P1x1p, P1x2p, P1x3p, P1x4p, P1x5p, P1x6p, P1x7p, P1x8p, P1x9p, P1x10p, P1x11p, P1x12p, P1x13p;    
    Double_t P1x1n, P1x2n, P1x3n, P1x4n, P1x5n, P1x6n, P1x7n, P1x8n, P1x9n, P1x10n, P1x11n, P1x12n, P1x13n;
    Double_t P1xs;    
          
    tree->SetBranchAddress("P.hod.1x1.posScalerRate", &P1x1p);    
    tree->SetBranchAddress("P.hod.1x2.posScalerRate", &P1x2p);    
    tree->SetBranchAddress("P.hod.1x3.posScalerRate", &P1x3p);    
    tree->SetBranchAddress("P.hod.1x4.posScalerRate", &P1x4p);    
    tree->SetBranchAddress("P.hod.1x5.posScalerRate", &P1x5p);    
    tree->SetBranchAddress("P.hod.1x6.posScalerRate", &P1x6p);    
    tree->SetBranchAddress("P.hod.1x7.posScalerRate", &P1x7p);    
    tree->SetBranchAddress("P.hod.1x8.posScalerRate", &P1x8p);    
    tree->SetBranchAddress("P.hod.1x9.posScalerRate", &P1x9p);    
    tree->SetBranchAddress("P.hod.1x10.posScalerRate", &P1x10p);    
    tree->SetBranchAddress("P.hod.1x11.posScalerRate", &P1x11p);    
    tree->SetBranchAddress("P.hod.1x12.posScalerRate", &P1x12p);    
    tree->SetBranchAddress("P.hod.1x13.posScalerRate", &P1x13p);    
    tree->SetBranchAddress("P.hod.1x1.negScalerRate", &P1x1n);    
    tree->SetBranchAddress("P.hod.1x2.negScalerRate", &P1x2n);    
    tree->SetBranchAddress("P.hod.1x3.negScalerRate", &P1x3n);    
    tree->SetBranchAddress("P.hod.1x4.negScalerRate", &P1x4n);    
    tree->SetBranchAddress("P.hod.1x5.negScalerRate", &P1x5n);    
    tree->SetBranchAddress("P.hod.1x6.negScalerRate", &P1x6n);    
    tree->SetBranchAddress("P.hod.1x7.negScalerRate", &P1x7n);    
    tree->SetBranchAddress("P.hod.1x8.negScalerRate", &P1x8n);    
    tree->SetBranchAddress("P.hod.1x9.negScalerRate", &P1x9n);    
    tree->SetBranchAddress("P.hod.1x10.negScalerRate", &P1x10n);    
    tree->SetBranchAddress("P.hod.1x11.negScalerRate", &P1x11n);    
    tree->SetBranchAddress("P.hod.1x12.negScalerRate", &P1x12n);    
    tree->SetBranchAddress("P.hod.1x13.negScalerRate", &P1x13n);    
    tree->SetBranchAddress("P.S1X.scalerRate", &P1xs);    

    

    Int_t nEvents = tree->GetEntries();

    for (Int_t event = 0; event < nEvents; ++ event)
    {
	tree->GetEntry(event);

	PS1xplus->Fill(P1x1p+P1x2p+P1x3p+P1x4p+P1x5p+P1x6p+P1x7p+P1x8p+P1x9p+P1x10p+P1x11p+P1x12p+P1x13p);
	PS1xminus->Fill(P1x1n+P1x2n+P1x3n+P1x4n+P1x5n+P1x6n+P1x7n+P1x8n+P1x9n+P1x10n+P1x11n+P1x12n+P1x13n);
	PS1X->Fill(P1xs);
    }

    TCanvas *c1 = new TCanvas("c1","c1"); 
    PS1xplus->GetXaxis()->SetTitle("S1X Scaler Rates (Hz)");
    PS1xplus->Draw();
    PS1xplus->SetLineColor(kRed);    
    PS1xminus->Draw("same");
    PS1xplus->SetLineColor(kBlue);    
    PS1X->Draw("same");
    PS1X->SetLineColor(kGreen);    
    TText *xlabel = new TText();
    xlabel -> SetTextFont(1);
    xlabel -> SetTextColor(1);
    xlabel -> SetTextSize(0.03);
    xlabel -> DrawText(1050000,160,"S1X+");
    xlabel -> SetTextColor(4);
    xlabel -> SetTextSize(0.03);
    xlabel -> DrawText(950000,160,"S1X-");
    xlabel -> SetTextColor(3);
    xlabel -> SetTextSize(0.03);
    xlabel -> DrawText(880000,160,"S1X");

    c1->SaveAs("s1x.pdf");

}
