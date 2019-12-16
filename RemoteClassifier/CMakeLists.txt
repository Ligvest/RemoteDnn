cmake_minimum_required(VERSION 3.1.0)

#Name of the project
project(RemoteClassifier)

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
    imageinfo.h
    remoteclassifier.h
    remoteclassifierserver.h
    classifyworkerthread.h
)

#Sources
set(source_files 
    imageinfo.cpp
    remoteclassifier.cpp
    remoteclassifierserver.cpp
    classifyworkerthread.cpp
)

add_subdirectory(Classifier)
add_subdirectory(DataTransfer)

#Set name of executable
add_library(${PROJECT_NAME})
target_sources(${PROJECT_NAME} PRIVATE ${source_files} ${header_files})
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)


#Link libraries
target_link_libraries(${PROJECT_NAME} Classifier DataTransfer ${OpenCV_LIBS} Qt5::Core Qt5::Network Qt5::Widgets)
