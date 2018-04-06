#! /bin/bash

    
onlineDir="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"    
export onlineDir

echo ""
echo "Setting up environment for the E1206107 analysis:"


echo " -> sourcing hcana environment"
source ${onlineDir}/hcana/setup.sh
export PATH=$PATH:${onlineDir}/hcana

echo " -> sourcing replay environment"
source ${onlineDir}/ct_replay/setup.sh

echo " -> changing to replay folder"
cd ${onlineDir}/ct_replay

echo ""
echo "Run './run_coin.sh' to analyze coincidence run"
echo ""
