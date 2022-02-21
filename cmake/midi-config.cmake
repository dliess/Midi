get_filename_component(midi_CMAKE_DIR "${CMAKE_CURRENT_LIST_FILE}" PATH)
include(CMakeFindDependencyMacro)

list(APPEND CMAKE_MODULE_PATH "${midi_CMAKE_DIR}")

find_dependency(rtmidi REQUIRED)
find_dependency(fmt REQUIRED)
find_dependency(mpark_variant REQUIRED)

if(NOT TARGET MIDI::midi)
    include("${midi_CMAKE_DIR}/Midi-targets.cmake")
endif()