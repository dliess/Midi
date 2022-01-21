add_library(MidiMediumDummy STATIC "MidiMediumDummy.cpp")
target_include_directories(MidiMediumDummy PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(MidiMediumDummy PUBLIC midimedium)