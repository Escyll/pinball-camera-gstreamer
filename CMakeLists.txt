cmake_minimum_required(VERSION 3.12)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_AUTOMOC ON)

project(
    PinballOpenCVApi
    VERSION 1.0
    LANGUAGES CXX)

if(WIN32)
    set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/modules/)
    find_package(GSTREAMER REQUIRED)
    include_directories(${GSTREAMER_INCLUDE_DIRS})
    message("GStreamer: ${GSTREAMER_INCLUDE_DIRS}")
else()
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(gtk3 REQUIRED IMPORTED_TARGET gtk+-3.0)
    pkg_search_module(gstreamer REQUIRED IMPORTED_TARGET gstreamer-1.0>=1.4)
    pkg_search_module(gstreamer-app REQUIRED IMPORTED_TARGET gstreamer-app-1.0>=1.4)
endif()

find_package(OpenCV REQUIRED)
find_package(Qt6 6.4 REQUIRED COMPONENTS Core Gui HttpServer SerialPort)

add_subdirectory(src)