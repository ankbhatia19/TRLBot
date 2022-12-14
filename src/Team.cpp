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

nlohmann::json Team::to_json() {
    nlohmann::json json;
    json["id"] = id;
    json["stats"]["wins"] = wins;
    json["stats"]["losses"] = losses;
    json["stats"]["differential"] = differential;

    for (const auto& [key, _] : members){
        json["members"].emplace_back(key);
    }

    return json;
}

Team::Team(nlohmann::json json) {
    id = json["id"];
    wins = json["stats"]["wins"];
    losses = json["stats"]["losses"];
    differential = json["stats"]["differential"];

    for (const auto& member : json["members"]){
        members.insert({member, RecordBook::players[member]});
    }
}
