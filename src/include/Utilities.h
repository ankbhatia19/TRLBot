//
// Created by ankit on 9/4/22.
//

#ifndef TRLBOT_UTILITIES_H
#define TRLBOT_UTILITIES_H

#include "dpp/dpp.h"
#include <nlohmann/json.hpp>
#include "SQLiteCpp/SQLiteCpp.h"
#include <string>
#include <fstream>
#include <random>

using namespace std;
using json = nlohmann::json;

class Utilities {
public:
    static bool checkPerms(dpp::interaction i);

    static string getBotToken();
    static string getBallchasingToken();
    static string getBallchasingGroup();
    static unsigned long long getScoreReportChannel();

    static string getQuote();

    static void cmd_init(dpp::slashcommand_map cmds);

    static map<string, unsigned long long> cmd_map;

    static void bind_val(SQLite::Statement& stmt, int index, const json& value);

    static SQLite::Database get_db();

    enum ErrorCode{
        kError = 0,
        kSuccess = 1,
        kPlayerNotRegistered = 2,
        kUsernameExists = 3,

    };
};


#endif //TRLBOT_UTILITIES_H
