cmake_minimum_required(VERSION 3.1.0)

project(SnakeGame VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)


include_directories(${PROJECT_SOURCE_DIR}/Headers)

find_package(Qt5 REQUIRED Core Widgets)


add_subdirectory(Sources)
