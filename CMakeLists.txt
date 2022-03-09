cmake_minimum_required(VERSION 3.11)

project(Midi
        LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_DEBUG_POSTFIX _g)

include(GNUInstallDirs)

option (MIDI_BUILD_TESTS "build tests enabled" ON)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(cmake/CPM.cmake)

find_package(Threads REQUIRED)

#CPMFindPackage(
#  NAME rtmidi
#  GITHUB_REPOSITORY dliess/rtmidi
#  GIT_TAG 292353f990a6f5ecf13d7807cfeacabd16825bf7
#  OPTIONS "BUILD_SHARED_LIBS OFF"
#)

CPMFindPackage(
  NAME rtmidi
  GITHUB_REPOSITORY dliess/rtmidi
  #GIT_TAG b2208130e9d60795cdeb6d8b2eb5b9ef69262412
  GIT_TAG develop
  OPTIONS "BUILD_SHARED_LIBS OFF"
)
if(rtmidi_ADDED)
  add_library(RtMidi::rtmidi ALIAS rtmidi)
endif()

CPMFindPackage(
  NAME fmt
  GITHUB_REPOSITORY fmtlib/fmt
  GIT_TAG 6.2.0
)
if(fmt_ADDED)
  install(TARGETS fmt EXPORT ${PROJECT_NAME}-targets
        PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME})
endif()

CPMFindPackage(
  NAME mpark_variant
  GITHUB_REPOSITORY mpark/variant
  VERSION 1.4.0
)
if(mpark_variant_ADDED)
  install(TARGETS mpark_variant EXPORT ${PROJECT_NAME}-targets
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME})
endif()

add_subdirectory(MidiMedium)
if(MIDI_BUILD_TESTS)
add_subdirectory(UnitTest)
endif()

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
  "Midi1Input.tcc"
  "MidiCCHighResOutputHandler.h"
  "MidiCCPairMap.h"
  "MidiInputCbBase.h"
  "MidiInput_inl.h"
  "MidiMessageIdsExtension.h"
  "NonBufferedMessageDrain.h"
  "XRpnInputHandler.h"
)

set(TARGET_NAME "midi")

add_library(${TARGET_NAME} STATIC ${SOURCES} ${HEADERS})

target_include_directories(${TARGET_NAME}
  PUBLIC
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}>"
    "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}>"
)
set_target_properties(${TARGET_NAME} PROPERTIES
    FRAMEWORK TRUE
    PUBLIC_HEADER "${HEADERS}"
)

target_link_libraries(${TARGET_NAME}
  PUBLIC
    midimedium
    mpark_variant
    fmt
)

install(TARGETS ${TARGET_NAME} EXPORT ${PROJECT_NAME}-targets
        PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME})


install(EXPORT ${PROJECT_NAME}-targets
  NAMESPACE  MIDI::
  DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}"
)

install(FILES "${CMAKE_CURRENT_SOURCE_DIR}/cmake/midi-config.cmake" 
        DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}"
)