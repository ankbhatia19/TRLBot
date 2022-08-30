//
// Created by ankit on 8/24/22.
//

#include "include/Player.h"

Player::Player(user profile) {
    Player::profile = profile;
}

int Player::calculateMVPR() {
    return 0;
}

int Player::getStatistic(Player::statistic stat) {
    int total;
    switch (stat){
        case (GOALS):
            for (auto statistic : stats)
                total += statistic.goals;
            return total;
        case (ASSISTS):
            for (auto statistic : stats)
                total += statistic.assists;
            return total;
        case (SAVES):
            for (auto statistic : stats)
                total += statistic.saves;
            return total;;
        case (SHOTS):
            for (auto statistic : stats)
                total += statistic.shots;
            return total;;
        case (AVG_MVPR):
            for (auto statistic : stats)
                total += (statistic.shots / 3 + statistic.saves + statistic.goals) * 1000;
            return total / stats.size();
    }
    return 0;
}
