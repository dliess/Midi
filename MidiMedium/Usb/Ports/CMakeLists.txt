cmake_minimum_required(VERSION 2.8.12)

set (CMAKE_CXX_STANDARD 11)

add_subdirectory(Test EXCLUDE_FROM_ALL)

add_library(usbMidiPortHandling STATIC 
    "UsbMidiInputPortListProvider.cpp"
    "UsbMidiOutputPortListProvider.cpp"
#    "UsbMidiDeviceManager.cpp"
    "Usb2DeviceMapper.cpp"
    "DeviceConfigParser.cpp"
)

target_include_directories(usbMidiPortHandling PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(usbMidiPortHandling rtmidi arduinoJson tools usbmidi midi)