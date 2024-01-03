cmake_minimum_required(VERSION 3.12)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_AUTOMOC ON)

project(
    PinballOpenCVApi
    VERSION 1.0
    LANGUAGES CXX)

find_package(PkgConfig REQUIRED)
pkg_check_modules(gtk3 REQUIRED IMPORTED_TARGET gtk+-3.0)
pkg_search_module(gstreamer REQUIRED IMPORTED_TARGET gstreamer-1.0>=1.4)
pkg_search_module(gstreamer-app REQUIRED IMPORTED_TARGET gstreamer-app-1.0>=1.4)

find_package(OpenCV REQUIRED)
find_package(Qt5 COMPONENTS Core Gui REQUIRED)

add_subdirectory(src)