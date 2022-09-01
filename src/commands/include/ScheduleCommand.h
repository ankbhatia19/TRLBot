//
// Created by ankit on 8/28/22.
//

#ifndef TRLBOT_SCHEDULECOMMAND_H
#define TRLBOT_SCHEDULECOMMAND_H

#include <dpp/dpp.h>
#include "../../include/Embeds.h"
using namespace dpp;

class ScheduleCommand {
public:
    static slashcommand cmd(snowflake botID);
    static message msg(const slashcommand_t &event, cluster& bot);
};


#endif //TRLBOT_SCHEDULECOMMAND_H
