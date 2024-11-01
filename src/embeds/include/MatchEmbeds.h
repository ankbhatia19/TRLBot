//
// Created by ankit on 9/7/22.
//

#ifndef TRLBOT_MATCHEMBEDS_H
#define TRLBOT_MATCHEMBEDS_H

#include "UtilityEmbeds.h"
#include "Game.h"

class MatchEmbeds {

public:

    static dpp::embed help();

    // On success
    static dpp::embed replay_processing(int matchID);
    static dpp::embed complete(int matchID);
    static dpp::embed removed(int matchID);

    // On Error
    static dpp::embed error_not_found(int matchID);
    static dpp::embed error_duplicate_submission(int matchID);
    static dpp::embed error_missing_username(vector<string> unregistered);
    static dpp::embed error_missing_team(vector<int64_t> teamless, int matchID);
};


#endif //TRLBOT_MATCHEMBEDS_H
