cmake_minimum_required(VERSION 2.8.12)

set (CMAKE_CXX_STANDARD 11)

add_subdirectory(Ports)

add_library(usbmidi STATIC 
    "UsbMidiOut.cpp"
    "UsbMidiIn.cpp"
    "rtmidi/RtMidi.cpp"
)

target_include_directories(usbmidi PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_include_directories(usbmidi PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}
    $<TARGET_PROPERTY:midimedium,INTERFACE_INCLUDE_DIRECTORIES>
)

target_link_libraries(usbmidi PUBLIC usbMidiPortHandling)

target_compile_definitions(usbmidi PUBLIC "__LINUX_ALSA__")
target_link_libraries(usbmidi PUBLIC asound)

# Enable jack: - both alsa and jack can be enabled at the same time
# target_compile_definitions(usbmidi PUBLIC "__UNIX_JACK__")
# target_link_libraries(usbmidi PUBLIC jack)
