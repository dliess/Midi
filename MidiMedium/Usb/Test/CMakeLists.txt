set(TARGET_NAME "testVirtualPorts")

add_executable(${TARGET_NAME} "main.cpp") 
target_link_libraries(${TARGET_NAME} usbmidi)