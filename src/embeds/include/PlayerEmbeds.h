//
// Created by ankit on 9/7/22.
//

#ifndef TRLBOT_PLAYEREMBEDS_H
#define TRLBOT_PLAYEREMBEDS_H

#include "UtilityEmbeds.h"

class PlayerEmbeds {

public:
    static dpp::embed help();

    // On success
    static dpp::embed view(dpp::user profile);
    static dpp::embed added_username(dpp::user user, string username);
    static dpp::embed removed_username(dpp::user user, string username);

    // On error
    static dpp::embed error_not_found(dpp::user profile);
    static dpp::embed error_duplicate_username(dpp::user profile, string name);
    static dpp::embed error_missing_username(dpp::user profile, string name);
};


#endif //TRLBOT_PLAYEREMBEDS_H
