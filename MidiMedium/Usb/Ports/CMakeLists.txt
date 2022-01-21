add_subdirectory(Test EXCLUDE_FROM_ALL)

add_library(usbMidiPortHandling STATIC 
    "UsbMidiInputPortListProvider.cpp"
    "UsbMidiOutputPortListProvider.cpp"
)

target_include_directories(usbMidiPortHandling PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(usbMidiPortHandling usbmidi midi rtmidi)