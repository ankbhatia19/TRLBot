//
// Created by ankit on 8/25/22.
//

#ifndef TRLBOT_COMMANDPROCESSOR_H
#define TRLBOT_COMMANDPROCESSOR_H
#include <dpp/dpp.h>
#include <iostream>
#include <string>

#include "Embeds.h"
#include "Schedule.h"
#include "Match.h"
#include "Player.h"
#include "Team.h"

using namespace std;
using namespace dpp;

class CommandProcessor {
public:
    static embed pingCommand(interaction interaction);
    static embed matchCommand(interaction interaction);
    static embed teamCommand(interaction interaction);
    static embed scheduleCommand(interaction interaction);
    static embed playerCommand(interaction interaction);
};


#endif //TRLBOT_COMMANDPROCESSOR_H
