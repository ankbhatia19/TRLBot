//
// Created by ankit on 8/24/22.
//

#include "Player.h"

Player::Player(unsigned long long id) {
    Player::id = id;
    Player::teamID = 0;
}

Player::Player() {
    Player::id = 0;
    Player::teamID = 0;
}

Player::Player(nlohmann::json json) {
    id = json["id"];
    teamID = json["teamID"];
    for (auto stat : json["stats"]){
        stats.emplace_back(MatchStatistic{
                stat["matchID"],
                stat["shots"],
                stat["goals"],
                stat["saves"],
                stat["assists"]
        });
    }

    for (std::string alias : json["aliases"]){
        aliases.emplace_back(alias);
    }
}

int Player::getStatistic(Player::statistic stat) {
    double total = 0;

    switch (stat){
        case (GOALS):
            for (auto game : stats)
                total += game.goals;
            return (int)total;
        case (ASSISTS):
            for (auto game : stats)
                total += game.assists;
            return (int)total;
        case (SAVES):
            for (auto game : stats)
                total += game.saves;
            return (int)total;
        case (SHOTS):
            for (auto game : stats)
                total += game.shots;
            return (int)total;
        case (AVG_MVPR):
            // (Goals) + (Assists * 0.75) + (Saves * 0.6) + (Shots / 3) ) / Games Played
            for (auto game : stats)
                total += 250 + (250 * (game.goals + (game.assists * 0.75) + (game.saves * 0.6) + (game.shots / 3.0)));
            return (int)total;
    }
    return 0;
}

double Player::getStatisticAvg(Player::statistic stat) {
    return ((double)getStatistic(stat) / stats.size());
}

bool Player::containsAlias(string alias) {
    for (string username : aliases){
        if (alias == username)
            return true;
    }
    return false;
}

nlohmann::json Player::to_json() {
    nlohmann::json json;
    json["id"] = id;
    json["teamID"] = teamID;
    for (int i = 0; i < stats.size(); i++){
        json["stats"][i]["matchID"] = stats[i].matchID;
        json["stats"][i]["shots"] = stats[i].shots;
        json["stats"][i]["goals"] = stats[i].goals;
        json["stats"][i]["saves"] = stats[i].saves;
        json["stats"][i]["assists"] = stats[i].assists;
    }

    for (int i = 0; i < aliases.size(); i++){
        json["aliases"][i] = aliases[i];
    }

    return json;
}



