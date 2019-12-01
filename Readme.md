# Hash Server
Hash Server is async TCP server, that receive string with end line char specified for current platform (Windows/Linux) as delimiter and send SHA256 hash of that string also with delimiter at end line.
The server implemented as shared library. Also repository included example server app &  test app based on boost unit test framework.
Used generator is CMake, build system - ninja.

## Dependicies
- [Boost](https://www.boost.org/) (at least 1.66 version)
- [PicoSHA2](https://github.com/okdshin/PicoSHA2)
- [CMake](https://cmake.org/) (at least 3.13)
- [ninja-build](https://ninja-build.org/) (at least 1.5)

## Tested
- Windows 10, MSVC 2017 
- Linux (Ubuntu 16.04), gcc 5.2

## Build steps
### Clone repository
`git clone --recursive https://github.com/m79lol/Exam3.git`

### Build Boost
Run follow commands at boost folder:
##### windows
~~~
.\booststrap.bat
.\b2 --build-type=complete <boost_install_dir>
~~~
##### linux
~~~
./booststrap
./b2 toolset=gcc cxxflags=-fPIC cflags=-fPIC -a <boost_install_dir>
~~~

### Build Hash Server
##### windows
Run follows commands from **Developer Command Prompt for VS**:
~~~
mkdir build
cd .\build
cmake -DCMAKE_INSTALL_PREFIX:PATH=<install_path> -DBOOST_LIBRARYDIR:PATH=<boost_libs_path> -DBOOST_ROOT:PATH=<boost_path> -G Ninja ..
ninja
ninja install
~~~
##### linux
~~~
mkdir build
cd ./build
cmake -DBoost_NO_SYSTEM_PATHS=TRUE -DCMAKE_INSTALL_PREFIX:PATH=<install_path> -DBOOST_LIBRARYDIR:PATH=<boost_libs_path> -DBOOST_ROOT:PATH=<boost_path> -G Ninja ..
ninja
ninja install
~~~

## Run
The listening port by default is **6767**
##### windows
Start **telnet** and input follow to connect to the server: `open localhost 6767`
##### linux
Run `netcat localhost 6767`
Then you can write strings and send by Enter key.
