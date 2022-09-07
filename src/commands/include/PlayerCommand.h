//
// Created by ankit on 8/28/22.
//

#ifndef TRLBOT_PLAYERCOMMAND_H
#define TRLBOT_PLAYERCOMMAND_H

#include <dpp/dpp.h>

#include "../../embeds/include/MatchEmbeds.h"
#include "../../embeds/include/TeamEmbeds.h"
#include "../../embeds/include/PlayerEmbeds.h"
#include "../../embeds/include/ScheduleEmbeds.h"

#include "../../include/Utilities.h"

class PlayerCommand {
public:
    static dpp::slashcommand cmd(dpp::snowflake botID);
    static dpp::message msg(const dpp::slashcommand_t &event, dpp::cluster& bot);
};


#endif //TRLBOT_PLAYERCOMMAND_H
