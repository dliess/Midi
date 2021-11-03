cmake_minimum_required(VERSION 2.8.12)

project(Midi)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_DEBUG_POSTFIX _g)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(cmake/CPM.cmake)

#CPMFindPackage(
#  NAME rtmidi
#  GITHUB_REPOSITORY dliess/rtmidi
#  GIT_TAG 292353f990a6f5ecf13d7807cfeacabd16825bf7
#  OPTIONS "BUILD_SHARED_LIBS OFF"
#)

CPMFindPackage(
  NAME rtmidi
  GITHUB_REPOSITORY thestk/rtmidi
  GIT_TAG b2208130e9d60795cdeb6d8b2eb5b9ef69262412
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
add_subdirectory(UnitTest)

set(SOURCES
  "Midi1Output.cpp"
  "MidiCCInputHandler.cpp"
  "XRpnInputHandler.cpp"
  "XRpnOutputHandler.cpp"
)

set(HEADERS
  "IMidi1InputCallback.h"
  "Midi1Output.h"
  "MidiCCInputHandler.h"
  "Midi.h"
  "MidiInput.h"
  "MidiMessage.h"
  "MidiMessageIds.h"
  "TransportHandling.h"
  "TransportHandling.tcc"
  "XRpnOutputHandler.h"
  "Midi1Input.h"
  "MidiCCHighResOutputHandler.h"
  "MidiCCPairMap.h"
  "MidiInputCbBase.h"
  "MidiInput_inl.h"
  "MidiMessageIdsExtension.h"
  "NonBufferedMessageDrain.h"
  "XRpnInputHandler.h"
)

add_library(midi STATIC ${SOURCES} ${HEADERS})

target_include_directories(midi PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}
    $<TARGET_PROPERTY:midimedium,INTERFACE_INCLUDE_DIRECTORIES>
)

target_link_libraries(midi
    PUBLIC mpark_variant
    PUBLIC fmt
)