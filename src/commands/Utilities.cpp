//
// Created by ankit on 9/4/22.
//

#include "include/Utilities.h"

bool Utilities::checkPerms(interaction i, cluster &bot) {
    auto roles = i.member.roles;
    for (auto snowflake : roles){
        if (dpp::find_role(snowflake)->name == "League Staff")
            return true;
    }
    return false;
}
