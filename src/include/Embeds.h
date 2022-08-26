//
// Created by ankit on 8/24/22.
//

#ifndef TRLBOT_EMBEDS_H
#define TRLBOT_EMBEDS_H

#include <dpp/dpp.h>
#include <string>
using namespace std;
using namespace dpp;

class Embeds {
public:
    static embed matchEmbed(int matchID, string home, string away, members_container homePlayers, members_container awayPlayers);
    static embed testEmbed();
};


#endif //TRLBOT_EMBEDS_H
