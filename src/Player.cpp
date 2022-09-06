//
// Created by ankit on 8/24/22.
//

#include "include/Player.h"

Player::Player(unsigned long long id) {
    Player::id = id;
}

Player::Player() {
    Player::id = 0;
}

int Player::getStatistic(Player::statistic stat) {
    int total = 0;

    switch (stat){
        case (GOALS):
            for (auto game : stats)
                total += game.goals;
            return total;
        case (ASSISTS):
            for (auto game : stats)
                total += game.assists;
            return total;
        case (SAVES):
            for (auto game : stats)
                total += game.saves;
            return total;;
        case (SHOTS):
            for (auto game : stats)
                total += game.shots;
            return total;;
        case (AVG_MVPR):
            // (Goals) + (Assists * 0.75) + (Saves * 0.6) + (Shots / 3) ) / Games Played
            for (auto game : stats)
                total += (game.goals + (game.assists * 0.75) + (game.saves * 0.6) + (game.shots / 3)) * 500;
            return total / stats.size();
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

