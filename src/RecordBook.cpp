//
// Created by ankit on 8/24/22.
//

#include "include/RecordBook.h"

vector<Match> RecordBook::schedule;
vector<Team> RecordBook::teams;
vector<Player> RecordBook::players;

bool RecordBook::hasTeam(snowflake id) {
    return getTeam(id) >= 0;
}

bool RecordBook::hasMatch(int id) {
    return getMatch(id) >= 0;
}

bool RecordBook::hasPlayer(snowflake id) {
    return getPlayer(id) >= 0;
}

int RecordBook::getTeam(snowflake id) {
    for (int i = 0; i < teams.size(); i++){
        if (id == teams[i].team.id){
            return i;
        }
    }
    return -1;
}

int RecordBook::getMatch(int id) {
    for (int i = 0; i < schedule.size(); i++){
        if (id == schedule[i].id){
            return i;
        }
    }
    return -1;
}

int RecordBook::getPlayer(snowflake id) {
    for (int i = 0; i < players.size(); i++){
        if (id == players[i].profile.id){
            return i;
        }
    }
    return -1;
}