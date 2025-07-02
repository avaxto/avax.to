# AVAX Toolbox
# Avalanche AVAX P2P wallet, chat, marketplace and more
# Author: @REKTBuildr 

set(SOURCES
    avaxtod.cpp
)

set(HEADERS
    
)

if(DEFINED ENV{LIBBITCOIN_PATH})
    set(LIBBITCOIN_PATH $ENV{LIBBITCOIN_PATH})
    message(STATUS "Using LIBBITCOIN_PATH from environment: ${LIBBITCOIN_PATH}")
else()    
    if(WIN32)
        set(USER_HOME_DIR $ENV{USERPROFILE})
    else()
        set(USER_HOME_DIR $ENV{HOME})
    endif()
    set(LIBBITCOIN_PATH "${USER_HOME_DIR}/libbitcoin-build")
    message(STATUS "LIBBITCOIN_PATH not set, using default: ${LIBBITCOIN_PATH}")
endif()

include_directories(${LIBBITCOIN_PATH}/include)
link_directories(${LIBBITCOIN_PATH}/lib)

if(MACOSX)
    include_directories($ENV{HOME}/avaxto/github/libavax/include)
    link_directories($ENV{HOME}/avaxto/github/libavax/build/mac/src)
    include_directories(/opt/homebrew/include)
    link_directories(/opt/homebrew/lib)
    include_directories(
            /opt/homebrew/opt/libpq/include
    )
    link_directories(
            /opt/homebrew/opt/libpq/lib
            /usr/local/lib
    )
else()
    include_directories($ENV{HOME}/webmaster/systems/avaxto/github/libavax/include)
    link_directories($ENV{HOME}/webmaster/systems/avaxto/github/libavax/build/linux/src)
endif()




add_executable(avax.to ${SOURCES} ${HEADERS})

target_link_libraries(avax.to 
    nlohmann_json::nlohmann_json
    pthread
    ssl
    crypto
    SQLite::SQLite3
)



if(MACOSX)
    # macOS specific libraries if needed
endif()

# Set compiler flags
target_compile_features(avax.to PRIVATE cxx_std_23)
target_compile_options(avax.to PRIVATE 
    -Wall 
    -Wextra 
    -O2
)

add_subdirectory(qt)