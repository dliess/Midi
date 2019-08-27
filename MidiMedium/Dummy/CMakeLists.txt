cmake_minimum_required(VERSION 2.8.12)

set (CMAKE_CXX_STANDARD 14)

add_library(MidiMediumDummy STATIC "MidiMediumDummy.cpp")
target_include_directories(MidiMediumDummy PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(MidiMediumDummy PUBLIC midimedium)