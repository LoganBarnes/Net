
################################################
# add test cases
################################################


set(
    TEST_NAMES

    ExampleUnitTests
    )

foreach( TEST ${TEST_NAMES} )

  set( EXEC_NAME run${TEST} )

  add_executable( ${EXEC_NAME} ${SRC_DIR}/testing/${TEST}.cpp )

  target_include_directories( ${EXEC_NAME} SYSTEM PUBLIC ${THIRDPARTY}/glm ${GMOCK_INCLUDE_DIRS} )
  target_include_directories( ${EXEC_NAME} PUBLIC ${INC_DIRS}                                    )
  target_link_libraries     ( ${EXEC_NAME}        ${NET_LIBRARY} ${GMOCK_BOTH_LIBS}              )
  set_property              ( TARGET ${EXEC_NAME} PROPERTY CXX_STANDARD 14                       )

endforeach( TEST )
