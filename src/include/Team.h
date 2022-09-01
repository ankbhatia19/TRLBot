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
    Team(role team);
    ~Team();
    vector<Player> players;

    int wins, losses;
    int differential;
    role team;

    int getPlayer(snowflake id);
    bool hasPlayer(snowflake id);

};


#endif //TRLBOT_TEAM_H