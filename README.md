Color Transparency Experiment in Hall-C at JLab
==================================================

This repo is meant to contain codes for offline analysis for the color transparency (CT) experiment at 12 GeV in Hall C. 


Directory Structure
-------------------

The three components of CT reconstruction (replay) and analysis are:

-- `hcana` (The hall C Analyzer)
-- `ct_replay`
-- CTAnalyzer

The directory `ct_replay` has `hallc_replay` as submodule, while `hcana` is a clone of original `hcana`. 

source root and other libraries as:
```
source /site/12gev_phys/production.csh 2.0
```

To run replay:

From top level directory of CT12GeV do:

```
source setup_replay.csh
```

This will setup necessary environment and take you to the `ct_replay` directory. Start hall C analyzer by typing `hcana` and run your desired replay script. 


Instructions for CTAnalyzer
----------------------------
* Modify `src/Constants.h` file to reflect correct data file path.

* Compile from the top level directory by typing `make`.

* Before you try it, do the following:

Put the following command into your `~/.bashrc` file:

```
if [ -f /path/to/libCT/bin/thisCT.(c)sh ]; then

. /path/to/libCT/bin/thisCT.(c)sh

fi
```
The above syntax is for `sh` shell. If you use `csh` shell, modify the `if statement` syntax accordingly.

However if you want to do the setup for the current session only, then just source as follows:

```
source bin/thisCT.(c)sh
```

* Now copy the rootlogon.C file in the directory `macros` under ROOT installation directory.  
	  
* From a new terminal start root doing `root -l`.

* For a list of available options Type `Help()` from `root`.

* If you are running on `cdaql1`, setup environment first of all as: `source setup.csh`
