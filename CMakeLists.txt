cmake_minimum_required(VERSION 3.22)
project(TRLBot VERSION 1.0 DESCRIPTION "A discord bot")
set(CMAKE_CXX_STANDARD 20)

# Add DPP as dependency
add_subdirectory(libs/DPP)

add_executable(${PROJECT_NAME}
        main.cpp
        src/Player.cpp src/include/Player.h
        src/Match.cpp src/include/Match.h
        src/Schedule.cpp src/include/Schedule.h
        src/Team.cpp src/include/Team.h
        src/Commands.cpp src/include/Commands.h
        src/Embeds.cpp src/include/Embeds.h
        src/CommandProcessor.cpp src/include/CommandProcessor.h
)

# Linking libraries
target_link_libraries(${PROJECT_NAME}
        dpp
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
