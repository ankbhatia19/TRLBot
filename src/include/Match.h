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

using namespace std;

class Match {
public:
    Match(unsigned long long homeID, unsigned long long awayID);
    Match(nlohmann::json json);
    Match();

    enum status {
        UNPLAYED = 0,
        PLAYED = 1
    };
    enum affiliation {
        HOME,
        AWAY,
        NONE
    };
    struct score{
        int game_num;
        int64_t home_team;
        int64_t away_team;
        int homeGoals;
        int awayGoals;
    };
    std::map<int, vector<score>> matchScores;

    unsigned long long homeID, awayID;
    int id = -1;
    std::tm matchTime;
    status matchStatus;
    affiliation matchWinner;

    string ballchasingID;

    score seriesScore;

    void determineWinner();

    nlohmann::json to_json();

    bool operator<(const Match& rhs) const;

    static void table_init(SQLite::Database& db);

    static int64_t create(SQLite::Database& db, int64_t home_id, int64_t away_id);

    static void set_status(SQLite::Database& db, int64_t match_id, status stat);
    static void set_ballchasing_id(SQLite::Database& db, int64_t match_id, string ballchasing_id);
    static Match::status get_status(SQLite::Database &db, int64_t match_id);
    static std::string get_ballchasing_id(SQLite::Database &db, int64_t match_id);

    static int64_t get_team(SQLite::Database &db, int64_t match_id, Match::affiliation affilation);
    static bool has_id(SQLite::Database& db, int64_t match_id);
    static vector<score> tally(SQLite::Database& db, int64_t match_id);

private:
    static vector<int> allIDs;
};

#endif //TRLBOT_MATCH_H