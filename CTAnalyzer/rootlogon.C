//Loading libCT automatically every time you start root.
//Author: Latiful kabir 
{
    cout << "\n\t\t=======================================================" <<endl;
    cout << "\t\t|  Welcome to CT Analysis Tool Integrated in ROOT     |"<<endl;
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
   
    // use plain black on white colors
    gStyle->SetFrameBorderMode(0);
    gStyle->SetCanvasBorderMode(0);
    gStyle->SetPadBorderMode(0);
    //gStyle->SetPadColor(0);
    //  gStyle->SetCanvasColor(0);
    //  gStyle->SetStatColor(0);
      gStyle->SetFillColor(2);

    // set the paper & margin sizes
    gStyle->SetPaperSize(24,24);
    //   gStyle->SetPadTopMargin(0.05);
    //gStyle->SetPadRightMargin(0.09);
    //gStyle->SetPadBottomMargin(0.16);
    //gStyle->SetPadLeftMargin(0.12);

    // use large Times-Roman fonts
    gStyle->SetTextFont(132);
    gStyle->SetTextSize(0.15);
    gStyle->SetLabelFont(132,"x");
    gStyle->SetLabelFont(132,"y");
    gStyle->SetLabelFont(132,"z");
    gStyle->SetLabelSize(0.05,"x");
    gStyle->SetTitleSize(0.2,"x");
    gStyle->SetLabelSize(0.05,"y");
    gStyle->SetTitleSize(0.2,"y");
    gStyle->SetLabelSize(0.05,"z");
    gStyle->SetTitleSize(0.2,"z");

    // use bold lines and markers
    gStyle->SetMarkerStyle(20);
    gStyle->SetMarkerSize(0.5);
    gStyle->SetHistLineWidth(3);
    gStyle->SetLineStyleString(2,"[12 12]"); // postscript dashes

    // get rid of X error bars and y error bar caps
    gStyle->SetErrorX(0.001);

    // do not display any of the standard histogram decorations
    //    gStyle->SetOptTitle(1);
    gStyle->SetOptStat(0);
    gStyle->SetOptFit(0);

    //    gStyle->SetPalette(0);
    //gStyle->SetPadBorderSize(3);
    gStyle->SetFrameLineWidth(3);
    // put tick marks on top and RHS of plots
    gStyle->SetPadTickX(1);
    gStyle->SetPadTickY(1);

    //gStyle->SetMarkerStyle(7);    // Filled square
    //    gStyle->SetMarkerStyle(4);    // Circle
    //gStyle->SetMarkerSize(.4);     // larger than half a pixel ...
    gStyle->SetMarkerColor(kBlack);

}

