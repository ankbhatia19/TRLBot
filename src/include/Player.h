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
};


#endif //TRLBOT_PLAYER_H