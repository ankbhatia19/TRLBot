//
// Created by ankit on 8/24/22.
//

#include "Match.h"

vector<int> Match::allIDs;

int generateID(){
    // Get a random number
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_real_distribution<double> dist(10000, 100000);
    return dist(mt); // always return a 5 digit random id
}

Match::Match(unsigned long long homeID, unsigned long long awayID){

    while (id == -1 || (find(allIDs.begin(), allIDs.end(), id) != allIDs.end())) {
        id = generateID();
    }
    allIDs.push_back(id);

    Match::homeID = homeID;
    Match::awayID = awayID;
    Match::matchStatus = status::UNPLAYED;
    Match::matchWinner = affiliation::NONE;

    Match::ballchasingID = "";

    std::time_t temp = time(nullptr);
    matchTime = *(std::localtime(&temp));
}

Match::Match(nlohmann::json json) {
    id = json["id"];
    homeID = json["teams"]["home"];
    awayID = json["teams"]["away"];
    matchStatus = json["match"]["status"];
    matchWinner = json["match"]["winner"];
    seriesScore.homeGoals = json["match"]["score"]["home"];
    seriesScore.awayGoals = json["match"]["score"]["away"];
    ballchasingID = json["ballchasing_id"];

    // We use 1 indexing for the replays, so the 0th index is always null
    for (int replayNum = 1; replayNum < json["match"]["scores"].size(); replayNum++){
        matchScores.insert({replayNum, vector<Match::score>()});
        matchScores[replayNum].emplace_back(Match::score{
                json["match"]["scores"][replayNum]["home"],
                json["match"]["scores"][replayNum]["away"]
        });
    }
    allIDs.push_back(id);
    std::time_t temp = time(nullptr);
    matchTime = *(std::localtime(&temp));
}

Match::Match() {
    id = -1;
    Match::homeID = 0;
    Match::awayID = 0;
    Match::matchStatus = status::UNPLAYED;
    Match::matchWinner = affiliation::NONE;
    std::time_t temp = time(nullptr);
    matchTime = *(std::localtime(&temp));
}


void Match::determineWinner() {

    seriesScore.homeGoals = 0;
    seriesScore.awayGoals = 0;

    for (const auto& [key, _] : matchScores){
        int homeGoals = 0, awayGoals = 0;
        for (auto score : matchScores[key]){
            homeGoals += score.homeGoals;
            awayGoals += score.awayGoals;
        }

        if (homeGoals > awayGoals){
            seriesScore.homeGoals++;
            RecordBook::teams[homeID].differential++;
            RecordBook::teams[awayID].differential--;
        }
        else {
            seriesScore.awayGoals++;
            RecordBook::teams[homeID].differential--;
            RecordBook::teams[awayID].differential++;
        }
    }

    if (seriesScore.homeGoals > seriesScore.awayGoals){
        matchWinner = affiliation::HOME;

        RecordBook::teams[homeID].wins++;
        RecordBook::teams[awayID].losses++;
    }
    else{
        matchWinner = affiliation::AWAY;

        RecordBook::teams[homeID].losses++;
        RecordBook::teams[awayID].wins++;
    }
    matchStatus = status::PLAYED;
}

nlohmann::json Match::to_json() {
    nlohmann::json json;
    json["id"] = id;
    json["teams"]["home"] = homeID;
    json["teams"]["away"] = awayID;
    json["match"]["status"] = (int)matchStatus;
    json["match"]["winner"] = (int)matchWinner;
    json["match"]["score"]["home"] = seriesScore.homeGoals;
    json["match"]["score"]["away"] = seriesScore.awayGoals;
    json["ballchasing_id"] = ballchasingID;

    for (const auto& [key, _] : matchScores) {
        int homeGoals = 0, awayGoals = 0;
        for (auto score: matchScores[key]) {
            homeGoals += score.homeGoals;
            awayGoals += score.awayGoals;
        }
        std::cout << "Added score " << homeGoals << " / " << awayGoals << std::endl;
        json["match"]["scores"][key]["home"] = homeGoals;
        json["match"]["scores"][key]["away"] = awayGoals;
    }

    return json;
}

bool Match::operator<(const Match &rhs) const {

    // please.. don't ask
    if (matchTime.tm_mon == rhs.matchTime.tm_mon){
        if (matchTime.tm_mday == rhs.matchTime.tm_mday){
            if (matchTime.tm_hour == rhs.matchTime.tm_hour){
                if (matchTime.tm_min == rhs.matchTime.tm_min){
                    return true;
                }
                return (matchTime.tm_min < rhs.matchTime.tm_min);
            }
            return (matchTime.tm_hour < rhs.matchTime.tm_hour);
        }
        return (matchTime.tm_mday < rhs.matchTime.tm_mday);
    }
    return (matchTime.tm_mon < rhs.matchTime.tm_mon);
}

