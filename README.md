Color Transparency Experiment in Hall-C at JLab
==================================================

This repo is meant to contain codes for offline analysis for the color transparency experiment at 12 GeV in Hall C. 

Instruction:
--------------

* Modify `src/Constants.h` file to reflect correct data file path.
* Compile from the top level directory by typing `make`.
* Before you try it, do the following:

Put the following command into your `~/.bashrc` file:

```
if [ -f /path/to/libCT/bin/thisCT.(c)sh ]; then

. /path/to/libCT/bin/thisCT.(c)sh

fi
```
The above syntax is for `sh` shell. If you used `csh` shell, modify the syntax accordingly.

Or setup for the current session only can be done using

```
source bin/thisCT.(c)sh
```

* Now copy the rootlogon.C file in the directory 'macros' under ROOT installation directory.  
	  
* Now from a new terminal start root doing `root -l`.
* For a list of available options Type `Help()`
