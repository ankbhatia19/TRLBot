//
// Created by ankit on 8/24/22.
//

#ifndef TRLBOT_EMBEDS_H
#define TRLBOT_EMBEDS_H

#include <dpp/dpp.h>
#include <string>
#include "Player.h"
#include "Schedule.h"
using namespace std;
using namespace dpp;

class Embeds {
public:
    static embed matchEmbed(int matchID, string home, string away, vector<Player> homePlayers, vector<Player> awayPlayers);
    static embed errorEmbed(string msg);
    static embed teamUnregisteredEmbed(role home, role away);
    static embed teamUnregisteredEmbed(role team);
    static embed teamRegisteredEmbed(role team);
    static embed teamDelistedEmbed(role team);
    static embed teamAddedPlayerEmbed(user player, role team);
    static embed testEmbed();
};


#endif //TRLBOT_EMBEDS_H
