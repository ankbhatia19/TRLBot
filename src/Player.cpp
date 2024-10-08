//
// Created by ankit on 8/24/22.
//

#include "Player.h"

Player::Player(unsigned long long id) {
    Player::id = id;
    Player::teamID = 0;
}

Player::Player() {
    Player::id = 0;
    Player::teamID = 0;
}

Player::Player(nlohmann::json json) {
    id = json["id"];
    teamID = json["teamID"];
    for (auto stat : json["stats"]){
        stats.emplace_back(MatchStatistic{
                stat["matchID"],
                stat["shots"],
                stat["goals"],
                stat["saves"],
                stat["assists"]
        });
    }

    for (std::string alias : json["aliases"]){
        aliases.emplace_back(alias);
    }
}

int Player::getStatistic(Player::statistic stat) {
    double total = 0;

    switch (stat){
        case (GOALS):
            for (auto game : stats)
                total += game.goals;
            return (int)total;
        case (ASSISTS):
            for (auto game : stats)
                total += game.assists;
            return (int)total;
        case (SAVES):
            for (auto game : stats)
                total += game.saves;
            return (int)total;
        case (SHOTS):
            for (auto game : stats)
                total += game.shots;
            return (int)total;
        case (NUM_GAMES):
            return (int)stats.size();
        case (AVG_MVPR):
            // (Goals) + (Assists * 0.75) + (Saves * 0.6) + (Shots / 3) ) / Games Played
            for (auto game : stats)
                total += 250 + (250 * (game.goals + (game.assists * 0.75) + (game.saves * 0.6) + (game.shots / 3.0)));
            return (int)total;
    }
    return 0;
}

double Player::getStatisticAvg(Player::statistic stat) {
    return ((double)getStatistic(stat) / stats.size());
}

bool Player::containsAlias(string alias) {
    for (string username : aliases){
        if (alias == username)
            return true;
    }
    return false;
}

nlohmann::json Player::to_json() {
    nlohmann::json json;
    json["id"] = id;
    json["teamID"] = teamID;
    for (int i = 0; i < stats.size(); i++){
        json["stats"][i]["matchID"] = stats[i].matchID;
        json["stats"][i]["shots"] = stats[i].shots;
        json["stats"][i]["goals"] = stats[i].goals;
        json["stats"][i]["saves"] = stats[i].saves;
        json["stats"][i]["assists"] = stats[i].assists;
    }

    for (int i = 0; i < aliases.size(); i++){
        json["aliases"][i] = aliases[i];
    }

    return json;
}

void Player::table_init(SQLite::Database &db) {
    try {
        db.exec(
                "CREATE TABLE IF NOT EXISTS player_ids ("
                "player_id INTEGER NOT NULL, "
                "usernames TEXT, "
                "PRIMARY KEY (player_id));"
        );
        std::cout << "player_ids table initialized successfully." << std::endl;
    } catch (const SQLite::Exception& e) {
        std::cerr << "SQLite error while initializing table: " << e.what() << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Standard error while initializing table: " << e.what() << std::endl;
    }
}

void Player::table_upsert(SQLite::Database& db, int64_t player_id, const std::string& username) {
    try {
        // Prepare a statement to fetch the current usernames as JSON
        SQLite::Statement select_query(db, "SELECT usernames FROM player_ids WHERE player_id = ?;");
        select_query.bind(1, player_id);

        std::string currentUsernamesJson;

        // Execute the query and get the current usernames
        if (select_query.executeStep()) {
            currentUsernamesJson = select_query.getColumn(0).getText();
        }

        // Convert current usernames JSON to a vector of strings
        nlohmann::json jsonUsernames;
        std::vector<std::string> usernames;

        // If the JSON is an array, extract the existing usernames
        if (!currentUsernamesJson.empty()) {
            jsonUsernames = nlohmann::json::parse(currentUsernamesJson);
            if (jsonUsernames.is_array()) {
                usernames = jsonUsernames.get<std::vector<std::string>>();
            }
        }

        // Check if the username already exists in the vector (exact match)
        if (std::find(usernames.begin(), usernames.end(), username) == usernames.end()) {
            // Only add the new username if it doesn't already exist
            usernames.push_back(username);
        }

        // Convert the updated usernames vector back to JSON
        nlohmann::json updatedUsernamesJson = usernames;
        std::string updatedUsernamesJsonString = updatedUsernamesJson.dump();

        // Prepare the upsert statement
        SQLite::Statement upsertQuery(db,
                                      "INSERT INTO player_ids (player_id, usernames) VALUES (?, ?) "
                                      "ON CONFLICT(player_id) DO UPDATE SET usernames = ?;");

        // Bind the values to the query
        upsertQuery.bind(1, player_id);
        upsertQuery.bind(2, updatedUsernamesJsonString);
        upsertQuery.bind(3, updatedUsernamesJsonString);

        // Execute the upsert query
        upsertQuery.exec();

        std::cout << "Upserted player ID: " << player_id << " with usernames: " << updatedUsernamesJsonString << std::endl;
    } catch (const SQLite::Exception& e) {
        std::cerr << "SQLite error while upserting player ID: " << e.what() << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Standard error while upserting player ID: " << e.what() << std::endl;
    }
}

int64_t Player::table_get(SQLite::Database &db, const string &username) {
    // Prepare the query
    std::string query = R"(
            SELECT player_id
            FROM player_ids,
                 json_each(usernames) AS username
            WHERE username.value = ?
        )";

    // Prepare the statement
    SQLite::Statement queryStmt(db, query);
    queryStmt.bind(1, username); // Bind the input username

    // Execute the query and retrieve results
    while (queryStmt.executeStep()) {
        return queryStmt.getColumn(0).getInt64();
    }

    return 0;
}




