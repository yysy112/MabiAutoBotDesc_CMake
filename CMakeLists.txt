cmake_minimum_required(VERSION 3.14)

project(MabiAutoBotDesc LANGUAGES CXX VERSION 1.0.0)

find_package(tinyxml2 REQUIRED)

if ("${CMAKE_BUILD_TYPE}" STREQUAL "")
    set(CMAKE_BUILD_TYPE Debug CACHE INTERNAL "")
endif()

set(MabiAutoBotDesc_PublicHeaders
	src/mabiRace.h
	src/mabiAutoBot.h
	src/mabiBuffHelper.h
	src/mabiAutoBotHelper.h
)

set(MabiAutoBotDesc_PublicSources
	src/mabiBuffHelper.cpp
	src/mabiAutoBotHelper.cpp
	src/main.cpp
)

source_group(
    src/
    FILES
    ${MabiAutoBotDesc_PublicHeaders}
    ${MabiAutoBotDesc_PublicSources}
)

add_compile_definitions(RESOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}/resource/")

add_executable(MabiAutoBotDesc
    ${MabiAutoBotDesc_PublicHeaders}
    ${MabiAutoBotDesc_PublicSources}
	src/main.cpp)

if(tinyxml2_FOUND)
    target_link_libraries(MabiAutoBotDesc tinyxml2::tinyxml2)
else(tinyxml2_FOUND)
    message(FATAL_ERROR ”tinyxml2 library not found”)
endif(tinyxml2_FOUND)