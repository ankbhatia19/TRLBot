//
// Created by ankit on 8/24/22.
//

#ifndef TRLBOT_EMBEDS_H
#define TRLBOT_EMBEDS_H

#include <dpp/dpp.h>
#include <string>
#include "Player.h"
#include "Match.h"
#include "Schedule.h"
using namespace std;
using namespace dpp;

class Embeds {
public:
    /* <Match Embeds> */

    static embed matchCreatedEmbed(Match matchID, role home, role away);

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
    static embed teamAddedPlayerEmbed(user player, role team);
    static embed teamRemovedPlayerEmbed(user player, role team);
    static embed teamViewAllEmbed(vector<Team> teams);
    static embed teamViewRoleEmbed(role team);

    /* </Team Embeds> */

    /* <Miscellaneous Embeds> */

    static embed errorEmbed(string msg);
    static embed testEmbed();

    /* </Miscellaneous Embeds> */

private:
    static embed embedTemplate();
};


#endif //TRLBOT_EMBEDS_H
