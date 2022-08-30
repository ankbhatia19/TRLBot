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


#include "../../include/Embeds.h"
#include "../../include/RecordBook.h"

using namespace dpp;

class MatchCommand {
public:
    static slashcommand cmd(snowflake botID);
    static message msg(const slashcommand_t& response, cluster& bot);

private:
    static string token;
    static string url;
    struct PlayerRecord{
        string team;
        int index;
    };
};


#endif //TRLBOT_MATCHCOMMAND_H
