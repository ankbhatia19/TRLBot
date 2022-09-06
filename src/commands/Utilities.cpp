//
// Created by ankit on 9/4/22.
//

#include "include/Utilities.h"

bool Utilities::checkPerms(interaction i) {
    auto roles = i.member.roles;
    for (auto snowflake : roles){
        if (dpp::find_role(snowflake)->name == "League Staff")
            return true;
    }
    return false;
}

string Utilities::getBotToken() {
    std::ifstream f("config.json");
    json data = json::parse(f);
    string token = data["bot_token"];
    f.close();

    return token;
}

string Utilities::getBallchasingToken() {
    std::ifstream f("config.json");
    json data = json::parse(f);
    string token = data["ballchasing_token"];
    f.close();

    return token;
}