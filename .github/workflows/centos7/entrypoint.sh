#!/bin/sh -l
set -x
uname -a 
cat /etc/issue
yum -y install  gcc gcc-c++ gcc-gfortran make which cmake cmake-data cmake-filesystem 
yum -y install  libX11-devel libX11  libXmu-devel libXmu libXau-devel libXau libXcursor-devel  libXcursor  libSM-devel libSM libICE libICE-devel libXext-devel libXext
yum -y install  xorg-x11-server-Xorg mesa-libGL-devel motif-devel
yum -y install  cmake3*

cmake3 -S. -BBUILD -DCMAKE_INSTALL_PREFIX=$(pwd)/INSTALL
cmake3 --build BUILD 
cmake3 --install BUILD 

out=$?
echo ::set-output name=out::$out
