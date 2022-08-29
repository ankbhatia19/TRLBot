//
// Created by ankit on 8/24/22.
//

#ifndef TRLBOT_PLAYER_H
#define TRLBOT_PLAYER_H

#include <vector>
#include <string>
#include <dpp/dpp.h>
#include "Match.h"
#include "Team.h"
using namespace std;
using namespace dpp;

class Team;
class Player {

public:
    Player(user profile);
    vector<string> aliases;

    user profile;
    Team* team = nullptr;

    int seasonGoals, seasonAssists, seasonSaves;
    double seasonAvgMVPR;
};


#endif //TRLBOT_PLAYER_H