//
// Created by ankit on 10/7/24.
//

#include "Game.h"


void Game::insert(SQLite::Database& db, int64_t player_id, int match_id, int game_num, json& stats) {
    SQLite::Statement query(db,
        "INSERT OR REPLACE INTO player_stats ("
        "player_id, match_id, game_num, shots, shots_against, goals, goals_against, saves, assists, score, mvp, "
        "shooting_percentage, bpm, bcpm, avg_amount, amount_collected, amount_stolen, amount_collected_big, "
        "amount_stolen_big, amount_collected_small, amount_stolen_small, count_collected_big, count_stolen_big, "
        "count_collected_small, count_stolen_small, amount_overfill, amount_overfill_stolen, "
        "amount_used_while_supersonic, time_zero_boost, percent_zero_boost, time_full_boost, percent_full_boost, "
        "time_boost_0_25, time_boost_25_50, time_boost_50_75, time_boost_75_100, percent_boost_0_25, "
        "percent_boost_25_50, percent_boost_50_75, percent_boost_75_100, avg_speed, total_distance, "
        "time_supersonic_speed, time_boost_speed, time_slow_speed, time_ground, time_low_air, time_high_air, "
        "time_powerslide, count_powerslide, avg_powerslide_duration, avg_speed_percentage, percent_slow_speed, "
        "percent_boost_speed, percent_supersonic_speed, percent_ground, percent_low_air, percent_high_air, "
        "avg_distance_to_ball, avg_distance_to_ball_possession, avg_distance_to_ball_no_possession, "
        "avg_distance_to_mates, time_defensive_third, time_neutral_third, time_offensive_third, "
        "time_defensive_half, time_offensive_half, time_behind_ball, time_infront_ball, time_most_back, "
        "time_most_forward, time_closest_to_ball, time_farthest_from_ball, percent_defensive_third, "
        "percent_offensive_third, percent_neutral_third, percent_defensive_half, percent_offensive_half, "
        "percent_behind_ball, percent_infront_ball, percent_most_back, percent_most_forward, percent_closest_to_ball, "
        "percent_farthest_from_ball, demos_inflicted, demos_taken) "
        "VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?);");

    Utilities::bind_val(query, 1, player_id);
    Utilities::bind_val(query, 2, match_id);
    Utilities::bind_val(query, 3, game_num);

    // Bind core stats
    Utilities::bind_val(query, 4, stats["core"]["shots"]);
    Utilities::bind_val(query, 5, stats["core"]["shots_against"]);
    Utilities::bind_val(query, 6, stats["core"]["goals"]);
    Utilities::bind_val(query, 7, stats["core"]["goals_against"]);
    Utilities::bind_val(query, 8, stats["core"]["saves"]);
    Utilities::bind_val(query, 9, stats["core"]["assists"]);
    Utilities::bind_val(query, 10, stats["core"]["score"]);
    Utilities::bind_val(query, 11, stats["core"]["mvp"]);
    Utilities::bind_val(query, 12, stats["core"]["shooting_percentage"]);

    // Bind boost stats
    Utilities::bind_val(query, 13, stats["boost"]["bpm"]);
    Utilities::bind_val(query, 14, stats["boost"]["bcpm"]);
    Utilities::bind_val(query, 15, stats["boost"]["avg_amount"]);
    Utilities::bind_val(query, 16, stats["boost"]["amount_collected"]);
    Utilities::bind_val(query, 17, stats["boost"]["amount_stolen"]);
    Utilities::bind_val(query, 18, stats["boost"]["amount_collected_big"]);
    Utilities::bind_val(query, 19, stats["boost"]["amount_stolen_big"]);
    Utilities::bind_val(query, 20, stats["boost"]["amount_collected_small"]);
    Utilities::bind_val(query, 21, stats["boost"]["amount_stolen_small"]);
    Utilities::bind_val(query, 22, stats["boost"]["count_collected_big"]);
    Utilities::bind_val(query, 23, stats["boost"]["count_stolen_big"]);
    Utilities::bind_val(query, 24, stats["boost"]["count_collected_small"]);
    Utilities::bind_val(query, 25, stats["boost"]["count_stolen_small"]);
    Utilities::bind_val(query, 26, stats["boost"]["amount_overfill"]);
    Utilities::bind_val(query, 27, stats["boost"]["amount_overfill_stolen"]);
    Utilities::bind_val(query, 28, stats["boost"]["amount_used_while_supersonic"]);
    Utilities::bind_val(query, 29, stats["boost"]["time_zero_boost"]);
    Utilities::bind_val(query, 30, stats["boost"]["percent_zero_boost"]);
    Utilities::bind_val(query, 31, stats["boost"]["time_full_boost"]);
    Utilities::bind_val(query, 32, stats["boost"]["percent_full_boost"]);
    Utilities::bind_val(query, 33, stats["boost"]["time_boost_0_25"]);
    Utilities::bind_val(query, 34, stats["boost"]["time_boost_25_50"]);
    Utilities::bind_val(query, 35, stats["boost"]["time_boost_50_75"]);
    Utilities::bind_val(query, 36, stats["boost"]["time_boost_75_100"]);
    Utilities::bind_val(query, 37, stats["boost"]["percent_boost_0_25"]);
    Utilities::bind_val(query, 38, stats["boost"]["percent_boost_25_50"]);
    Utilities::bind_val(query, 39, stats["boost"]["percent_boost_50_75"]);
    Utilities::bind_val(query, 40, stats["boost"]["percent_boost_75_100"]);

    // Bind movement stats
    Utilities::bind_val(query, 41, stats["movement"]["avg_speed"]);
    Utilities::bind_val(query, 42, stats["movement"]["total_distance"]);
    Utilities::bind_val(query, 43, stats["movement"]["time_supersonic_speed"]);
    Utilities::bind_val(query, 44, stats["movement"]["time_boost_speed"]);
    Utilities::bind_val(query, 45, stats["movement"]["time_slow_speed"]);
    Utilities::bind_val(query, 46, stats["movement"]["time_ground"]);
    Utilities::bind_val(query, 47, stats["movement"]["time_low_air"]);
    Utilities::bind_val(query, 48, stats["movement"]["time_high_air"]);
    Utilities::bind_val(query, 49, stats["movement"]["time_powerslide"]);
    Utilities::bind_val(query, 50, stats["movement"]["count_powerslide"]);
    Utilities::bind_val(query, 51, stats["movement"]["avg_powerslide_duration"]);
    Utilities::bind_val(query, 52, stats["movement"]["avg_speed_percentage"]);
    Utilities::bind_val(query, 53, stats["movement"]["percent_slow_speed"]);
    Utilities::bind_val(query, 54, stats["movement"]["percent_boost_speed"]);
    Utilities::bind_val(query, 55, stats["movement"]["percent_supersonic_speed"]);
    Utilities::bind_val(query, 56, stats["movement"]["percent_ground"]);
    Utilities::bind_val(query, 57, stats["movement"]["percent_low_air"]);
    Utilities::bind_val(query, 58, stats["movement"]["percent_high_air"]);

    // Bind positioning stats
    Utilities::bind_val(query, 59, stats["positioning"]["avg_distance_to_ball"]);
    Utilities::bind_val(query, 60, stats["positioning"]["avg_distance_to_ball_possession"]);
    Utilities::bind_val(query, 61, stats["positioning"]["avg_distance_to_ball_no_possession"]);
    Utilities::bind_val(query, 62, stats["positioning"]["avg_distance_to_mates"]);
    Utilities::bind_val(query, 63, stats["positioning"]["time_defensive_third"]);
    Utilities::bind_val(query, 64, stats["positioning"]["time_neutral_third"]);
    Utilities::bind_val(query, 65, stats["positioning"]["time_offensive_third"]);
    Utilities::bind_val(query, 66, stats["positioning"]["time_defensive_half"]);
    Utilities::bind_val(query, 67, stats["positioning"]["time_offensive_half"]);
    Utilities::bind_val(query, 68, stats["positioning"]["time_behind_ball"]);
    Utilities::bind_val(query, 69, stats["positioning"]["time_infront_ball"]);
    Utilities::bind_val(query, 70, stats["positioning"]["time_most_back"]);
    Utilities::bind_val(query, 71, stats["positioning"]["time_most_forward"]);
    Utilities::bind_val(query, 72, stats["positioning"]["time_closest_to_ball"]);
    Utilities::bind_val(query, 73, stats["positioning"]["time_farthest_from_ball"]);
    Utilities::bind_val(query, 74, stats["positioning"]["percent_defensive_third"]);
    Utilities::bind_val(query, 75, stats["positioning"]["percent_offensive_third"]);
    Utilities::bind_val(query, 76, stats["positioning"]["percent_neutral_third"]);
    Utilities::bind_val(query, 77, stats["positioning"]["percent_defensive_half"]);
    Utilities::bind_val(query, 78, stats["positioning"]["percent_offensive_half"]);
    Utilities::bind_val(query, 79, stats["positioning"]["percent_behind_ball"]);
    Utilities::bind_val(query, 80, stats["positioning"]["percent_infront_ball"]);
    Utilities::bind_val(query, 81, stats["positioning"]["percent_most_back"]);
    Utilities::bind_val(query, 82, stats["positioning"]["percent_most_forward"]);
    Utilities::bind_val(query, 83, stats["positioning"]["percent_closest_to_ball"]);
    Utilities::bind_val(query, 84, stats["positioning"]["percent_farthest_from_ball"]);

    // Bind demo stats
    Utilities::bind_val(query, 85, stats["demo"]["inflicted"]);
    Utilities::bind_val(query, 86, stats["demo"]["taken"]);

    query.exec();
}

void Game::table_init(SQLite::Database &db) {
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
}
