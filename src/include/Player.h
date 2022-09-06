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

class Team;
class Player {

public:
    Player(unsigned long long id);
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
        AVG_MVPR
    };

    vector<MatchStatistic> stats;
    int getStatistic(enum statistic stat);
    unsigned long long id;
    unsigned long long teamID = 0;

    bool containsAlias(string alias);
};


#endif //TRLBOT_PLAYER_H