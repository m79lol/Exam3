@echo on

rmdir /S /Q .\build .\install
if %errorlevel% neq 0 GOTO exit
mkdir build
if %errorlevel% neq 0 GOTO exit
cd .\build
if %errorlevel% neq 0 GOTO exit
mkdir debug release
if %errorlevel% neq 0 GOTO exit_from_build

cd .\debug
if %errorlevel% neq 0 GOTO exit_from_build
cmake -DCMAKE_INSTALL_PREFIX:PATH=..\..\install -DBOOST_LIBRARYDIR:PATH=..\..\libraries\3rdparty\boost\stage\lib -DBOOST_ROOT:PATH=..\..\libraries\3rdparty\boost -DCMAKE_BUILD_TYPE=Debug -G Ninja ..\..
if %errorlevel% neq 0 GOTO exit_from_subconfig
ninja -v
if %errorlevel% neq 0 GOTO exit_from_subconfig
ninja -v install
if %errorlevel% neq 0 GOTO exit_from_subconfig
cd ..
if %errorlevel% neq 0 GOTO exit_from_build

cd .\release
if %errorlevel% neq 0 GOTO exit_from_build
cmake -DCMAKE_INSTALL_PREFIX:PATH=..\..\install -DBOOST_LIBRARYDIR:PATH=..\..\libraries\3rdparty\boost\stage\lib -DBOOST_ROOT:PATH=..\..\libraries\3rdparty\boost -DCMAKE_BUILD_TYPE=Release -G Ninja ..\..
if %errorlevel% neq 0 GOTO exit_from_subconfig
ninja -v
if %errorlevel% neq 0 GOTO exit_from_subconfig
ninja -v install
if %errorlevel% neq 0 GOTO exit_from_subconfig
cd ../..
if %errorlevel% neq 0 GOTO exit

GOTO exit

:exit_from_subconfig
  cd ..

:exit_from_build
  cd ..
  
:exit
  exit /b %errorlevel%

