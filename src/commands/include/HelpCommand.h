//
// Created by ankit on 9/9/22.
//

#ifndef TRLBOT_HELPCOMMAND_H
#define TRLBOT_HELPCOMMAND_H

#include <dpp/dpp.h>

#include "../../embeds/include/UtilityEmbeds.h"

class HelpCommand {
public:
    static dpp::slashcommand cmd(dpp::snowflake botID);
    static dpp::message msg(const dpp::slashcommand_t& response, dpp::cluster& bot);
};


#endif //TRLBOT_HELPCOMMAND_H
