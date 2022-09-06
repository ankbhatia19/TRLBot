//
// Created by ankit on 8/28/22.
//

#ifndef TRLBOT_SCHEDULECOMMAND_H
#define TRLBOT_SCHEDULECOMMAND_H

#include <dpp/dpp.h>
#include "../../include/Embeds.h"

class ScheduleCommand {
public:
    static dpp::slashcommand cmd(dpp::snowflake botID);
    static dpp::message msg(const dpp::slashcommand_t &event, dpp::cluster& bot);
};


#endif //TRLBOT_SCHEDULECOMMAND_H
