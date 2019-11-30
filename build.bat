@echo off

rmdir /S /Q .\build
mkdir build
cd .\build
cmake -DCMAKE_INSTALL_PREFIX:PATH=..\install -DBOOST_LIBRARYDIR:PATH=..\..\libraries\3rdparty\boost\stage\lib -DBOOST_ROOT:PATH=..\..\libraries\3rdparty\boost -G Ninja ..
ninja
ninja install
cd ..