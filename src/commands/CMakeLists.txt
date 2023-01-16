add_subdirectory(include)

target_sources(${CMAKE_PROJECT_NAME} PUBLIC
        HelpCommand.cpp
        MatchCommand.cpp
        PingCommand.cpp
        PlayerCommand.cpp
        ScheduleCommand.cpp
        TeamCommand.cpp
)

target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC include)