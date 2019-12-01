@echo on

rmdir /S /Q .\build .\install
mkdir build
cd .\build
mkdir debug release

cd .\debug
cmake -DCMAKE_INSTALL_PREFIX:PATH=..\..\install -DBOOST_LIBRARYDIR:PATH=..\..\..\libraries\3rdparty\boost\stage\lib -DBOOST_ROOT:PATH=..\..\..\libraries\3rdparty\boost -DCMAKE_BUILD_TYPE=Debug -G Ninja ..\..
ninja -v
ninja install
cd ..

cd .\release
cmake -DCMAKE_INSTALL_PREFIX:PATH=..\..\install -DBOOST_LIBRARYDIR:PATH=..\..\..\libraries\3rdparty\boost\stage\lib -DBOOST_ROOT:PATH=..\..\..\libraries\3rdparty\boost -DCMAKE_BUILD_TYPE=Release -G Ninja ..\..
ninja -v
ninja install
cd ..\..
