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
    const char* botToken = std::getenv("BOT_TOKEN");
    return botToken ? std::string(botToken) : "";
}

string Utilities::getBallchasingToken() {
    const char* ballchasingToken = std::getenv("BALLCHASING_TOKEN");
    return ballchasingToken ? std::string(ballchasingToken) : "";
}


string Utilities::getBallchasingGroup() {
    const char* ballchasingGroup = std::getenv("BALLCHASING_GROUP");
    return ballchasingGroup ? std::string(ballchasingGroup) : "";
}


unsigned long long Utilities::getScoreReportChannel() {
    const char* reportChannel = std::getenv("REPORT_CHANNEL");
    return reportChannel ? std::stoll(reportChannel) : 0;
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

SQLite::Database Utilities::get_db() {
    return {getBallchasingGroup() + ".db", SQLite::OPEN_READWRITE};
}

