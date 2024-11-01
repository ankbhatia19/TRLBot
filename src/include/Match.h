//
// Created by ankit on 8/24/22.
//

#ifndef TRLBOT_MATCH_H
#define TRLBOT_MATCH_H

#include <iostream>
#include <algorithm>
#include <vector>
#include <ctime>
#include <random>
#include <dpp/dpp.h>
#include <SQLiteCpp/SQLiteCpp.h>

#include "Team.h"
#include "Game.h"

using namespace std;

class Match {
public:

    enum status {
        UNPLAYED = 0,
        PLAYED = 1
    };
    enum affiliation {
        HOME,
        AWAY,
        NONE
    };

    struct series_score {
        int64_t home_id;
        int64_t away_id;
        int home_score;
        int away_score;
    };

    static void table_init(SQLite::Database& db);

    static int64_t create(SQLite::Database& db, int64_t home_id, int64_t away_id);

    static void set_status(SQLite::Database& db, int64_t match_id, status stat);
    static void set_ballchasing_id(SQLite::Database& db, int64_t match_id, string ballchasing_id);
    static Match::status get_status(SQLite::Database &db, int64_t match_id);
    static std::string get_ballchasing_id(SQLite::Database &db, int64_t match_id);

    static int64_t get_team(SQLite::Database &db, int64_t match_id, Match::affiliation affilation);
    static bool has_id(SQLite::Database& db, int64_t match_id);
    static vector<Game::score> tally(SQLite::Database& db, int64_t match_id);
    static Match::series_score score(SQLite::Database& db, int64_t match_id);
};

#endif //TRLBOT_MATCH_H