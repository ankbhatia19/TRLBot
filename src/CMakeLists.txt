add_subdirectory(include)
add_subdirectory(commands)
add_subdirectory(embeds)

target_sources(${CMAKE_PROJECT_NAME} PUBLIC
        BallchasingClient.cpp
        Match.cpp
        Player.cpp
        RecordBook.cpp
        Team.cpp
        Utilities.cpp
)

target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC include)