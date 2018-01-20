#ifdef __CINT__

#pragma link C++ class CTRun;                        // Main CT Run Class
#pragma link C++ function Help;                      // Help function
#pragma link C++ function CTkinDist;                 // Make some Kinematic distribution plots by looping over event by event
#pragma link C++ function CTkinDistRun;              // Make some Kinematic distribution plots by looping over event by event
#pragma link C++ function CTpidCut;                  // Make some Kinematic distribution plots by looping over event by event with cut
#pragma link C++ function CTcoinTime;                // Calculate coincidence time

#pragma link C++ function Plot_Data_Simc_eSHMS;      // Compare SIMC with data e in SHMS
#pragma link C++ function Plot_Data_Simc_pSHMS;      // Compare SIMC with data p in SHMS
#pragma link C++ function Plot_Data_Simc;            // Compare SIMC with data for either e or p
#pragma link C++ function Plot_kine_compare_eSHMS;   // Compare kinematics for SIMC with data e in SHMS
#pragma link C++ function Plot_kine_compare_pSHMS;   // Compare kinematics for SIMC with data e in SHMS
#pragma link C++ function Plot_kine_compare;         // Compare kinematics for SIMC with data for either e or p

#endif
