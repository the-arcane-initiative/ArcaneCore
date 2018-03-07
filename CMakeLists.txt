cmake_minimum_required(VERSION 3.2)
project(ArcaneCore)

# TODO: sort out flags based on the release mode
IF(WIN32)

    # TODO: control based on architecture?
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build/windows)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build/windows)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build/windows)

ELSEIF(APPLE)

    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build/mac)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build/mac)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build/mac)

    # TODO: apple compiler flags? should probably just use clang
    set(CMAKE_CXX_FLAGS "-std=c++11")

# TODO: linux if
ELSE()

    # TODO: control based on architecture?
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build/linux)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build/linux)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build/linux)

    # TODO: clean up compiler flags
    set(CMAKE_CXX_FLAGS "-Wall -std=c++11 -fPIC")

ENDIF()

#---------------------------------GET LIBRARIES---------------------------------

# TODO: Deus should have a cmake package
SET(DEUS_INCLUDE_DIRS ../Deus/src/cpp)
SET(DEUS_LIBPATHS ../Deus/build/linux)
IF(WIN32)
    SET(DEUS_LIBPATHS ../Deus/build/windows/Release)
ELSEIF(APPLE)
    SET(DEUS_LIBPATHS ../Deus/build/mac)
ENDIF()

# TODO: should just write a gtest find_package for windows
IF(NOT WIN32)
    find_package(GTest REQUIRED)
    find_package(benchmark REQUIRED)
ENDIF()

#----------------------------------BASE MODULE----------------------------------

include_directories(
    ${INCLUDE_DIRECTORIES}
    src/cpp
    ${DEUS_INCLUDE_DIRS}
)

set(BASE_SRC
    src/cpp/arcanecore/base/arg/Action.cpp
    src/cpp/arcanecore/base/arg/DefaultHelpFlag.cpp
    src/cpp/arcanecore/base/arg/Flag.cpp
    src/cpp/arcanecore/base/arg/Parser.cpp
    src/cpp/arcanecore/base/clock/ClockOperations.cpp
)

add_library(arcanecore_base STATIC ${BASE_SRC})

#-----------------------------------UNIT TESTS----------------------------------

# TODO: sort out
if(WIN32)
    set(ARC_UNIT_INCLUDE_DIRS
        "C:/Program Files/googletest-distribution/include"
    )
    set(ARC_UNIT_LIBPATHS
        "C:/Program Files/googletest-distribution/lib"
    )
    set(DEUS_UNIT_LIBS
        gtest
    )
ELSE()
    set(ARC_UNIT_INCLUDE_DIRS
        ${GTEST_INCLUDE_DIRS}
    )
    set(ARC_UNIT_LIBPATHS
        ""
    )
    set(DEUS_UNIT_LIBS
        ${GTEST_LIBRARIES}
        pthread
    )
ENDIF()

include_directories(
    ${INCLUDE_DIRECTORIES}
    tests/unit/cpp
    ${ARC_UNIT_INCLUDE_DIRS}
    ${DEUS_INCLUDE_DIRS}
)

link_directories(
    ${LINK_DIRECTORIES}
    ${ARC_UNIT_LIBPATHS}
    ${DEUS_LIBPATHS}
)

set(ARC_UNIT_INCLUDES
    tests/unit/cpp/Proto_UnitTest.cpp
    tests/unit/cpp/UnitTestsMain.cpp
)

add_executable(unit_tests ${ARC_UNIT_INCLUDES})

target_link_libraries(unit_tests
    arcanecore_base
    deus
    ${DEUS_UNIT_LIBS}
)
