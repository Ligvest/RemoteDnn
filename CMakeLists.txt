cmake_minimum_required(VERSION 3.1.0)

#Name of the project
project(RemoteDnn)

#Flags
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

#Include libraries
find_package(OpenCV REQUIRED)
find_package(Qt5 COMPONENTS Widgets Core)

#Includes of libraries
include_directories(${OpenCV_INCLUDE_DIRS})

#Headers
set(header_files   
    window.h
    profile_advanced.h
)

#Sources
set(source_files 
    main.cpp
    window.cpp
    window.ui
    profile_advanced.cpp
)

add_subdirectory(RemoteClassifier)

#Set name of executable
add_executable(${PROJECT_NAME})

target_sources(${PROJECT_NAME} PRIVATE ${source_files} ${header_files})

#Link libraries
target_link_libraries(${PROJECT_NAME} Classifier DataTransfer RemoteClassifier ${OpenCV_LIBS} Qt5::Core Qt5::Widgets)
