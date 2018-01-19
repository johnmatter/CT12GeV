//Loading libCT automatically every time you start root.
//Author: Latiful kabir 
{
    cout << "\n\t\t=======================================================" <<endl;
    cout << "\t\t|  Welcome to CT Analysis Tools Integrated in ROOT    |"<<endl;
    cout << "\t\t|   Type: 'Help()' for a list of available options    |" <<endl;
    cout << "\t\t=======================================================" <<endl;

    cout << "\t\t  Attempting to load libCT.so ... ";
    gSystem->Load("libTree");
    gSystem->Load("libTreePlayer");
    gSystem->Load("libHist");
    int status1=gSystem->Load("libCT.so");
    if(status1>-1)
	cout << "attempt completed !!\n";
    else
	cout << "attempt failed.\n";
   
    //gStyle->SetMarkerStyle(7);    // Filled square
    gStyle->SetMarkerStyle(4);    // Circle
    gStyle->SetMarkerSize(.4);     // larger than half a pixel ...
    gStyle->SetMarkerColor(kBlack);
    //gStyle->SetMarkerColor(kRed);

}

