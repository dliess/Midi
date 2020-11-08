cmake_minimum_required(VERSION 2.8.12)

project(Midi)

set (CMAKE_CXX_STANDARD 17)

include(cmake/CPM.cmake)

CPMFindPackage(
  NAME rtmidi
  GITHUB_REPOSITORY dliess/rtmidi
  GIT_TAG 27f55e39deae3cdf7ddb43d304b1e432199e4517
  OPTIONS "BUILD_SHARED_LIBS OFF"
)

CPMFindPackage(
  NAME fmt
  GITHUB_REPOSITORY fmtlib/fmt
  VERSION 7.0.3
  GIT_TAG 76e97dc4df0517749da366a2f78ebb4f87977804
)

CPMFindPackage(
  NAME variant
  GITHUB_REPOSITORY mpark/variant
  VERSION 1.4.0
)

add_subdirectory(MidiMedium)

add_library(midi STATIC
    "Midi1Output.cpp"
    "MidiCCInputHandler.cpp"
    "XRpnInputHandler.cpp"
    "XRpnOutputHandler.cpp"
)

target_include_directories(midi PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}
    $<TARGET_PROPERTY:midimedium,INTERFACE_INCLUDE_DIRECTORIES>
)

target_link_libraries(midi
    PUBLIC mpark_variant
    PUBLIC fmt
)