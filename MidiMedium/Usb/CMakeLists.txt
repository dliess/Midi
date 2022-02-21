set(RtMidiApi "RtMidi::LINUX_ALSA" CACHE STRING "Midi-API")
set_property(CACHE RtMidiApi PROPERTY STRINGS "RtMidi::UNIX_JACK" "RtMidi::LINUX_ALSA")
message(STATUS "RtMidiApi='${RtMidiApi}'")

add_subdirectory(Ports)

set(TARGET_NAME "usbmidi")

set(SOURCES
  "UsbMidiOut.cpp"
  "UsbMidiIn.cpp"
)

set(HEADERS
  "UsbMidiOut.h"
  "UsbMidiIn.h"
)

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
    usbMidiPortHandling 
    RtMidi::rtmidi
)

if(rtmidi_ADDED)
  target_compile_definitions(${TARGET_NAME} PUBLIC __RTMIDI_ADDED__="ON")
endif()
target_compile_definitions(${TARGET_NAME} PUBLIC __RTMIDI_API__=${RtMidiApi})

install(TARGETS ${TARGET_NAME} EXPORT ${PROJECT_NAME}-targets
        PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME})