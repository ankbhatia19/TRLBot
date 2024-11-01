//
// Created by ankit on 8/28/22.
//

#ifndef TRLBOT_MATCHCOMMAND_H
#define TRLBOT_MATCHCOMMAND_H

#include <dpp/dpp.h>
#include <httplib.h>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <filesystem>

#include "MatchEmbeds.h"
#include "TeamEmbeds.h"
#include "PlayerEmbeds.h"
#include "ScheduleEmbeds.h"

#include "Game.h"

#include "Utilities.h"
#include "BallchasingClient.h"

class MatchCommand {
public:
    static dpp::slashcommand cmd(dpp::snowflake botID);
    static dpp::message msg(const dpp::slashcommand_t& response, dpp::cluster& bot);
};


#endif //TRLBOT_MATCHCOMMAND_H
