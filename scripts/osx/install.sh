#!/bin/bash

set -ev

SCRIPTDIR=$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )
source $SCRIPTDIR/../lib.sh

#install dependencies
brew update
brew install libtool gettext libuv libsigc++ jansson curl luajit owfs libusb ola
brew link --force gettext

mkdir -p $LOCAL_DEPS

pushd $HOME

if [ ! -e "$LOCAL_DEPS/include/gtest/gtest.h" ]; then

wget_retry https://github.com/google/googletest/archive/release-1.8.0.zip
unzip release-1.8.0.zip
pushd googletest-release-1.8.0/
mkdir build && pushd build
cmake .. -DCMAKE_INSTALL_PREFIX=$LOCAL_DEPS -DBUILD_SHARED_LIBS=ON -DCMAKE_SKIP_RPATH=ON
make
make install
popd
popd

fi

popd
