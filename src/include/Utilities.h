//
// Created by ankit on 9/4/22.
//

#ifndef TRLBOT_UTILITIES_H
#define TRLBOT_UTILITIES_H

#include "dpp/dpp.h"
#include <string>
#include <fstream>

using namespace std;

class Utilities {
public:
    static bool checkPerms(dpp::interaction i);

    static string getBotToken();
    static string getBallchasingToken();
    static string getBallchasingGroup();

    static void cmd_init(dpp::slashcommand_map cmds);

    static map<string, unsigned long long> cmd_map;
};


#endif //TRLBOT_UTILITIES_H
