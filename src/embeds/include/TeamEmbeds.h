//
// Created by ankit on 9/7/22.
//

#ifndef TRLBOT_TEAMEMBEDS_H
#define TRLBOT_TEAMEMBEDS_H

#include "UtilityEmbeds.h"

class TeamEmbeds {

public:
    static dpp::embed teamHelpEmbed();

    // On Error
    static dpp::embed teamUnregisteredEmbed(dpp::role home, dpp::role away);
    static dpp::embed teamUnregisteredEmbed(dpp::role team);
    static dpp::embed teamPlayerAlreadyRegisteredEmbed(dpp::user player, dpp::role team);
    static dpp::embed teamPlayerUnregisteredEmbed(dpp::user player, dpp::role team);

    // On Success
    static dpp::embed teamRegisteredEmbed(dpp::role team);
    static dpp::embed teamDelistedEmbed(dpp::role team);
    static dpp::embed teamAddedPlayersEmbed(vector<dpp::user> players, dpp::role team);
    static dpp::embed teamRemovedPlayerEmbed(dpp::user player, dpp::role team);
    static dpp::embed teamViewAllEmbed(map<unsigned long long, Team> teams);
    static dpp::embed teamViewRoleEmbed(dpp::role team);
};


#endif //TRLBOT_TEAMEMBEDS_H
