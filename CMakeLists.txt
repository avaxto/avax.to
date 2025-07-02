# AVAX Toolbox
# Avalanche AVAX P2P wallet, chat, marketplace and more
# Author: @REKTBuildr 

cmake_minimum_required(VERSION 3.28.3)

project(avax.to)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)
#set(CMAKE_VERBOSE_MAKEFILE true)

if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    set(MACOSX TRUE)
endif()

include(FetchContent)

FetchContent_Declare(
nlohmann_json
GIT_REPOSITORY https://github.com/nlohmann/json
GIT_TAG  v3.11.3
GIT_SHALLOW TRUE)

FetchContent_MakeAvailable(nlohmann_json)

include_directories(include)
include_directories(/usr/local/include)
include_directories(${json_SOURCE_DIR}/include)


if(MACOSX)
    include_directories(/opt/homebrew/include)
    link_directories(/usr/local/lib)
    link_directories(/opt/homebrew/lib)
    #set(CMAKE_C_COMPILER "/opt/local/bin/gcc")
    #set(CMAKE_CXX_COMPILER "/opt/local/bin/g++")
endif()

add_subdirectory(src)


