cmake_minimum_required(VERSION 3.1)
set(project_name "img_proc")
PROJECT(${project_name})

add_definitions( -std=c++11 )

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
)


find_package(Qt5Core REQUIRED)
find_package(Qt5Gui REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Test REQUIRED)

set(SOURCES
    main.cpp
    mainwindow.cpp
    imagelabel.cpp
    scrollbar.cpp
    brightnesswindow.cpp
    filterwindow.cpp
    transformswindow.cpp
    morphwindow.cpp
    segmentationwindow.cpp
    segmentation.cpp
    morph.cpp
    imgdata.cpp
    converter.cpp
    filter.cpp
    transforms.cpp
)
set(HEADERS
    mainwindow.h
    imagelabel.h
    scrollbar.h
    brightnesswindow.h
    filterwindow.h
    transformswindow.h
    morphwindow.h
    segmentationwindow.h
    segmentation.h
    morph.h
    transforms.h
    imgdata.h
    converter.h
    filter.h
    data2d.h
)
qt5_wrap_ui(UIS_HDRS
    mainwindow.ui
    brightnesswindow.ui
    filterwindow.ui
    transformswindow.ui
    morphwindow.ui
    segmentationwindow.ui
)

add_executable(${project_name} WIN32 ${SOURCES} ${HEADERS} ${UIS_HDRS})
target_link_libraries(${project_name} Qt5::Core Qt5::Gui Qt5::Widgets Qt5::Test)
