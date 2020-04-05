cmake_minimum_required(VERSION 2.8.12)

set (CMAKE_CXX_STANDARD 17)

add_subdirectory(MidiMedium)

add_library(midi STATIC
    "Midi1Output.cpp"
    "Midi1Input.cpp"
    "MidiCCInputHandler.cpp"
    "XRpnInputHandler.cpp"
    "XRpnOutputHandler.cpp"
)

target_include_directories(midi PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}
    $<TARGET_PROPERTY:midimedium,INTERFACE_INCLUDE_DIRECTORIES>
)

target_link_libraries(midi
    PUBLIC mpark_variant
    PUBLIC fmt
)