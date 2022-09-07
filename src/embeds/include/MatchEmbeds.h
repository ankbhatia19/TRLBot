//
// Created by ankit on 9/7/22.
//

#ifndef TRLBOT_MATCHEMBEDS_H
#define TRLBOT_MATCHEMBEDS_H

#include "UtilityEmbeds.h"

class MatchEmbeds {

public:

    // On success
    static dpp::embed matchReplayProcessing(int matchID);
    static dpp::embed matchCompleteEmbed(int matchID);

    // On Error
    static dpp::embed matchNotFound(int matchID);
    static dpp::embed matchAlreadyPlayed(int matchID);
    static dpp::embed matchPlayersNotRegistered(vector<string> unregistered);
};


#endif //TRLBOT_MATCHEMBEDS_H
