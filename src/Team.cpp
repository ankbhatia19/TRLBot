//
// Created by ankit on 8/24/22.
//

#include "include/Team.h"

Team::Team(unsigned long long id) {
    Team::id = id;
    wins = 0, losses = 0;
    differential = 0;
}

Team::Team() {
    Team::id = 0;
    wins = 0, losses = 0;
    differential = 0;
}
