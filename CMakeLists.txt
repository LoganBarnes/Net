cmake_minimum_required( VERSION 3.1.0 )
project( NetExamples )

option( STRICT_FLAGS "Use strict compile flags on Unix machines" OFF )
option( UNIT_TESTS   "Compile net unit tests"                    OFF )

set( SRC_DIR    ${PROJECT_SOURCE_DIR}/src        )
set( THIRDPARTY ${PROJECT_SOURCE_DIR}/thirdparty )


if ( UNIT_TESTS )

  include( ${SRC_DIR}/testing/GoogleTest.cmake )

endif( UNIT_TESTS )

# strict compile flags
if ( NOT MSVC AND STRICT_FLAGS )

  set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pedantic -Wall -Wextra -Wcast-align -Wcast-qual"            )
  set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wctor-dtor-privacy -Wdisabled-optimization -Wformat=2"      )
  set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Winit-self -Wmissing-declarations -Wmissing-include-dirs"   )
  set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wold-style-cast -Woverloaded-virtual -Wredundant-decls"     )
  set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wshadow -Wsign-conversion -Wsign-promo -Wstrict-overflow=5" )
  set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wswitch-default -Wundef -Werror -Wno-unused"                )

endif( )


add_subdirectory( ${SRC_DIR}/net )

set(
    SRC_FILES

    ${SRC_DIR}/helpers/Intersections.cpp
    )

set(
    INC_DIRS

    ${SRC_DIR}/helpers
    ${NET_INCLUDE_DIR}
    )

set(
    EXAMPLE_NAMES

    XOR
    Addition
    Intersection
    )

foreach( EXAMPLE ${EXAMPLE_NAMES} )

  set( EXEC_NAME run${EXAMPLE} )

  add_executable( ${EXEC_NAME} ${SRC_DIR}/exec/${EXAMPLE}.cpp ${SRC_FILES}  )

  target_include_directories( ${EXEC_NAME} SYSTEM PUBLIC ${THIRDPARTY}/glm  )
  target_include_directories( ${EXEC_NAME} PUBLIC ${INC_DIRS}               )
  target_link_libraries     ( ${EXEC_NAME}        ${NET_LIBRARY}            )
  set_property              ( TARGET ${EXEC_NAME} PROPERTY CXX_STANDARD 14  )

endforeach( EXAMPLE )


if ( UNIT_TESTS )

  include( ${SRC_DIR}/testing/Testing.cmake )

endif( UNIT_TESTS )




