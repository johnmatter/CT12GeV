#!/bin/csh
# Setup for CT on cdaql1
# Latif Kabir

echo "Setting environment for CT on cdaql1 ........"
#---------------------- Setup for EVIO/ET libraries ------------------
set called=($_)
if ("$called" != "") then
  set scriptdir=$called[2]
  set GEM_DATAEXPO_DIR=`dirname $scriptdir`
  set GEM_DATAEXPO_DIR=`c\d $GEM_DATAEXPO_DIR && pwd`    # ensure absolute path
else
  set scriptdir=$1
  set GEM_DATAEXPO_DIR=$scriptdir
endif
setenv LD_LIBRARY_PATH "${LD_LIBRARY_PATH}:${GEM_DATAEXPO_DIR}/coda/Linux-x86_64/lib"


#----------------- Setup for ROOT 6----------------------------
module load gcc_5.2.0

if (!($?LD_LIBRARY_PATH)) then
    setenv LD_LIBRARY_PATH /apps/gcc/5.2.0/install/lib64:/apps/python/python-2.7.1/lib/
else 
    setenv LD_LIBRARY_PATH /apps/gcc/5.2.0/install/lib64:/apps/python/python-2.7.1/lib/:$LD_LIBRARY_PATH
endif

setenv CXX /apps/gcc/5.2.0/install/bin/c++
setenv GCC /apps/gcc/5.2.0/install/bin/gcc
setenv CC /apps/gcc/5.2.0/install/bin/gcc
setenv PATH /apps/gcc/5.2.0/install/bin:$PATH
setenv PATH /apps/cmake/cmake-3.5.1/bin:/apps/python/python-2.7.1/bin:$PATH

# Need Python >= 2.7.1
setenv PYTHON /apps/python/python-2.7.1

# For csh family
setenv ROOTSYS /u/apps/root/6.08.00/root
setenv PATH ${PYTHON}/bin/:${ROOTSYS}/bin:${PATH}

if (!($?LD_LIBRARY_PATH)) then
  setenv LD_LIBRARY_PATH ${PYTHON}/lib:${ROOTSYS}/lib
else
  setenv LD_LIBRARY_PATH  ${PYTHON}/lib:${ROOTSYS}/lib:${LD_LIBRARY_PATH}
endif

#---------------------------------------------------------------------------------
echo "Done"
