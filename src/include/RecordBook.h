//
// Created by ankit on 8/24/22.
//

#ifndef TRLBOT_RECORDBOOK_H
#define TRLBOT_RECORDBOOK_H

#include <vector>
#include <fstream>
#include <dpp/dpp.h>
#include <nlohmann/json.hpp>

#include "Match.h"
#include "Player.h"
#include "Team.h"

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

    static string getBotToken();
    static string getBallchasingToken();
};


#endif //TRLBOT_RECORDBOOK_H
