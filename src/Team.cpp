//
// Created by ankit on 8/24/22.
//

#include "include/Team.h"

Team::Team(role team) {
    Team::team = team;
    wins = 0, losses = 0;
    differential = 0;
}

Team::~Team() {

}

int Team::getMatch(int id) {
    for (int i = 0; i < matches.size(); i++){
        if (id == matches[i].id){
            return i;
        }
    }
    return -1;
}

bool Team::hasMatch(int id) {
    return getMatch(id) >= 0;
}

int Team::getPlayer(snowflake id) {
    for (int i = 0; i < players.size(); i++){
        if (id == players[i].profile.id){
            return i;
        }
    }
    return -1;
}

bool Team::hasPlayer(snowflake id) {
    return getPlayer(id) >= 0;
}
