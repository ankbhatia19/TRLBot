//
// Created by ankit on 9/8/22.
//

#include "BallchasingClient.h"

string BallchasingClient::token = Utilities::getBallchasingToken();
string BallchasingClient::group_id = Utilities::getBallchasingGroup();

BallchasingClient::BallchasingClient() : client("ballchasing.com"){
    client.enable_server_certificate_verification(true);
}

json BallchasingClient::upload(string path, string replayName) {
    httplib::MultipartFormDataItems items = {
            { "file", dpp::utility::read_file(path), replayName, "multipart/form-data" },
    };

    auto uploadRes = client.Post("/api/v2/upload?visibility=public", {{"Authorization", BallchasingClient::token}}, items);
    return json::parse(uploadRes.value().body);
}

json BallchasingClient::create(int matchID) {
    // create a new subgroup for this match
    json post = {
            {"name", std::to_string(matchID)},
            {"parent", BallchasingClient::group_id},
            {"player_identification", "by-id"},
            {"team_identification", "by-player-clusters"}
    };

    auto postRes = client.Post("/api/groups",
                               {{"Authorization", BallchasingClient::token}},
                               to_string(post),
                               "application/json"
    );

    return json::parse(postRes.value().body);
}


void BallchasingClient::group(string replayName, string group, string ballchasingID) {

    json patch = {
            {"title", replayName},
            {"group", group}
    };

    auto patchRes = client.Patch("/api/replays/" + ballchasingID,
                                 {{"Authorization", BallchasingClient::token}},
                                 to_string(patch),
                                 "application/json"
    );
}

json BallchasingClient::pull(string ballchasingID) {
    string getEndpoint = "/api/replays/" + ballchasingID;
    json replayData;

    do {
        std::this_thread::sleep_for (std::chrono::milliseconds(500));
        auto replayRes = client.Get(getEndpoint, {{"Authorization", BallchasingClient::token}});
        replayData = json::parse(replayRes.value().body);
    } while (replayData["status"].get<std::string>() == "pending");

    return replayData;
}

