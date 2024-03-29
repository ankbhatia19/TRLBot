//
// Created by ankit on 9/7/22.
//

#ifndef TRLBOT_SCHEDULEEMBEDS_H
#define TRLBOT_SCHEDULEEMBEDS_H

#include "UtilityEmbeds.h"
#include "MatchEmbeds.h"

class ScheduleEmbeds {

public:
    static dpp::embed scheduleHelpEmbed();

    // On Success
    static dpp::embed scheduleViewAllMatches();
    static dpp::embed scheduleViewMatch(int id);
    static dpp::embed scheduleViewTeamMatches(unsigned long long role);

    // On Error
    static dpp::embed scheduleMatchDoesNotExist(int id);
    static dpp::embed scheduleInvalidTime();
};


#endif //TRLBOT_SCHEDULEEMBEDS_H
