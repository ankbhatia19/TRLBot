//
// Created by ankit on 10/7/24.
//

#ifndef TRLBOT_GAME_H
#define TRLBOT_GAME_H

#include "SQLiteCpp/SQLiteCpp.h"
#include "nlohmann/json.hpp"
#include "Utilities.h"
#include <iostream>

using json = nlohmann::json;

class Game {

public:

    static void table_init(SQLite::Database& db){
        try {
            db.exec(
                    "CREATE TABLE IF NOT EXISTS player_stats ("
                    "player_id INTEGER NOT NULL, "
                    "match_id INTEGER NOT NULL, "
                    "game_num INTEGER NOT NULL, "
                    "shots INTEGER, "
                    "shots_against INTEGER, "
                    "goals INTEGER, "
                    "goals_against INTEGER, "
                    "saves INTEGER, "
                    "assists INTEGER, "
                    "score INTEGER, "
                    "mvp BOOLEAN, "
                    "shooting_percentage REAL, "
                    "bpm INTEGER, "
                    "bcpm REAL, "
                    "avg_amount REAL, "
                    "amount_collected INTEGER, "
                    "amount_stolen INTEGER, "
                    "amount_collected_big INTEGER, "
                    "amount_stolen_big INTEGER, "
                    "amount_collected_small INTEGER, "
                    "amount_stolen_small INTEGER, "
                    "count_collected_big INTEGER, "
                    "count_stolen_big INTEGER, "
                    "count_collected_small INTEGER, "
                    "count_stolen_small INTEGER, "
                    "amount_overfill INTEGER, "
                    "amount_overfill_stolen INTEGER, "
                    "amount_used_while_supersonic INTEGER, "
                    "time_zero_boost REAL, "
                    "percent_zero_boost REAL, "
                    "time_full_boost REAL, "
                    "percent_full_boost REAL, "
                    "time_boost_0_25 REAL, "
                    "time_boost_25_50 REAL, "
                    "time_boost_50_75 REAL, "
                    "time_boost_75_100 REAL, "
                    "percent_boost_0_25 REAL, "
                    "percent_boost_25_50 REAL, "
                    "percent_boost_50_75 REAL, "
                    "percent_boost_75_100 REAL, "
                    "avg_speed INTEGER, "
                    "total_distance INTEGER, "
                    "time_supersonic_speed REAL, "
                    "time_boost_speed REAL, "
                    "time_slow_speed REAL, "
                    "time_ground REAL, "
                    "time_low_air REAL, "
                    "time_high_air REAL, "
                    "time_powerslide REAL, "
                    "count_powerslide INTEGER, "
                    "avg_powerslide_duration REAL, "
                    "avg_speed_percentage REAL, "
                    "percent_slow_speed REAL, "
                    "percent_boost_speed REAL, "
                    "percent_supersonic_speed REAL, "
                    "percent_ground REAL, "
                    "percent_low_air REAL, "
                    "percent_high_air REAL, "
                    "avg_distance_to_ball INTEGER, "
                    "avg_distance_to_ball_possession INTEGER, "
                    "avg_distance_to_ball_no_possession INTEGER, "
                    "avg_distance_to_mates INTEGER, "
                    "time_defensive_third REAL, "
                    "time_neutral_third REAL, "
                    "time_offensive_third REAL, "
                    "time_defensive_half REAL, "
                    "time_offensive_half REAL, "
                    "time_behind_ball REAL, "
                    "time_infront_ball REAL, "
                    "time_most_back REAL, "
                    "time_most_forward REAL, "
                    "time_closest_to_ball REAL, "
                    "time_farthest_from_ball REAL, "
                    "percent_defensive_third REAL, "
                    "percent_offensive_third REAL, "
                    "percent_neutral_third REAL, "
                    "percent_defensive_half REAL, "
                    "percent_offensive_half REAL, "
                    "percent_behind_ball REAL, "
                    "percent_infront_ball REAL, "
                    "percent_most_back REAL, "
                    "percent_most_forward REAL, "
                    "percent_closest_to_ball REAL, "
                    "percent_farthest_from_ball REAL, "
                    "demos_inflicted INTEGER, "
                    "demos_taken INTEGER, "
                    "PRIMARY KEY (player_id, match_id, game_num)"
                    ");"
            );
            std::cout << "PlayerStats table initialized successfully." << std::endl;
        } catch (const std::exception& e) {
            std::cerr << "Error initializing PlayerStats table: " << e.what() << std::endl;
        }
    }

    static void insert(SQLite::Database& db, int64_t player_id, int match_id, int game_num, json& stats);

private:

};


#endif //TRLBOT_GAME_H
