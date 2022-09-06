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
using namespace dpp;

class Embeds {
public:

    static embed pingEmbed(string uptime);

    /* <Match Embeds> */

    static embed matchReplayProcessing(int matchID);
    static embed matchCompleteEmbed(int matchID);

    // On Error
    static embed matchNotFound(int matchID);
    static embed matchAlreadyPlayed(int matchID);
    static embed playersNotRegistered(vector<string> unregistered);

    /* </Match Embeds> */

    /* <Team Embeds> */

    // On Error
    static embed teamUnregisteredEmbed(role home, role away);
    static embed teamUnregisteredEmbed(role team);
    static embed teamPlayerAlreadyRegisteredEmbed(user player, role team);
    static embed teamPlayerUnregisteredEmbed(user player, role team);

    // On Success
    static embed teamRegisteredEmbed(role team);
    static embed teamDelistedEmbed(role team);
    static embed teamAddedPlayersEmbed(vector<user> players, role team);
    static embed teamRemovedPlayerEmbed(user player, role team);
    static embed teamViewAllEmbed(map<unsigned long long, Team> teams);
    static embed teamViewRoleEmbed(role team);

    /* </Team Embeds> */

    /* <RecordBook Embeds> */

    // On Success
    static embed scheduleViewAllMatches();
    static embed scheduleViewMatch(int id);

    // On Error
    static embed scheduleMatchDoesNotExist(int id);

    /* </RecordBook Embeds> */

    /* <Player Embeds> */

    // On success
    static embed playerView(user profile);
    static embed playerAddedUsername(user user, string username);

    // On error
    static embed playerNotFound(user profile);
    static embed playerUsernameExists(user profile, string name);

    /* </Player Embeds> */

    /* <Miscellaneous Embeds> */

    static embed insufficientPermsEmbed(interaction interaction);
    static embed loadingEmbed();
    static embed errorEmbed(string msg);
    static embed testEmbed();

    /* </Miscellaneous Embeds> */

private:
    static embed embedTemplate();
};


#endif //TRLBOT_EMBEDS_H
