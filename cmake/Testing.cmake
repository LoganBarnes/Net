
####################################################
# add test cases
####################################################
set(
    TEST_SOURCE

    ${SRC_DIR}/testing/ExampleUnitTests.cpp
    )


####################################################
# Download and unpack googletest at configure time
####################################################
DownloadProject( ${CMAKE_DIR}/CMakeLists.txt.googletest googletest )


####################################################
# Build googletest source with rest of project
####################################################

# Prevent overriding the parent project's compiler/linker settings on Windows
set( gtest_force_THIRDPARTY_crt ON CACHE BOOL "" FORCE )

# Add googlemock directly to the project build
add_subdirectory(
                 ${CMAKE_BINARY_DIR}/googletest-src/googlemock
                 ${CMAKE_BINARY_DIR}/googletest-build
                 )

# gmock and gtest header dirs
set(
    GMOCK_INCLUDE_DIRS

    ${CMAKE_BINARY_DIR}/googletest-src/googletest/include
    ${CMAKE_BINARY_DIR}/googletest-src/googlemock/include
    )


####################################################
# Build googletest source with rest of project
####################################################
set( TEST_NAME test${PROJECT_NAME} )

add_executable( ${TEST_NAME} ${TEST_SOURCE} )

target_include_directories( ${TEST_NAME} SYSTEM PUBLIC ${GLM_INCLUDE_DIR} ${GMOCK_INCLUDE_DIRS} )
target_include_directories( ${TEST_NAME} PUBLIC ${INC_DIRS}                                     )
target_link_libraries     ( ${TEST_NAME}        ${NET_LIBRARY} gmock gmock_main                 )
add_dependencies          ( ${TEST_NAME}        ${NET_LIBRARY} gmock gmock_main                 )
set_property              ( TARGET ${TEST_NAME} PROPERTY CXX_STANDARD 14                        )

if ( INTENSE_FLAGS )
  set_target_properties( ${EXEC_NAME} PROPERTIES COMPILE_FLAGS ${INTENSE_FLAGS} )
endif( )
