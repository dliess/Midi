cmake_minimum_required(VERSION 2.8.12)

set (CMAKE_CXX_STANDARD 17)

add_subdirectory(Ports)

add_library(usbmidi STATIC 
    "UsbMidiOut.cpp"
    "UsbMidiIn.cpp"
)

target_include_directories(usbmidi PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_include_directories(usbmidi PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}
    $<TARGET_PROPERTY:midimedium,INTERFACE_INCLUDE_DIRECTORIES>
)

target_link_libraries(usbmidi PUBLIC usbMidiPortHandling rtmidi)

set(RtMidiApi "RtMidi::LINUX_ALSA" CACHE STRING "Midi-API")
set_property(CACHE RtMidiApi PROPERTY STRINGS "RtMidi::UNIX_JACK" "RtMidi::LINUX_ALSA")
message(STATUS "RtMidiApi='${RtMidiApi}'")
target_compile_definitions(usbmidi PUBLIC __RTMIDI_API__=${RtMidiApi})