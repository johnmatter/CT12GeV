// Filename: Plot_Data_Simc_eSHMS.h
// Description: Plot, delta, xptar and yptar distributions for SHMS and HMS to compare between data and SIMC
// enter runumber and whether e or p in SHMS 

#include <TString.h>
void Plot_Data_Simc(Int_t runNumber, TString shms_part = "p", TString simfile = "h1_1.root", Double_t normfac = 1.0);

