//
// Created by ankit on 8/24/22.
//

#ifndef TRLBOT_MATCH_H
#define TRLBOT_MATCH_H

#include <iostream>
#include <algorithm>
#include <vector>
#include "Team.h"
using namespace std;

class Match;
class Team;

class Match {
public:
    Match(Team* home, Team* away);
    Team* home;
    Team* away;
    enum status {
        UNPLAYED,
        RESCHEDULED,
        PLAYED
    };
    enum affiliation {
        HOME,
        AWAY,
        NONE
    };
    struct score{
        int homeGoals;
        int awayGoals;
    };

    int id = -1;
    struct tm* matchTime;
    enum status matchStatus;
    enum affiliation matchWinner;
    //vector<score> matchScores;
    std::map<int, vector<score>> matchScores;

    void determineWinner();

private:
    static vector<int> allIDs;
};

#endif //TRLBOT_MATCH_H