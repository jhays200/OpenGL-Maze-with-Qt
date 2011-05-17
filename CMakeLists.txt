cmake_minimum_required(VERSION 2.8)
project(OpenGL-Lab3)

#Version Information
set(${PROJECT_NAME}_MAJOR_VERSION 1)
set(${PROJECT_NAME}_MINOR_VERSION 0)

#include debug symbols
set(CMAKE_CXX_FLAGS "-g -Wall")

#Qt lib stuff
find_package(Qt4 REQUIRED)
set(QT_USE_QTOPENGL TRUE)
include(${QT_USE_FILE})

#include directories to mess with
include_directories(GLWidget QtLogic)

#Separeted graphics and interface logic into libs
add_subdirectory(GLWidget)
#add_subdirectory(QtLogic)

#compile time stuff
add_executable(Demo main.cpp)
target_link_libraries(Demo ${QT_LIBRARIES} GLWidgetLib)# QtLogicLib)
