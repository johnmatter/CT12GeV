#!/bin/csh
# Setup for CT on cdaql1
# Latif Kabir

echo "Setting environment for CT on cdaql1 ........"
#---------------------- Setup for EVIO/ET libraries ------------------
set called=($_)
if ("$called" != "") then
  set scriptdir=$called[2]
  set CT_ANALYSIS_DIR=`dirname $scriptdir`
  set CT_ANALYSIS_DIR=`c\d $CT_ANALYSIS_DIR && pwd`    # ensure absolute path
else
  set scriptdir=$1
  set CT_ANALYSIS_DIR=$scriptdir
endif

setenv LD_LIBRARY_PATH "${LD_LIBRARY_PATH}:${CT_ANALYSIS_DIR}/lib"
setenv PATH "${PATH}:${CT_ANALYSIS_DIR}/bin"

    

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
#setenv ROOTSYS /u/apps/root/6.08.00/root
setenv ROOTSYS /site/12gev_phys/2.0/Linux_CentOS7.2.1511-x86_64-gcc5.2.0/root/6.08.00/bin/root
setenv PATH ${PYTHON}/bin/:${ROOTSYS}/bin:${PATH}

if (!($?LD_LIBRARY_PATH)) then
  setenv LD_LIBRARY_PATH ${PYTHON}/lib:${ROOTSYS}/lib
else
  setenv LD_LIBRARY_PATH  ${PYTHON}/lib:${ROOTSYS}/lib:${LD_LIBRARY_PATH}
endif

#---------------------------------------------------------------------------------
echo "Done"
