// Filename: Plot_kine_compare.cc
// Description: W, Em, Pm and Cointime distributions for SHMS and HMS to compare between data and SIMC (except for Pm_per).
// Needs runnumber, 
// Also needs normfac from SIMC and the simc root file name

#include <TString.h>
void Plot_kine_compare(Int_t runNumber, TString shms_part = "p", TString simfile = "h1_1.root", Double_t normfac = 1.0);

