# AVAX Toolbox
# Avalanche AVAX P2P wallet, chat, marketplace and more
# Author: @REKTBuildr 

set(SOURCES
    avaxtod.cpp
)

set(HEADERS
    
)

# Run the GUI
add_executable(avax.to ${SOURCES} ${HEADERS})

# Link required libraries
target_link_libraries(avax.to 
    nlohmann_json::nlohmann_json
    pthread
    ssl
    crypto
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