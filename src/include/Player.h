//
// Created by ankit on 8/24/22.
//

#ifndef TRLBOT_PLAYER_H
#define TRLBOT_PLAYER_H

#include <vector>
#include <string>
#include <dpp/dpp.h>
#include "SQLiteCpp/SQLiteCpp.h"
#include "Utilities.h"
#include "Match.h"
#include "Team.h"

using namespace std;

class Player {

public:
    Player(unsigned long long id);
    Player(nlohmann::json);
    Player();
    vector<string> aliases;

    struct MatchStatistic{
        int matchID;
        int shots;
        int goals;
        int saves;
        int assists;
    };

    enum statistic {
        SHOTS,
        GOALS,
        SAVES,
        ASSISTS,
        NUM_GAMES,
        AVG_MVPR
    };
    vector<MatchStatistic> stats;

    int getStatistic(statistic stat);
    double getStatisticAvg(statistic stat);
    unsigned long long id;
    unsigned long long teamID;

    bool containsAlias(string alias);

    nlohmann::json to_json();

    static void table_init(SQLite::Database& db);

    static Utilities::ErrorCode add_name(SQLite::Database& db, int64_t player_id, const string& username);
    static Utilities::ErrorCode add_team(SQLite::Database& db, int64_t player_id, int64_t team_id);

    static int64_t get_id(SQLite::Database& db, const string& username);
    static int64_t get_team(SQLite::Database& db, const string& username);
    static int64_t get_team(SQLite::Database& db, int64_t player_id);

    static bool has_id(SQLite::Database& db, int64_t player_id);

    static double get_war(SQLite::Database& db, int64_t player_id);

    static vector<string> get_names(SQLite::Database& db, int64_t player_id);

    double get_statistic(SQLite::Database& db, statistic stat);
    double get_statistic(SQLite::Database& db, statistic stat, int64_t match_id);
};


#endif //TRLBOT_PLAYER_H