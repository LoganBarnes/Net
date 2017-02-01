cmake_minimum_required( VERSION 3.1.0 )
project( Net )


set(
    SRC_FILES
    ${SRC_FILES}

    ${CMAKE_CURRENT_SOURCE_DIR}/Neuron.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Net.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ConnectedNet.cpp
    )

set( NET_INC ${CMAKE_CURRENT_SOURCE_DIR} )
set( NET_LIB net )

add_library( ${NET_LIB} ${SRC_FILES} )

target_include_directories( ${NET_LIB} PUBLIC ${NET_INC} )
target_compile_features   ( ${NET_LIB} PRIVATE cxx_range_for )

set( NET_INCLUDE_DIR ${NET_INC} PARENT_SCOPE )
set( NET_LIBRARY     ${NET_LIB} PARENT_SCOPE )
