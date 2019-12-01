#!/bin/bash
set -e

rm -rf ./build ./install
mkdir build
cd ./build
mkdir debug release

cd ./debug
cmake -DBoost_NO_SYSTEM_PATHS=TRUE -DCMAKE_INSTALL_PREFIX:PATH=../../install -DBOOST_LIBRARYDIR:PATH=../boost_1_66_0/stage/lib -DBOOST_ROOT:PATH=../boost_1_66_0 -DCMAKE_BUILD_TYPE=Debug -G Ninja ../..
ninja -v
ninja -v install
cd ..

cd ./release
cmake -DBoost_NO_SYSTEM_PATHS=TRUE -DCMAKE_INSTALL_PREFIX:PATH=../../install -DBOOST_LIBRARYDIR:PATH=../../boost_1_66_0/stage/lib -DBOOST_ROOT:PATH=../boost_1_66_0 -DCMAKE_BUILD_TYPE=Release -G Ninja ../..
ninja -v
ninja -v install
cd ../..
