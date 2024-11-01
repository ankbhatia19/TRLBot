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

    struct score{
        int game_num;
        int home_goals;
        int away_goals;
    };

    static void table_init(SQLite::Database& db);

    static void insert(SQLite::Database& db, int64_t player_id, int match_id, int game_num, json& stats);

private:

};


#endif //TRLBOT_GAME_H
