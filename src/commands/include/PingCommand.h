//
// Created by ankit on 8/28/22.
//

#ifndef TRLBOT_PINGCOMMAND_H
#define TRLBOT_PINGCOMMAND_H

#include <dpp/dpp.h>
#include "../../include/Embeds.h"
using namespace dpp;

class PingCommand {
public:
    static slashcommand cmd(snowflake botID);
    static message msg(const slashcommand_t& event);
};


#endif //TRLBOT_PINGCOMMAND_H
