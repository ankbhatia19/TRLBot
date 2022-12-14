//
// Created by ankit on 9/7/22.
//

#ifndef TRLBOT_PLAYEREMBEDS_H
#define TRLBOT_PLAYEREMBEDS_H

#include "UtilityEmbeds.h"

class PlayerEmbeds {

public:
    static dpp::embed playerHelpEmbed();

    // On success
    static dpp::embed playerView(dpp::user profile);
    static dpp::embed playerAddedUsername(dpp::user user, string username);
    static dpp::embed playerRemovedUsername(dpp::user user, string username);

    // On error
    static dpp::embed playerNotFound(dpp::user profile);
    static dpp::embed playerUsernameExists(dpp::user profile, string name);
    static dpp::embed playerUsernameDoesNotExist(dpp::user profile, string name);
};


#endif //TRLBOT_PLAYEREMBEDS_H
