//
// Created by ankit on 8/24/22.
//

#include "include/RecordBook.h"

map<unsigned long long, Match> RecordBook::schedule;
map<unsigned long long, Team> RecordBook::teams;
map<unsigned long long, Player> RecordBook::players;

string RecordBook::getBotToken() {
    std::ifstream f("config.json");
    json data = json::parse(f);
    string token = data["bot_token"];
    f.close();

    return token;
}

string RecordBook::getBallchasingToken() {
    std::ifstream f("config.json");
    json data = json::parse(f);
    string token = data["ballchasing_token"];
    f.close();

    return token;
}