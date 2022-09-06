//
// Created by ankit on 8/24/22.
//

#ifndef TRLBOT_RECORDBOOK_H
#define TRLBOT_RECORDBOOK_H

#include <vector>
#include <dpp/dpp.h>

#include "Match.h"
#include "Player.h"
#include "Team.h"

using namespace std;

class Match;
class Team;
class Player;

class RecordBook {
public:
    static map<unsigned long long, Match> schedule;
    static map<unsigned long long, Team> teams;
    static map<unsigned long long, Player> players;
};


#endif //TRLBOT_RECORDBOOK_H
