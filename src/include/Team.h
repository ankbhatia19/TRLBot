//
// Created by ankit on 8/24/22.
//

#ifndef TRLBOT_TEAM_H
#define TRLBOT_TEAM_H

#include <vector>
#include <dpp/dpp.h>
#include <SQLiteCpp/SQLiteCpp.h>

#include "Player.h"
#include "Match.h"
#include "Utilities.h"
using namespace std;

class Player;

class Team {
public:
    
    static void table_init(SQLite::Database& db);

    static Utilities::ErrorCode add_team(SQLite::Database& db, int64_t team_id);
    static Utilities::ErrorCode add_player(SQLite::Database& db, int64_t team_id, int64_t player_id);

    static vector<int64_t> get_players(SQLite::Database& db, int64_t team_id);
};


#endif //TRLBOT_TEAM_H