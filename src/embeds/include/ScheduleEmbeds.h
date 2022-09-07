//
// Created by ankit on 9/7/22.
//

#ifndef TRLBOT_SCHEDULEEMBEDS_H
#define TRLBOT_SCHEDULEEMBEDS_H

#include "UtilityEmbeds.h"
#include "MatchEmbeds.h"

class ScheduleEmbeds {

public:
    // On Success
    static dpp::embed scheduleViewAllMatches();
    static dpp::embed scheduleViewMatch(int id);

    // On Error
    static dpp::embed scheduleMatchDoesNotExist(int id);
};


#endif //TRLBOT_SCHEDULEEMBEDS_H
