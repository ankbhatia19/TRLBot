//
// Created by ankit on 8/28/22.
//

#ifndef TRLBOT_MATCHCOMMAND_H
#define TRLBOT_MATCHCOMMAND_H

#include <dpp/dpp.h>
#include "../../include/Embeds.h"
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <filesystem>

using namespace dpp;

class MatchCommand {
public:
    static slashcommand cmd(snowflake botID);
    static message msg(const slashcommand_t& response, cluster& bot);
};


#endif //TRLBOT_MATCHCOMMAND_H
