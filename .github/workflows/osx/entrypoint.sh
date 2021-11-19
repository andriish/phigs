#!/bin/sh -l
set -x
brew install wget coreutils gcc cmake
brew cask install xquartz

cmake -S. -BBUILD -DCMAKE_INSTALL_PREFIX=$(pwd)/INSTALL
cmake --build BUILD 
cmake --install BUILD 
