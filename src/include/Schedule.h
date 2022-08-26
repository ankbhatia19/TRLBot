//
// Created by ankit on 8/24/22.
//

#ifndef TRLBOT_SCHEDULE_H
#define TRLBOT_SCHEDULE_H

#include <vector>
#include "Match.h"
#include "Player.h"
using namespace std;

class Match;
class Team;

class Schedule {
public:
    static vector<Match> schedule;
    static vector<Team> teams;
    static bool hasTeam(snowflake id);
    static bool hasMatch(int id);
    static int getTeam(snowflake id);
    static int getMatch(int id);
};


#endif //TRLBOT_SCHEDULE_H
