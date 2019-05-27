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

## Build and run (tested on Manjaro, Arch and Raspbian)

```
mkdir build
cd build
cmake ../sources/
cmake --build .
```
