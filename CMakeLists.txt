cmake_minimum_required(VERSION 4.0)
project(06Nov)

set(CMAKE_CXX_STANDARD 20)

add_executable(06Nov main.cpp
        ListaDeCarga.h
        ListaDeCarga.cpp
        RotorDeMapeo.h
        RotorDeMapeo.cpp
        TramaBase.h
        TramaLoad.h
        TramaLoad.cpp
        TramaMap.h
        TramaMap.cpp
        SerialPort.h
        SerialPort.cpp
        main.cpp)
