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

Match::Match(Team* home, Team* away){

    while (id == -1 || (find(allIDs.begin(), allIDs.end(), id) != allIDs.end())) {
        id = generateID();
    }
    allIDs.push_back(id);

    Match::home = home;
    Match::away = away;
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
            home->differential++;
            away->differential--;
        }
        else {
            awayWins++;
            home->differential--;
            away->differential++;
        }
    }

    if (homeWins > awayWins){
        matchWinner = affiliation::HOME;
        Match::home->wins++;
        Match::away->losses++;
    }
    else{
        matchWinner = affiliation::AWAY;
        Match::away->wins++;
        Match::home->losses++;
    }
    matchStatus = status::PLAYED;
}