amber-cpp-drivers
=================

[![Build Status](https://travis-ci.org/project-capo/amber-cpp-drivers.svg?branch=master)](https://travis-ci.org/project-capo/amber-cpp-drivers)

[![Coverity Scan Build Status](https://scan.coverity.com/projects/4014/badge.svg?style=flat)](https://scan.coverity.com/projects/4014)

Robot devices drivers for Amber platform in C/C++.

Requirements
------------

* `protobuf-compiler` and `libprotobuf-dev`
* `libboost-dev` and stuff, like `libboost-thread-dev`, `libboost-system-dev`, `libboost-program-options-dev`
* `liblog4cxx10-dev`
* `g++` with `make`

How to use (Makefile)
---------------------

Simply.

    make
    ./<name>-driver/bin/<name>_driver

How to contribute
-----------------

Clone this repo and setup your enviroment. Next, change what you want and make pull request.
