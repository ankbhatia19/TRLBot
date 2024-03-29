//
// Created by ankit on 8/24/22.
//

#ifndef TRLBOT_MATCH_H
#define TRLBOT_MATCH_H

#include <iostream>
#include <algorithm>
#include <vector>
#include <ctime>
#include <random>
#include <dpp/dpp.h>

#include "Team.h"
#include "RecordBook.h"

using namespace std;

class Match {
public:
    Match(unsigned long long homeID, unsigned long long awayID);
    Match(nlohmann::json json);
    Match();

    enum status {
        UNPLAYED,
        PLAYED
    };
    enum affiliation {
        HOME,
        AWAY,
        NONE
    };
    struct score{
        int homeGoals;
        int awayGoals;
    };

    unsigned long long homeID, awayID;
    int id = -1;
    std::tm matchTime;
    status matchStatus;
    affiliation matchWinner;

    string ballchasingID;

    std::map<int, vector<score>> matchScores;
    score seriesScore;

    void determineWinner();

    nlohmann::json to_json();

    bool operator<(const Match& rhs) const;

private:
    static vector<int> allIDs;
};

#endif //TRLBOT_MATCH_H