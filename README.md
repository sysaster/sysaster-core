# sysaster-core

## Requirements

1. Run the following:
```
sudo apt-get update
sudo apt-get upgrade
sudo apt-get install libboost-all-dev
sudo apt-get install libcurl4-openssl-dev
```
2. Install restclient-cpp: https://github.com/mrtazz/restclient-cpp
3. Install `cmake` >= 3.13
4. Install sqlite3

### In order to support the Tello drone
1. Install python2
2. Install pip2
3. Install numpy for python2
4. Find includes for numpy and link to /usr/include/numpy
5. Install opencv-python via pip2

## Build and run (tested on Manjaro, Arch and Raspbian)

```
./configure.sh
cd build
cmake --build .
```
