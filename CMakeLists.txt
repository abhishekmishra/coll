cmake_minimum_required ( VERSION 3.9 )

project ( coll C )

# Set the project source dir (just convention)
set( COLL_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR} )
set( COLL_BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR} )

# It's useful to setup the current version of our code in the build system
# using a `semver` style
set ( COLL_VERSION_MAJOR 0 )
set ( COLL_VERSION_MINOR 1 )
set ( COLL_VERSION_PATCH 0 )

# Lists
# Setup the list of source files
set( COLL_SOURCES 
  src/arraylist.h
  src/arraylist.c
)

set( COLL_TEST_SOURCES 
  test/test_all.c
  test/test_arraylist.c
)

add_library ( ${PROJECT_NAME} ${COLL_SOURCES} )
set_property(TARGET ${PROJECT_NAME} PROPERTY C_STANDARD 11)

# TEST Configuration
option(ENABLE_TESTS "Perform unit tests after build" OFF)
set ( COLL_TEST_PROGRAM_NAME coll_test )
if (ENABLE_TESTS)
  add_executable(${COLL_TEST_PROGRAM_NAME} ${COLL_TEST_SOURCES})
  set_property(TARGET ${COLL_TEST_PROGRAM_NAME} PROPERTY C_STANDARD 11)
  include_directories( test )
  include_directories( src )

  # cmocka
  find_package(cmocka CONFIG REQUIRED)
  target_include_directories(${COLL_TEST_PROGRAM_NAME} PRIVATE ${CMOCKA_INCLUDE_DIR})
  target_link_libraries(${COLL_TEST_PROGRAM_NAME} PRIVATE ${CMOCKA_LIBRARIES})

  target_link_libraries(${COLL_TEST_PROGRAM_NAME} PUBLIC ${PROJECT_NAME})
  add_test( all_coll_tests ${COLL_TEST_PROGRAM_NAME} )
  enable_testing()
endif(ENABLE_TESTS)

# Package Configuration
export(TARGETS coll NAMESPACE coll:: FILE collConfig.cmake)
set(CMAKE_EXPORT_PACKAGE_REGISTRY ON)
export(PACKAGE coll)
