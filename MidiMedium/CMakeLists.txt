add_subdirectory(Usb)
add_subdirectory(Dummy)

add_library(midimedium INTERFACE)
target_include_directories(midimedium INTERFACE ${CMAKE_CURRENT_LIST_DIR})
