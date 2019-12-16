cmake_minimum_required(VERSION 3.1.0)

#Name of the project
project(Classifier)

#Flags
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

#Include libraries
find_package(OpenCV REQUIRED)
find_package(Qt5 COMPONENTS Core Network Widgets)

#Includes of libraries
include_directories(${OpenCV_INCLUDE_DIRS})


#Headers
set(header_files   
    classifier.h
    classifiedimageinfo.h
    objectinfo.h
)

#Sources
set(source_files 
    classifier.cpp
    classifiedimageinfo.cpp
    objectinfo.cpp
)

#Set name of executable
add_library(${PROJECT_NAME})

target_sources(${PROJECT_NAME} PRIVATE ${source_files} ${header_files})

#Link libraries
target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS} Qt5::Core Qt5::Network Qt5::Widgets)
