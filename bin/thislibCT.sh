# Filename: thislibCT.sh
# Description: 
# Author: Latif Kabir < latif@jlab.org >
# Created: Mon Jan 15 18:23:06 2018 (-0500)
# URL: jlab.org/~latif

CTBIN=$(dirname $BASH_ARGV[0])
CTROOT=$(cd $CTBIN/..;pwd)

export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$CTROOT/lib
export PATH=$PATH:$CTROOT/bin


