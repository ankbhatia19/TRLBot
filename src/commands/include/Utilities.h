//
// Created by ankit on 9/4/22.
//

#ifndef TRLBOT_UTILITIES_H
#define TRLBOT_UTILITIES_H

#include <dpp/dpp.h>
#include <string>
#include <fstream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;
using namespace std;

class Utilities {
public:
    static bool checkPerms(dpp::interaction i);

    static string getBotToken();
    static string getBallchasingToken();
};


#endif //TRLBOT_UTILITIES_H
