//
// Created by ankit on 8/24/22.
//

#ifndef TRLBOT_PLAYER_H
#define TRLBOT_PLAYER_H

#include <vector>
#include <string>
#include "Match.h"
using namespace std;

class Player {

public:
    Player();
    vector<string> aliases;

    string token;
};


#endif //TRLBOT_PLAYER_H