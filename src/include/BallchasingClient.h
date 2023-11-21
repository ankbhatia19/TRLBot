//
// Created by ankit on 9/8/22.
//

#ifndef TRLBOT_BALLCHASINGCLIENT_H
#define TRLBOT_BALLCHASINGCLIENT_H

#include <httplib.h>
#include <dpp/dpp.h>
#include <string>
#include <nlohmann/json.hpp>

#include "Utilities.h"

using namespace std;
using json = nlohmann::json;

class BallchasingClient {
public:
    BallchasingClient();

    json upload(string path, string replayName);
    json create(int matchID);
    void group(string replayName, string group, string ballchasingID);
    json pull(string ballchasingID);

private:
    httplib::SSLClient client;

    static string token;
    static string group_id;

};


#endif //TRLBOT_BALLCHASINGCLIENT_H
