cmake_minimum_required( VERSION 3.1.0 )
project( NetExamples )

option( STRICT_FLAGS "Use strict compile flags on Unix machines" OFF )
option( UNIT_TESTS   "Compile net unit tests"                    OFF )

set( SRC_DIR    ${PROJECT_SOURCE_DIR}/src        )
set( THIRDPARTY ${PROJECT_SOURCE_DIR}/thirdparty )


# strict compile flags
if ( NOT MSVC AND STRICT_FLAGS )

  set( INTENSE_FLAGS "${INTENSE_FLAGS} -pedantic -Wall -Wextra -Wcast-align -Wcast-qual"            )
  set( INTENSE_FLAGS "${INTENSE_FLAGS} -Wctor-dtor-privacy -Wdisabled-optimization -Wformat=2"      )
  set( INTENSE_FLAGS "${INTENSE_FLAGS} -Winit-self -Wmissing-declarations -Wmissing-include-dirs"   )
  set( INTENSE_FLAGS "${INTENSE_FLAGS} -Wold-style-cast -Woverloaded-virtual -Wredundant-decls"     )
  set( INTENSE_FLAGS "${INTENSE_FLAGS} -Wshadow -Wsign-conversion -Wsign-promo -Wstrict-overflow=5" )
  set( INTENSE_FLAGS "${INTENSE_FLAGS} -Wswitch-default -Wundef -Werror -Wno-unused"                )

endif( )


add_subdirectory( ${SRC_DIR}/net )

set(
    SRC_FILES

    ${SRC_DIR}/helpers/Intersections.cpp
    ${SRC_DIR}/exec/App.cpp
    )

set(
    INC_DIRS

    ${SRC_DIR}/helpers
    ${SRC_DIR}/exec
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
  add_dependencies          ( ${EXEC_NAME}        ${NET_LIBRARY}            )
  set_property              ( TARGET ${EXEC_NAME} PROPERTY CXX_STANDARD 14  )

  if ( INTENSE_FLAGS )
    set_target_properties( ${EXEC_NAME} PROPERTIES COMPILE_FLAGS ${INTENSE_FLAGS} )
  endif( )

endforeach( EXAMPLE )


if ( UNIT_TESTS )

  include( ${SRC_DIR}/testing/Testing.cmake )

endif( UNIT_TESTS )




