//
// Created by ankit on 9/7/22.
//

#ifndef TRLBOT_SCHEDULEEMBEDS_H
#define TRLBOT_SCHEDULEEMBEDS_H

#include "UtilityEmbeds.h"
#include "MatchEmbeds.h"

class ScheduleEmbeds {

public:
    static dpp::embed help();

    // On Success
    static dpp::embed view();
    static dpp::embed view(int id);
    static dpp::embed view_team(unsigned long long role);

    // On Error
    static dpp::embed error_not_found(int id);
    static dpp::embed error_invalid_time();
};


#endif //TRLBOT_SCHEDULEEMBEDS_H
