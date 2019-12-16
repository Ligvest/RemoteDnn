cmake_minimum_required(VERSION 3.1.0)

#Name of the project
project(DataTransfer)

#Flags
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

#Include libraries
find_package(Qt5 COMPONENTS Core Network Widgets)

#Headers
set(header_files
    dataclient.h
    dataserver.h   
)

#Sources
set(source_files 
    dataclient.cpp
    dataserver.cpp
)

#Set name of executable
add_library(${PROJECT_NAME})

target_sources(${PROJECT_NAME} PRIVATE ${source_files} ${header_files})

#Link libraries
target_link_libraries(${PROJECT_NAME} Qt5::Core Qt5::Network Qt5::Widgets)
