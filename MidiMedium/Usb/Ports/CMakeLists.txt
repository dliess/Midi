add_subdirectory(Test EXCLUDE_FROM_ALL)
add_subdirectory(UnitTest EXCLUDE_FROM_ALL)

set(TARGET_NAME "usbMidiPortHandling")

set(SOURCES
  "UsbMidiInputPortListProvider.cpp"
  "UsbMidiOutputPortListProvider.cpp"
)

set(HEADERS
  "UsbMidiPortInfo.h"
  "RtMidiAdaptTypes.h"
  "UsbMidiPortNotifier.h"
  "UsbMidiPortNotifier.tcc"
  "UsbMidiOutputPortListProvider.h"
  "UsbMidiInputPortListProvider.h"
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
    RtMidi::rtmidi
)

if(rtmidi_ADDED)
  target_compile_definitions(${TARGET_NAME} PUBLIC __RTMIDI_ADDED__="ON")
endif()
target_compile_definitions(${TARGET_NAME} PUBLIC __RTMIDI_API__=${RtMidiApi})

install(TARGETS ${TARGET_NAME} EXPORT ${PROJECT_NAME}-targets
        PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME})