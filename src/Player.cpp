//
// Created by ankit on 8/24/22.
//

#include "include/Player.h"

Player::Player(unsigned long long id) {
    Player::id = id;
    Player::teamID = 0;
}

Player::Player() {
    Player::id = 0;
    Player::teamID = 0;
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
            return round(total / stats.size());
    }
    return 0;
}

bool Player::containsAlias(string alias) {
    for (string username : aliases){
        if (alias == username)
            return true;
    }
    return false;
}

