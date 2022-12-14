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

add_executable(${PROJECT_NAME}
        main.cpp
        src/Player.cpp src/include/Player.h
        src/Match.cpp src/include/Match.h
        src/RecordBook.cpp src/include/RecordBook.h
        src/Team.cpp src/include/Team.h
        src/commands/TeamCommand.cpp src/commands/include/TeamCommand.h
        src/commands/MatchCommand.cpp src/commands/include/MatchCommand.h
        src/commands/PlayerCommand.cpp src/commands/include/PlayerCommand.h
        src/commands/ScheduleCommand.cpp src/commands/include/ScheduleCommand.h
        src/commands/PingCommand.cpp src/commands/include/PingCommand.h
        src/Utilities.cpp src/include/Utilities.h
        src/embeds/UtilityEmbeds.cpp src/embeds/include/UtilityEmbeds.h
        src/embeds/TeamEmbeds.cpp src/embeds/include/TeamEmbeds.h
        src/embeds/MatchEmbeds.cpp src/embeds/include/MatchEmbeds.h
        src/embeds/ScheduleEmbeds.cpp src/embeds/include/ScheduleEmbeds.h
        src/embeds/PlayerEmbeds.cpp src/embeds/include/PlayerEmbeds.h
        src/BallchasingClient.cpp src/include/BallchasingClient.h
        src/commands/HelpCommand.cpp src/commands/include/HelpCommand.h)

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

# Specify includes
target_include_directories(${PROJECT_NAME} PRIVATE
        libs/DPP/include
)

# Set C++ version
set_target_properties(${PROJECT_NAME} PROPERTIES
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED ON
)
