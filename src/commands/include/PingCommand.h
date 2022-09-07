//
// Created by ankit on 8/28/22.
//

#ifndef TRLBOT_PINGCOMMAND_H
#define TRLBOT_PINGCOMMAND_H

#include <dpp/dpp.h>

#include "../../embeds/include/UtilityEmbeds.h"

class PingCommand {
public:
    static dpp::slashcommand cmd(dpp::snowflake botID);
    static dpp::message msg(const dpp::slashcommand_t &event, dpp::cluster& bot);
};


#endif //TRLBOT_PINGCOMMAND_H
