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

class Player;

class Team {
public:
    Team();
    Team(unsigned long long id);

    int wins, losses;
    int differential;
    unsigned long long id;

    map<unsigned long long, Player> members;
};


#endif //TRLBOT_TEAM_H