find_package(GTest REQUIRED)
find_package(GMock REQUIRED)
include_directories(${GMOCK_INCLUDE_DIRS})

set(TARGET_NAME "midiTests")

set(SOURCES "UT_TransportHandling.cpp"
)

add_executable(${TARGET_NAME} ${SOURCES})

set_target_properties(${TARGET_NAME} PROPERTIES
   DEBUG_POSTFIX ${CMAKE_DEBUG_POSTFIX}
)

target_link_libraries(${TARGET_NAME}
    GTest::GTest
    GTest::Main
    ${GMOCK_LIBRARIES}
    midi
)

add_test(NAME ${TARGET_NAME} COMMAND ${TARGET_NAME})