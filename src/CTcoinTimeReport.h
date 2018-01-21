// Filename:  CTcoinTimeReport.h
// Description: Calculate corrected coincidence time and make comparison report
// Author: Latif Kabir < latif@jlab.org >
// Created: Thu Jan  4 17:16:45 2018 (-0500)
// URL: jlab.org/~latif

#include <TString.h>
void CTcoinTimeReport(Int_t runNumber, TString shms_particle = "p", Int_t analyzedEvents = -1);
