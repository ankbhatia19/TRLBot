//
// Created by ankit on 8/24/22.
//

#include "RecordBook.h"

map<unsigned long long, Match> RecordBook::schedule;
map<unsigned long long, Team> RecordBook::teams;
map<unsigned long long, Player> RecordBook::players;

void RecordBook::save_match(unsigned long long matchID) {

    std::thread save([matchID](){
        std::cout << "Beginning save process for match " << matchID << std::endl;
        // Download the match
        std::filesystem::path path{ "Matches" }; // creates a local matches folder
        path /= std::to_string(matchID); // Add a match file
        std::filesystem::create_directories(path.parent_path()); // add directories based on the object path

        // Save the downloaded data to a local file
        std::ofstream ofs(path);
        ofs << schedule[matchID].to_json();
        ofs.close();
    });

    save.detach();
}

void RecordBook::fill_schedule() {

    std::filesystem::path path{ "Matches" }; // creates a local matches folder
    std::filesystem::create_directories(path); // add directories based on the object path

    for (const auto& file : std::filesystem::directory_iterator("Matches")){
        std::ifstream f(file.path());
        nlohmann::json data = json::parse(f);
        std::cout << "Filling schedule with id " << data["id"] << std::endl;
        schedule.insert({data["id"], Match{data}});
    }

}

void RecordBook::save_player(unsigned long long id) {

    std::thread save([id](){
        std::cout << "Beginning save process for player " << id << std::endl;
        // Download the match
        std::filesystem::path path{ "Players" }; // creates a local players folder
        path /= std::to_string(id); // Add a player file
        std::filesystem::create_directories(path.parent_path()); // add directories based on the object path

        // Save the downloaded data to a local file
        std::ofstream ofs(path);
        ofs << players[id].to_json();
        ofs.close();
    });

    save.detach();
}

void RecordBook::fill_players() {

    std::filesystem::path path{ "Players" }; // creates a local players folder
    std::filesystem::create_directories(path); // add directories based on the object path

    for (const auto& file : std::filesystem::directory_iterator("Players")){
        std::ifstream f(file.path());
        nlohmann::json data = json::parse(f);
        std::cout << "Filling roster with id " << data["id"] << std::endl;
        players.insert({data["id"], Player{data}});
    }
}

void RecordBook::save_team(unsigned long long teamID) {

    std::thread save([teamID](){
        std::cout << "Beginning save process for team " << teamID << std::endl;
        // Download the match
        std::filesystem::path path{ "Teams" }; // creates a local teams folder
        path /= std::to_string(teamID); // Add a team file
        std::filesystem::create_directories(path.parent_path()); // add directories based on the object path

        // Save the downloaded data to a local file
        std::ofstream ofs(path);
        ofs << teams[teamID].to_json();
        ofs.close();
    });

    save.detach();
}

void RecordBook::fill_teams() {

    std::filesystem::path path{ "Teams" }; // creates a local teams folder
    std::filesystem::create_directories(path); // add directories based on the object path

    for (const auto& file : std::filesystem::directory_iterator("Teams")){
        std::ifstream f(file.path());
        nlohmann::json data = json::parse(f);
        std::cout << "Filling teams with id " << data["id"] << std::endl;
        teams.insert({data["id"], Team{data}});
    }
}
//
//void RecordBook::create_matches_table(SQLite::Database &db) {
//    db.exec("CREATE TABLE IF NOT EXISTS Matches ("
//            "id INTEGER PRIMARY KEY, "
//            "home_id INTEGER NOT NULL, "
//            "away_id INTEGER NOT NULL, "
//            "match_status INTEGER, "
//            "ballchasing_id TEXT, "
//            "match_time INTEGER, " // Unix timestamp
//    );


//}