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

## Build and run (tested in Manjaro, Arch and Raspbian (RP3 B)

```
mkdir build
cd build
cmake ../sources/
cmake --build .
```
