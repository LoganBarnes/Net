
################################################
# add gmock functionality
################################################

# prevent /MT MSVC flag
set( gtest_force_shared_crt ON CACHE BOOL "" FORCE )

add_subdirectory( ${THIRDPARTY}/googletest/googlemock )

# gmock and gtest header dir
set(
    GMOCK_INCLUDE_DIRS
    ${THIRDPARTY}/googletest/googletest/include
    ${THIRDPARTY}/googletest/googlemock/include
    )

# gmock lib
set( GMOCK_LIB       gmock            )
set( GMOCK_MAIN_LIB  gmock_main       )
set( GMOCK_BOTH_LIBS gmock gmock_main )

