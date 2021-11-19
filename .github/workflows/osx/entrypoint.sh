#!/bin/sh -l
set -x
brew install wget coreutils gcc openmotif
#brew  install --cask  xquartz

cmake -S. -BBUILD -DCMAKE_INSTALL_PREFIX=$(pwd)/INSTALL -DCMAKE_Fortran_COMPILER=gfortran-11
cmake --build BUILD 
cmake --install BUILD 
