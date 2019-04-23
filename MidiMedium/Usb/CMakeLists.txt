cmake_minimum_required(VERSION 2.8.12)

set (CMAKE_CXX_STANDARD 11)

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