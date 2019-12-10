# Hash Server
Hash Server is an asynchronous TCP server that receives platform-specific strings with the end of line delimiter specified for the current platform (Windows/Linux), and sends back a SHA256 hash of the received string with the same end-of-line delimiter.
The server is implemented as a shared library. The repository also includes an example server app and a test app based on boost unit test framework.
Build generator - CMake, build system - ninja.


## Dependencies
- [Boost](https://www.boost.org/) (at least 1.66 version)
- [PicoSHA2](https://github.com/okdshin/PicoSHA2)
- [CMake](https://cmake.org/) (at least 3.13)
- [ninja-build](https://ninja-build.org/) (at least 1.5)

## Tested on
- Windows 10, MSVC 2017 
- Linux (Ubuntu 16.04), gcc 5.2

## Build steps
### Clone repository
`git clone --recursive https://github.com/m79lol/Exam3.git`

### Build Boost
Run follow commands at the boost folder:
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
Run follow commands from **Developer Command Prompt for VS**:
~~~
mkdir build
cd .\build
cmake -DCMAKE_INSTALL_PREFIX:PATH=<install_path> -DBOOST_LIBRARYDIR:PATH=<boost_libs_path> -DBOOST_ROOT:PATH=<boost_path> -G Ninja ..
ninja
ninja test
ninja install
~~~
##### linux
~~~
mkdir build
cd ./build
cmake -DBoost_NO_SYSTEM_PATHS=TRUE -DCMAKE_INSTALL_PREFIX:PATH=<install_path> -DBOOST_LIBRARYDIR:PATH=<boost_libs_path> -DBOOST_ROOT:PATH=<boost_path> -G Ninja ..
ninja
ninja test
ninja install
~~~

## Run
Just run `app` or `app --help`.
##### windows
Start **telnet** and input the following to connect to the server: `open localhost 6767`
##### linux
Run `netcat localhost 6767`
Then you can write strings and send by Enter key.
