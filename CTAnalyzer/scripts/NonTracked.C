

void NonTracked()
{
    TFile *f = new TFile("/work/hallc/e1206107/CT12GeV/ct_replay/ROOTfiles/shms_coin_replay_production_2284_-1.root");
    TTree *T = (TTree*)f->Get("T");
    Int_t nEntries = T->GetEntries();
    cout <<"Tracked entries: "<<nEntries <<endl;

    TH1D *hist = new TH1D("tempHist","tempHist",13,0.5,13.5);

    T->Draw("P.hod.1x.negAdcCounter>>tempHist","","goff");
    hist = (TH1D*)gDirectory->Get("tempHist");
    Int_t nEntries_nonTracked = hist->GetEntries();
    cout << "Tracked + Untracked entries: " <<nEntries_nonTracked <<endl;

    TH1D *negADC = new TH1D("negADC","negADC", 13, 0.5, 13.5);
    Double_t adc_counter;
    //T->SetBranchAddress("P.hod.1x.negAdcCounter", &adc_counter);  //<---------- This approach fails

    for(Int_t event = 0; event < nEntries; ++event)
    {
      T->GetEntry(event);
	
	adc_counter = T->GetLeaf("P.hod.1x.negAdcCounter")->GetValue();    // <----- Alternative approach
        if (event > 7680 && event < 7750)cout << event <<" event is = " <<adc_counter << endl;
	negADC->Fill(adc_counter);
    }

    negADC->Draw();
}
