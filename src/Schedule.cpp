//
// Created by ankit on 8/24/22.
//

#include "include/Schedule.h"

vector<Match> Schedule::schedule;
vector<Team> Schedule::teams;

bool Schedule::hasTeam(snowflake id) {
    return getTeam(id) >= 0;
}

bool Schedule::hasMatch(int id) {
    return getMatch(id) >= 0;
}

int Schedule::getTeam(snowflake id) {
    for (int i = 0; i < teams.size(); i++){
        if (id == teams[i].id){
            return i;
        }
    }
    return -1;
}

int Schedule::getMatch(int id) {
    for (int i = 0; i < schedule.size(); i++){
        if (id == schedule[i].id){
            return i;
        }
    }
    return -1;
}
