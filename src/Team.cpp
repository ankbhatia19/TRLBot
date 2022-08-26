//
// Created by ankit on 8/24/22.
//

#include "include/Team.h"

Team::Team(snowflake id) {
    Team::id = id;
    wins, losses, draws = 0;
}

Team::~Team() {

}
