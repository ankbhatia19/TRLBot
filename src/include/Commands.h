//
// Created by ankit on 8/24/22.
//

#ifndef TRLBOT_COMMANDS_H
#define TRLBOT_COMMANDS_H
#include <dpp/dpp.h>
using namespace dpp;

class Commands {
public:
    static slashcommand pingCommand(snowflake botID);
    static slashcommand matchCommand(snowflake botID);
    static slashcommand teamCommand(snowflake botID);
    static slashcommand scheduleCommand(snowflake botID);
    static slashcommand playerCommand(snowflake botID);
};


#endif //TRLBOT_COMMANDS_H
