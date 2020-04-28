project(koth-snake)

#**************************************************************************************************
# General cMake settings **************************************************************************
cmake_minimum_required(VERSION 3.5)

#**************************************************************************************************
# Find Package ************************************************************************************
find_library(USOCKETS_LIB uSockets)
find_package(ZLIB REQUIRED)
find_package(cxxopts REQUIRED)
find_package(fmt CONFIG REQUIRED)

find_package(unofficial-libuv CONFIG REQUIRED)

#**************************************************************************************************
# Include *****************************************************************************************
include_directories(${_VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/backend/src)

# get_cmake_property(_variableNames VARIABLES)
# list (SORT _variableNames)
# foreach (_variableName ${_variableNames})
#     message(STATUS "${_variableName}=${${_variableName}}")
# endforeach()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

add_subdirectory(backend)



#**************************************************************************************************
# Build Tests *************************************************************************************
option(BUILD_UNIT_TESTS "Build the unit tests" ON)
if(BUILD_UNIT_TESTS)
  enable_testing()

  SET(TEST_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/tests/unit-main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/tests/unit-board.cpp
  )

  add_executable(unit-test ${TEST_SOURCES} ${SOURCES} ${TEST_HEADERS})
  target_link_libraries(unit-test PRIVATE -pthread -lstdc++fs)

  add_test(NAME "unit_test_default"
    COMMAND unit-test
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
  )
endif()
