//
// Created by ankit on 9/7/22.
//

#ifndef TRLBOT_UTILITYEMBEDS_H
#define TRLBOT_UTILITYEMBEDS_H

#include <dpp/dpp.h>
#include <string>

#include "Match.h"
#include "Player.h"
#include "Team.h"
#include "Utilities.h"

using namespace std;

class UtilityEmbeds {
public:
    static dpp::embed pong(string uptime);

    static dpp::embed help();

    static dpp::embed error_missing_perms(dpp::interaction interaction);

    static dpp::embed loading();
    static dpp::embed error(string msg);
    static dpp::embed test();

    static dpp::embed base();
};


#endif //TRLBOT_UTILITYEMBEDS_H
