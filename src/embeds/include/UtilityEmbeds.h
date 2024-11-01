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
    static dpp::embed pingEmbed(string uptime);

    static dpp::embed helpEmbed();

    static dpp::embed insufficientPermsEmbed(dpp::interaction interaction);

    static dpp::embed loadingEmbed();
    static dpp::embed errorEmbed(string msg);
    static dpp::embed testEmbed();

    static dpp::embed embedTemplate();
};


#endif //TRLBOT_UTILITYEMBEDS_H
