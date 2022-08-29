//
// Created by ankit on 8/24/22.
//

#ifndef TRLBOT_RECORDBOOK_H
#define TRLBOT_RECORDBOOK_H

#include <vector>
#include <fstream>
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
    static vector<Match> schedule;
    static vector<Team> teams;
    static vector<Player> players;

    static bool hasTeam(snowflake id);
    static bool hasMatch(int id);
    static int getTeam(snowflake id);
    static int getMatch(int id);
    static bool hasPlayer(snowflake id);
    static int getPlayer(snowflake id);

    static string getBotToken();
    static string getBallchasingToken();
    static string getBallchasingURL();
};


#endif //TRLBOT_RECORDBOOK_H
