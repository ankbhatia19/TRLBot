//
// Created by ankit on 8/24/22.
//

#ifndef TRLBOT_EMBEDS_H
#define TRLBOT_EMBEDS_H

#include <dpp/dpp.h>
#include <string>
#include "Player.h"
#include "Match.h"
#include "RecordBook.h"
using namespace std;

class Embeds {
public:

    static dpp::embed pingEmbed(string uptime);

    /* <Match Embeds> */

    static dpp::embed matchReplayProcessing(int matchID);
    static dpp::embed matchCompleteEmbed(int matchID);

    // On Error
    static dpp::embed matchNotFound(int matchID);
    static dpp::embed matchAlreadyPlayed(int matchID);
    static dpp::embed playersNotRegistered(vector<string> unregistered);

    /* </Match Embeds> */

    /* <Team Embeds> */

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

    /* </Team Embeds> */

    /* <RecordBook Embeds> */

    // On Success
    static dpp::embed scheduleViewAllMatches();
    static dpp::embed scheduleViewMatch(int id);

    // On Error
    static dpp::embed scheduleMatchDoesNotExist(int id);

    /* </RecordBook Embeds> */

    /* <Player Embeds> */

    // On success
    static dpp::embed playerView(dpp::user profile);
    static dpp::embed playerAddedUsername(dpp::user user, string username);

    // On error
    static dpp::embed playerNotFound(dpp::user profile);
    static dpp::embed playerUsernameExists(dpp::user profile, string name);

    /* </Player Embeds> */

    /* <Miscellaneous Embeds> */

    static dpp::embed insufficientPermsEmbed(dpp::interaction interaction);
    static dpp::embed loadingEmbed();
    static dpp::embed errorEmbed(string msg);
    static dpp::embed testEmbed();

    /* </Miscellaneous Embeds> */

private:
    static dpp::embed embedTemplate();
};


#endif //TRLBOT_EMBEDS_H
