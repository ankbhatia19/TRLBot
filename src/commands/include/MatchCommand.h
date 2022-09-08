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

#include "../../embeds/include/MatchEmbeds.h"
#include "../../embeds/include/TeamEmbeds.h"
#include "../../embeds/include/PlayerEmbeds.h"
#include "../../embeds/include/ScheduleEmbeds.h"

#include "../../include/RecordBook.h"
#include "../../include/Utilities.h"
#include "../../include/BallchasingClient.h"

class MatchCommand {
public:
    static dpp::slashcommand cmd(dpp::snowflake botID);
    static dpp::message msg(const dpp::slashcommand_t& response, dpp::cluster& bot);

private:
    struct PlayerRecord{
        string color;
        int index;
        dpp::snowflake playerID;
        enum Match::affiliation team;
    };
};


#endif //TRLBOT_MATCHCOMMAND_H
