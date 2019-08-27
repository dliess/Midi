cmake_minimum_required(VERSION 2.8.12)

set (CMAKE_CXX_STANDARD 11)

add_subdirectory(Usb)
add_subdirectory(Dummy)

add_library(midimedium INTERFACE)
target_include_directories(midimedium INTERFACE ${CMAKE_CURRENT_LIST_DIR})
