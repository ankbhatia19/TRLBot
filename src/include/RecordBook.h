//
// Created by ankit on 8/24/22.
//

#ifndef TRLBOT_RECORDBOOK_H
#define TRLBOT_RECORDBOOK_H

#include <vector>
#include <thread>
#include <dpp/dpp.h>
#include <nlohmann/json.hpp>
#include <SQLiteCpp/SQLiteCpp.h>

#include "Match.h"
#include "Player.h"
#include "Team.h"
#include "Game.h"

using namespace std;
using json = nlohmann::json;

class Match;
class Team;
class Player;

class RecordBook {
public:
    static map<unsigned long long, Match> schedule;
    static map<unsigned long long, Team> teams;
    static map<unsigned long long, Player> players;

    static void save_match(unsigned long long);
    static void save_team(unsigned long long);
    static void save_player(unsigned long long);


    static void fill_schedule();
    static void fill_teams();
    static void fill_players();
};


#endif //TRLBOT_RECORDBOOK_H
