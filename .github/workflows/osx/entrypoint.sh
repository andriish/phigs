#!/bin/sh -l
set -x
brew install wget coreutils gcc cmake
brew update
brew cleanup
brew cask cleanup
brew cask install xquartz

cmake -S. -BBUILD -DCMAKE_INSTALL_PREFIX=$(pwd)/INSTALL -DCMAKE_Fortran_COMPILER=gfortran-11
cmake --build BUILD 
cmake --install BUILD 
