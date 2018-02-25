#! /bin/tcsh
set onlineDir="/work/hallc/e1206107/CT12GeV"

echo ""
echo "Setting up environment for the E1206107 analysis:"


echo " -> sourcing hcana environment"
source "${onlineDir}/hcana/setup.csh" "${onlineDir}/hcana"

echo " -> sourcing replay environment"
source "${onlineDir}/ct_replay/setup.csh"

echo " -> changing to replay folder"
cd "${onlineDir}/ct_replay"

echo ""
echo "Run './run_coin.sh' to analyze coincidence run"
echo ""
