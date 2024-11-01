//
// Created by ankit on 9/7/22.
//

#ifndef TRLBOT_TEAMEMBEDS_H
#define TRLBOT_TEAMEMBEDS_H

#include "UtilityEmbeds.h"

class TeamEmbeds {

public:
    static dpp::embed help();

    // On Error
    static dpp::embed error_unregistered(dpp::role home, dpp::role away);
    static dpp::embed error_unregistered(dpp::role team);
    static dpp::embed error_duplicate_player(dpp::user player, dpp::role team);
    static dpp::embed error_player_not_on_team(dpp::user player, dpp::role team);

    // On Success
    static dpp::embed registered(dpp::role team);
    static dpp::embed delisted(dpp::role team);
    static dpp::embed added_players(vector<int64_t> users, int64_t team);
    static dpp::embed removed_player(dpp::user player, dpp::role team);
    static dpp::embed view(vector<int64_t> teams);
    static dpp::embed view(dpp::role team);
};


#endif //TRLBOT_TEAMEMBEDS_H
