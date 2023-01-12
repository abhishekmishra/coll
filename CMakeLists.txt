cmake_minimum_required ( VERSION 3.9 )

project ( coll VERSION 0.1.0 LANGUAGES C )

# Export all symbols for shared linking to work on windows
# see https://stackoverflow.com/questions/37128555/getting-cmake-to-build-shared-library-for-msvc
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS 1)

# Set the project source dir (just convention)
set( COLL_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR} )
set( COLL_BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR} )

# version numbers in semver style
# matching the release tag version
set ( COLL_VERSION_MAJOR "0" )
set ( COLL_VERSION_MINOR "1" )
set ( COLL_VERSION_PATCH "0" )
set ( COLL_VERSION_PRERELEASE "-alpha.0" )
set ( COLL_VERSION_STR ${COLL_VERSION_MAJOR}.${COLL_VERSION_MINOR}.${COLL_VERSION_PATCH}${COLL_VERSION_PRERELEASE})

# https://stackoverflow.com/a/47260387/9483968
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Lists
# Setup the list of source files
set( COLL_SOURCES 
  src/coll_arraylist.h
  src/coll_arraylist.c
  src/coll_arraylist_map.h
  src/coll_arraylist_map.c
)

set( COLL_TEST_SOURCES 
  test/test_all.c
  test/test_coll_arraylist.h
  test/test_coll_arraylist.c
  test/test_coll_arraylist_map.h
  test/test_coll_arraylist_map.c
)

if(CMAKE_HOST_SYSTEM_NAME STREQUAL "Darwin" AND LUA_FROM_PKGCONFIG)
  find_package(PkgConfig REQUIRED)
  pkg_check_modules(lua REQUIRED IMPORTED_TARGET lua)
endif()

# Test enable lua bindings
option (ENABLE_LUA "Enable LUA bindings for collections" OFF)
if (ENABLE_LUA)
  include(FindLua)
  if (LUA_FOUND)
    message("-- lua found at ${LUA_LIBRARIES} ${LUA_INCLUDE_DIR}")
    message("-- lua version is ${LUA_VERSION_STRING}")
    add_compile_definitions(LUA_ENABLED)
    set( COLL_SOURCES ${COLL_SOURCES}
      src/coll_lualib_arraylist.c
    )
  endif (LUA_FOUND)
endif (ENABLE_LUA)

add_library ( ${PROJECT_NAME} SHARED ${COLL_SOURCES} )
set_property(TARGET ${PROJECT_NAME} PROPERTY C_STANDARD 11)
target_include_directories ( ${PROJECT_NAME} PUBLIC src)

if (ENABLE_LUA)
  if (LUA_FOUND)
  target_include_directories(${PROJECT_NAME} PRIVATE ${LUA_INCLUDE_DIR})
  target_link_libraries(${PROJECT_NAME} PRIVATE ${LUA_LIBRARIES})    
  endif (LUA_FOUND)
endif (ENABLE_LUA)

# TEST Configuration
option(ENABLE_TESTS "Perform unit tests after build" OFF)
set ( COLL_TEST_PROGRAM_NAME coll_test )
if (ENABLE_TESTS)
  if (ENABLE_LUA)
    if (LUA_FOUND)
      set( COLL_TEST_SOURCES ${COLL_TEST_SOURCES}
        test/test_coll_lualib_arraylist.c
        test/test_coll_lualib_arraylist.h
      )   
    endif (LUA_FOUND)
  endif (ENABLE_LUA)

  add_executable(${COLL_TEST_PROGRAM_NAME} ${COLL_TEST_SOURCES})
  set_property(TARGET ${COLL_TEST_PROGRAM_NAME} PROPERTY C_STANDARD 11)
  include_directories( test )
  include_directories( src )

  # cmocka
  find_package(cmocka CONFIG REQUIRED)
  target_link_libraries(${COLL_TEST_PROGRAM_NAME} PRIVATE cmocka::cmocka)

  if (ENABLE_LUA)
    if (LUA_FOUND)
    target_include_directories(${COLL_TEST_PROGRAM_NAME} PRIVATE ${LUA_INCLUDE_DIR})
    target_link_libraries(${COLL_TEST_PROGRAM_NAME} PRIVATE ${LUA_LIBRARIES})    
    endif (LUA_FOUND)
  endif (ENABLE_LUA)

  target_link_libraries(${COLL_TEST_PROGRAM_NAME} PUBLIC ${PROJECT_NAME})
  add_test( all_coll_tests ${COLL_TEST_PROGRAM_NAME} )
  enable_testing()
endif(ENABLE_TESTS)

# Package Configuration
export(TARGETS coll NAMESPACE coll:: FILE collConfig.cmake)
set(CMAKE_EXPORT_PACKAGE_REGISTRY ON)
export(PACKAGE coll)
