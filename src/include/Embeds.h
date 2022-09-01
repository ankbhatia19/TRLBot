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
    /* <Match Embeds> */

    static embed matchReplayProcessing(int matchID);
    static embed matchReplayProcessingComplete(int matchID);
    static embed matchCompleteEmbed(int matchID);

    // On Error
    static embed matchNotFound(int matchID);
    static embed matchAlreadyPlayed(int matchID);

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
    static embed teamAddedPlayersEmbed(user player1, user player2, user player3, role team);
    static embed teamRemovedPlayerEmbed(user player, role team);
    static embed teamViewAllEmbed(vector<Team> teams);
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

    /* </Player Embeds> */

    /* <Miscellaneous Embeds> */

    static embed errorEmbed(string msg);
    static embed testEmbed();

    /* </Miscellaneous Embeds> */

private:
    static embed embedTemplate();
};


#endif //TRLBOT_EMBEDS_H
