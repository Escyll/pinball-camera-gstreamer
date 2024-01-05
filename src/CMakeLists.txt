include_directories(${OpenCV_INCLUDE_DIRS})

qt_standard_project_setup()

qt_add_executable(PinballOpenCVApi
    main.cpp
    FrameFetcher.h
    FrameFetcher.cpp
    RestAPI.h
    RestAPI.cpp
)
if(WIN32)
target_link_libraries(PinballOpenCVApi
PRIVATE
    ${OpenCV_LIBS}    
    ${GSTREAMER_LIBRARIES}
    Qt6::Core Qt6::Gui Qt6::HttpServer Qt6::SerialPort)
else()
target_link_libraries(PinballOpenCVApi
    ${OpenCV_LIBS}
    PkgConfig::gtk3
    PkgConfig::gstreamer
    PkgConfig::gstreamer-app
    Qt6::Core Qt6::Gui Qt6::HttpServer Qt6::SerialPort)
endif()