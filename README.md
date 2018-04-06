Color Transparency Experiment in Hall-C at JLab
==================================================

This repo is meant to contain codes for offline analysis for the color transparency (CT) experiment at 12 GeV in Hall C. 


Directory Structure
-------------------

The three components of CT reconstruction (replay) and analysis are:

- `hcana` : The Hall C Analyzer. `hcana` directory is a clone of original `hcana`
- `ct_replay` : CT Replay with CT specific scripts, calibration and parameters/database. The directory `ct_replay` has `hallc_replay` as submodule. The files inside `hallc_replay` are fork from original `hallc_replay` repository.
- CTAnalyzer : Analysis library/scripts for CT data.

If you want to setup a personal copy, just clone/download this repository. Add a clone of `hcana` inside `CT12GeV` and create appropriate (`raw`,`ROOTfiles` etc) symbolic links inside `ct_replay`. The following instructions are based on the shared setup under `/work` disk on `ifarm`.


Instruction for CT Replay under `/work` directory on ifarm
-----------------------------------------------------------

source root and other libraries as:

```
source /site/12gev_phys/production.csh 2.0
```

- Note: Depending on which version of `ROOT` you are using, you might need to re-compile `libCT` or `hcana` again, specially if your version is different than the one used to compile earlier by any other user.


**Running replay**

From top level directory of `CT12GeV` do:

```
source setup_replay.(c)sh
```

This will setup necessary environment and take you to the `ct_replay` directory. Start Hall C analyzer by typing `hcana` and run your desired replay script. 


`ct_replay` vs `hallc_replay`
-----------------------------
`ct_replay` is the CT version of `hallc_replay`. `ct_replay` has `hallc_replay` directory inside it added as submodule. Both `ct_replay` and `hallc_replay` have all necessary calibration, definition and templates directories/files, but the contents of the files are not necessarily the same. CT users will be modifying or adding files within `ct_replay` directory and keeping those inside `hallc_replay/` intact. However the replay script of the user might call the database/calibration either from `ct_replay` or `hallc_replay`. All CT related replay scripts will reside inside `ct_replay/SCRIPTS/` directory.


If you want to use the database/parameters from `ct_replay`, the replay script will call parameter/database files just in the usual way, for example:

```
  gHcParms->AddString("g_ctp_database_filename", "DBASE/COIN/STD/standard.database");
  gHcDetectorMap->Load("MAPS/COIN/DETEC/coin.map");
  gHcParms->Load("PARAM/SHMS/GEN/p_fadc_debug.param");
  gHcParms->Load("PARAM/HMS/GEN/h_fadc_debug.param");
```

But if you want to use the database/parameters from `hall_replay`, just use `hallc_replay` as the prefix in the path, for example:

```
  gHcParms->AddString("g_ctp_database_filename", "hallc_replay/DBASE/COIN/STD/standard.database");
  gHcDetectorMap->Load("hallc_replay/MAPS/COIN/DETEC/coin.map");
  gHcParms->Load("hallc_replay/PARAM/SHMS/GEN/p_fadc_debug.param");
  gHcParms->Load("hallc_replay/PARAM/HMS/GEN/h_fadc_debug.param");
```

- If any `hallc_relay` calibration provides better result, you might want to copy the file to the corresponding directory under `ct_replay`.


**Updating `hallc_replay`**

If you want to update the `hallc_replay` directory from the main git repository maintained by Hall C, use the following command from the top level directory `CT12GeV`:
```
git submodule update --init --recursive
```

**Updating `hcana`**

`hcana` is a clone copy. To update, from the `CT12GeV/hcana/` directory do:

```
git pull
```

Now recompile `hcana` using:
```
make
```

Instructions for CT Analyzer (`libCT`)
--------------------------------------
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



Push Update to Github
-----------------------

- Keep all plots under the directory `CT12GeV/CTAnalyzer/PLOTS/`. Keep interpreted analysis scripts under `CT12GeV/CTAnalyzer/scripts/`.

- If you make any change to the `CT12GeV` directory, push it to Github. To make the push easier, there is a script `push.sh`, you can run it as:

```
push.sh  "Short description of changes here"
```

This will push all changes/update to Github and change the permission of newly created files so that other user can modify (discard not permitted message for files that do not belong to you). 


You might need to do `git pull` first if git complains that local repo does not have all updates from remote repo. 
