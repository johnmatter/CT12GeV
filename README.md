Color Transparency Experiment in Hall-C at Lab
==================================================

This repo is meant to contain codes for offline analysis in the color transparency experiment at 12 GeV in Hall C. 

Instruction:
--------------
* Compile the make file inside libCT directory typing `make` from that directory.
* Before you try it you MUST do the following:

Put the following command into your `~/.bashrc` file:

```
if [ -f /path/to/libCT/bin/thisCT.(c)sh ]; then

. /path/to/libCT/bin/thisCT.(c)sh

fi
```
The above syntax is for `sh` shell. If you used `csh` shell, modify the syntax accordingly.

Now copy the rootlogon.C file in the directory 'macros' under ROOT installation directory.  
	  
* Now from a new terminal start root doing `root -l`.
* For a list of available options Type `Help()`
