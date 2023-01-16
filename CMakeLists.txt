cmake_minimum_required(VERSION 3.22)
project(TRLBot VERSION 1.0 DESCRIPTION "A discord bot")
set(CMAKE_CXX_STANDARD 20)

set(OPENSSL_USE_STATIC_LIBS TRUE)
find_package(OpenSSL REQUIRED)
if(OPENSSL_FOUND)
    set(HTTPLIB_IS_USING_OPENSSL TRUE)
endif()

# Add DPP as dependency
add_subdirectory(libs/DPP)

# Add cpp-httplib as a dependency
add_subdirectory(libs/cpp-httplib)

# Create target
add_executable(${PROJECT_NAME} main.cpp)

# Add all project dirs
add_subdirectory(src)

# Set C++ version
set_target_properties(${PROJECT_NAME} PROPERTIES
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED ON
)

# Copy quotes json
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/quotes.json
        ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)

# Copy config json
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/config.json
        ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)

# Linking libraries
target_link_libraries(${PROJECT_NAME} PUBLIC
        $<$<BOOL:${HTTPLIB_IS_USING_OPENSSL}>:OpenSSL::SSL>
        $<$<BOOL:${HTTPLIB_IS_USING_OPENSSL}>:OpenSSL::Crypto>
        httplib
        dpp
)

target_compile_definitions(${PROJECT_NAME} PUBLIC
        $<$<BOOL:${HTTPLIB_IS_USING_OPENSSL}>:CPPHTTPLIB_OPENSSL_SUPPORT>
)

# Specify includes for dpp
target_include_directories(${PROJECT_NAME} PUBLIC
        libs/DPP/include
)