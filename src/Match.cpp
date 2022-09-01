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
    Match::matchWinner = winner::NONE;
}

void Match::determineWinner() {

    int homeWins = 0, awayWins = 0;

    for (auto game : matchScores){
        if (game.homeGoals > game.awayGoals){
            Match::home->differential++;
            Match::away->differential--;
            homeWins++;
        }
        else {
            Match::away->differential++;
            Match::home->differential--;
            awayWins++;
        }
    }
    if (homeWins > awayWins){
        matchWinner = winner::HOME;
        Match::home->wins++;
        Match::away->losses++;
    }
    else{
        matchWinner = winner::AWAY;
        Match::away->wins++;
        Match::home->losses++;
    }
    matchStatus = status::PLAYED;
}