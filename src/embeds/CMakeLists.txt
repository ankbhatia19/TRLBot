add_subdirectory(include)

target_sources(${CMAKE_PROJECT_NAME} PUBLIC
        MatchEmbeds.cpp
        PlayerEmbeds.cpp
        ScheduleEmbeds.cpp
        TeamEmbeds.cpp
        UtilityEmbeds.cpp
)

target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC include)