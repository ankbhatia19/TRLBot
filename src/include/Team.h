//
// Created by ankit on 8/24/22.
//

#ifndef TRLBOT_TEAM_H
#define TRLBOT_TEAM_H

#include <vector>
#include <dpp/dpp.h>
#include "Player.h"
#include "Match.h"
using namespace std;
using namespace dpp;

class Player;
class Match;

class Team {
public:
    Team(snowflake id);
    ~Team();
    vector<Player> players;
    vector<Match> matches;
    int wins, losses, draws;
    snowflake id;

    bool hasPlayer(Player player);
    bool hasMatch(Match match);
};


#endif //TRLBOT_TEAM_H