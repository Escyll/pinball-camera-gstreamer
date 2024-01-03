include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(PinballOpenCVApi
    main.cpp
    FrameFetcher.h
    FrameFetcher.cpp
)

target_link_libraries(PinballOpenCVApi
    ${OpenCV_LIBS}
    PkgConfig::gtk3
    PkgConfig::gstreamer
    PkgConfig::gstreamer-app
    Qt5::Core Qt5::Gui)