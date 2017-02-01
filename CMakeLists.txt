cmake_minimum_required( VERSION 3.1.0 )
project( NetExamples )

option( STRICT_FLAGS "Use strict compile flags on Unix machines" OFF )

set( SRC_DIR ${PROJECT_SOURCE_DIR}/src )

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
    EXAMPLE_NAMES

    XOR
    Addition
    Intersection
    )

foreach( EXAMPLE ${EXAMPLE_NAMES} )

  set( EXEC_NAME run${EXAMPLE} )

  add_executable( ${EXEC_NAME} ${SRC_DIR}/exec/${EXAMPLE}.cpp )

  target_include_directories( ${EXEC_NAME} PUBLIC  ${NET_INCLUDE_DIR} )
  target_link_libraries     ( ${EXEC_NAME}         ${NET_LIBRARY}     )
  target_compile_features   ( ${EXEC_NAME} PRIVATE cxx_range_for      )

endforeach( EXAMPLE )


