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
    return (rand() % 100000);
}

Match::Match(Team* home, Team* away){

    while (id == -1 || (find(allIDs.begin(), allIDs.end(), id) != allIDs.end())) {
        id = generateID();
    }
    allIDs.push_back(id);

    Match::home = home;
    Match::away = away;
}