# Filename: thislibCT.csh
# Description: 
# Author: Latif Kabir < latif@jlab.org >
# Created: Mon Jan 15 18:23:22 2018 (-0500)
# URL: jlab.org/~latif

#!/bin/csh
    
set called=($_)
if ("$called" != "") then
  set scriptdir=$called[2]
  set CTBIN=`dirname $scriptdir`
  set CTBIN=`c\d $CTBIN && pwd`    # ensure absolute path
else
  set scriptdir=$1
  set CTBIN=$scriptdir
endif
set CTROOT=`cd $CTBIN/.. && pwd`
    
setenv LD_LIBRARY_PATH "${LD_LIBRARY_PATH}:${CTROOT}/lib"
setenv PATH "${PATH}:${CTROOT}/bin"
    
