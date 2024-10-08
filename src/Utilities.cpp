//
// Created by ankit on 9/4/22.
//

#include "Utilities.h"

map<string, unsigned long long> Utilities::cmd_map;

bool Utilities::checkPerms(dpp::interaction i) {
    auto roles = i.member.get_roles();
    for (auto snowflake : roles){
        if (dpp::find_role(snowflake)->name == "League Staff")
            return true;
    }
    return false;
}

string Utilities::getBotToken() {
    std::ifstream f("config.json");
    json data = json::parse(f);
    string token = data["bot_token"].get<string>();
    f.close();

    return token;
}

string Utilities::getBallchasingToken() {
    std::ifstream f("config.json");
    json data = json::parse(f);
    string token = data["ballchasing_token"].get<string>();
    f.close();

    return token;
}


string Utilities::getBallchasingGroup() {
    std::ifstream f("config.json");
    json data = json::parse(f);
    string group = data["ballchasing_group"].get<string>();
    f.close();

    return group;
}


unsigned long long Utilities::getScoreReportChannel() {
    std::ifstream f("config.json");
    json data = json::parse(f);
    unsigned long long channel = data["report_channel"].get<long>();
    f.close();

    return channel;
}


void Utilities::cmd_init(dpp::slashcommand_map cmds) {
    for (const auto& [id, _] : cmds)
        cmd_map.insert({cmds[id].name, id});
}

string Utilities::getQuote() {

    // Open quotes file
    std::ifstream f("quotes.json");
    json data = json::parse(f);
    f.close();

    // Get a random quote
    std::random_device rd;
    std::mt19937 mt(rd());
    int quotes_size = data["quotes"].size();
    std::uniform_real_distribution<double> dist(0, (quotes_size));

    return data["quotes"][(int)dist(mt)];
}

// Helper function to bind values dynamically
void Utilities::bind_val(SQLite::Statement& stmt, int index, const json& value) {
    if (value.is_null()) {
        stmt.bind(index, nullptr); // Bind NULL for null values
    } else if (value.is_boolean()) {
        stmt.bind(index, value.get<bool>());
    } else if (value.is_number_integer()) {
        stmt.bind(index, value.get<int64_t>());
    } else if (value.is_number_float()) {
        stmt.bind(index, value.get<double>());
    } else if (value.is_string()) {
        stmt.bind(index, value.get<std::string>());
    } else {
        throw std::runtime_error("Unsupported JSON type");
    }
}

