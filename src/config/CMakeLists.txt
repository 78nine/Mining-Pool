cmake_minimum_required(VERSION 3.19)

add_library(config STATIC src/config/config_impl.cpp
                          src/config/validator.cpp
                          src/config/config_api_impl.cpp)
                    
target_include_directories(config
    PUBLIC 
        $<INSTALL_INTERFACE:inc>    
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/inc>
    PRIVATE src
)

target_link_libraries(config common nlohmann_json::nlohmann_json)