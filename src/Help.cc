// Filename: Help.cpp
// Description: 
// Author: Latiful Kabir < siplukabir@gmail.com >
// Created: Mon Mar 28 15:12:34 2016 (-0400)
// URL: latifkabir.github.io

#include<iostream>
#include<sstream>
#include<fstream>
#include"Constants.h"
using namespace std;


int Help()
{
    string str;
    str=SOURCE_PATH;
	
    str=str+"LinkDef.h";
    cout << str <<endl;

    ifstream myFile(str.c_str());
    if(!myFile)
    {
	cout << "Source file NOT found" <<endl;
	return -1;
    }
    cout << "\n\n\t\t=========================================================================" <<endl;

    cout << "\t\t|\t\t\t   Functions/Classes inside libCT       \t|" <<endl;
    cout << "\t\t=========================================================================" <<endl;
    while(getline(myFile,str))       
	cout << str <<endl;

    myFile.close();
    
    return 0;
}


int Help(string file)
{
    string str=SOURCE_PATH;
    str=str+file+".h";
	ifstream myFile(str.c_str());
 
    if(!myFile)
    {
	cout << "Source file NOT found" <<endl;
	return -1;
    }
    while(getline(myFile,str))       
	cout << str <<endl;

    myFile.close();
    return 0;
}
