

void momentum()
{
    TFile *f = new TFile("/work/hallc/e1206107/CT12GeV/ct_replay/ROOTfiles/coin_replay_production_2050_-1.root");
    TTree *T = (TTree*)f->Get("T");

    TH1D *hist = new TH1D("tempHist","tempHist",100,3,7);

    T->Draw("H.kin.primary.omega>>tempHist","");
}
