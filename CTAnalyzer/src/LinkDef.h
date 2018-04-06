#ifdef __CINT__

#pragma link C++ class CTRun;                        // Main CT Run Class
#pragma link C++ function Help;                      // Help function
#pragma link C++ function CTkinDist;                 // Make some Kinematic distribution plots by looping over event by event
#pragma link C++ function CTkinDistRun;              // Make some Kinematic distribution plots by looping over event by event using CTRun class
#pragma link C++ function CTpidCut;                  // Make some Kinematic distribution plots by looping over event by event with cut
#pragma link C++ function CTcoinTime;                // Calculate coincidence time
#pragma link C++ function CTmissEngCorr;             // Missing Energy Correlation
#pragma link C++ function CTdemoDraw;                // Demo Draw Using CTRun class
#pragma link C++ function CTdemoLoop;                // Loop over events Using CTRun class
#pragma link C++ function CTdataVsSimc;              // Compare Reconstructed and Kinematic Quantities from data and Simc
#pragma link C++ function CTexample;                 // quick plotter example
#pragma link C++ function CTgrass;                   // test grass
#pragma link C++ function CTtrkingEffi;              // Calculate Tracking Efficiency           
#pragma link C++ function CTdataCorrelations;        // 2D plots of different quantities against Em            
#pragma link C++ function CTWCorrelations;           // 2D plots of different quantities against W           
#pragma link C++ function CTcoinTimeReport;          // Calculate coincidence time  with different correlation
#pragma link C++ function CTcoinTimeRaw;             // Calculate raw coincidence time
#pragma link C++ function CTEmTest;                  // Calculate Em correlations
#pragma link C++ function CTEmTest_h;                  // Calculate Em correlations
#pragma link C++ function CT2run;                  // Campare 2 runs


#pragma link C++ function Plot_Data_Simc_eSHMS;      // Compare SIMC with data e in SHMS
#pragma link C++ function Plot_Data_Simc_pSHMS;      // Compare SIMC with data p in SHMS
#pragma link C++ function Plot_Data_Simc;            // Compare SIMC with data for either e or p
#pragma link C++ function Plot_kine_compare_eSHMS;   // Compare kinematics for SIMC with data e in SHMS
#pragma link C++ function Plot_kine_compare_pSHMS;   // Compare kinematics for SIMC with data e in SHMS
#pragma link C++ function Plot_kine_compare;         // Compare kinematics for SIMC with data for either e or p
#pragma link C++ function Plot_kine_compare_carbon;  // Compare kinematics for Carbon target SIMC with data for either e or p






#endif
