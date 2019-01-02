#! /bin/tcsh

set called=($_)
if ("$called" != "") then
  set scriptdir=$called[2]
  set CT12GEV_DIR=`dirname $scriptdir`
  set CT12GEV_DIR=`c\d $CT12GEV_DIR && pwd` 
else
  set scriptdir=$1
  set CT12GEV_DIR=$scriptdir
endif
setenv onlineDir "${CT12GEV_DIR}"
    
    
echo ""
echo "Setting up environment for the E1206107 analysis:"

echo " -> sourcing the libCT.so environment"
source "${onlineDir}/CTAnalyzer/bin/thislibCT.csh"

echo " -> sourcing hcana environment"
source "${onlineDir}/../hcana/setup.csh" "${onlineDir}/hcana"

echo " -> sourcing replay environment"
source "${onlineDir}/ct_replay/setup.csh"

echo " -> changing to replay folder"
cd "${onlineDir}/ct_replay"

echo ""

