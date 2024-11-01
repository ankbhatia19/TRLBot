//
// Created by ankit on 9/7/22.
//

#ifndef TRLBOT_MATCHEMBEDS_H
#define TRLBOT_MATCHEMBEDS_H

#include "UtilityEmbeds.h"

class MatchEmbeds {

public:

    static dpp::embed matchHelpEmbed();

    // On success
    static dpp::embed matchReplayProcessing(int matchID);
    static dpp::embed matchCompleteEmbed(int matchID);
    static dpp::embed matchRemoved(int matchID);

    // On Error
    static dpp::embed matchNotFound(int matchID);
    static dpp::embed matchAlreadyPlayed(int matchID);
    static dpp::embed matchPlayersNotRegistered(vector<string> unregistered);
    static dpp::embed matchPlayersNotOnTeam(vector<int64_t> teamless, int matchID);
};


#endif //TRLBOT_MATCHEMBEDS_H
