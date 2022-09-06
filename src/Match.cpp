//
// Created by ankit on 8/24/22.
//

#include "include/Match.h"
#include <ctime>
#include <cstdlib>

vector<int> Match::allIDs;

int generateID(){
    time_t lt = time(NULL);
    srand(lt);
    return (rand() % 90000) + 10000; // always return a 5 digit random id
}

Match::Match(unsigned long long homeID, unsigned long long awayID){

    while (id == -1 || (find(allIDs.begin(), allIDs.end(), id) != allIDs.end())) {
        id = generateID();
    }
    allIDs.push_back(id);

    Match::homeID = homeID;
    Match::awayID = awayID;
    Match::matchStatus = status::UNPLAYED;
    Match::matchWinner = affiliation::NONE;
}

Match::Match() {
    id = -1;
    Match::homeID = 0;
    Match::awayID = 0;
    Match::matchStatus = status::UNPLAYED;
    Match::matchWinner = affiliation::NONE;
}

void Match::determineWinner() {

    int homeWins = 0, awayWins = 0;

    for (const auto& [key, _] : matchScores){
        int homeGoals = 0, awayGoals = 0;
        for (auto score : matchScores[key]){
            homeGoals += score.homeGoals;
            awayGoals += score.awayGoals;
        }
        (homeGoals > awayGoals) ? homeWins++ : awayWins++;
        if (homeGoals > awayGoals){
            homeWins++;
            RecordBook::teams[homeID].differential++;
            RecordBook::teams[awayID].differential--;
        }
        else {
            awayWins++;
            RecordBook::teams[homeID].differential--;
            RecordBook::teams[awayID].differential++;
        }
    }

    if (homeWins > awayWins){
        matchWinner = affiliation::HOME;

        RecordBook::teams[homeID].wins++;
        RecordBook::teams[awayID].losses++;
    }
    else{
        matchWinner = affiliation::AWAY;

        RecordBook::teams[homeID].losses++;
        RecordBook::teams[awayID].wins++;
    }
    matchStatus = status::PLAYED;
}