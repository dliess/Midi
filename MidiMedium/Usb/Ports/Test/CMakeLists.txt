cmake_minimum_required(VERSION 2.8.12)

set (CMAKE_CXX_STANDARD 11)

add_executable(testUsbMidiPorts "main.cpp") 
target_link_libraries(testUsbMidiPorts usbMidiPortHandling)