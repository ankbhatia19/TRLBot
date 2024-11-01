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
    for (auto stat: json["stats"]) {
        stats.emplace_back(MatchStatistic{
                stat["matchID"],
                stat["shots"],
                stat["goals"],
                stat["saves"],
                stat["assists"]
        });
    }

    for (std::string alias: json["aliases"]) {
        aliases.emplace_back(alias);
    }
}

int Player::getStatistic(Player::statistic stat) {
    double total = 0;

    switch (stat) {
        case (GOALS):
            for (auto game: stats)
                total += game.goals;
            return (int) total;
        case (ASSISTS):
            for (auto game: stats)
                total += game.assists;
            return (int) total;
        case (SAVES):
            for (auto game: stats)
                total += game.saves;
            return (int) total;
        case (SHOTS):
            for (auto game: stats)
                total += game.shots;
            return (int) total;
        case (NUM_GAMES):
            return (int) stats.size();
        case (AVG_MVPR):
            // (Goals) + (Assists * 0.75) + (Saves * 0.6) + (Shots / 3) ) / Games Played
            for (auto game: stats)
                total += 250 + (250 * (game.goals + (game.assists * 0.75) + (game.saves * 0.6) + (game.shots / 3.0)));
            return (int) total;
    }
    return 0;
}

double Player::getStatisticAvg(Player::statistic stat) {
    return ((double) getStatistic(stat) / stats.size());
}

bool Player::containsAlias(string alias) {
    for (string username: aliases) {
        if (alias == username)
            return true;
    }
    return false;
}

nlohmann::json Player::to_json() {
    nlohmann::json json;
    json["id"] = id;
    json["teamID"] = teamID;
    for (int i = 0; i < stats.size(); i++) {
        json["stats"][i]["matchID"] = stats[i].matchID;
        json["stats"][i]["shots"] = stats[i].shots;
        json["stats"][i]["goals"] = stats[i].goals;
        json["stats"][i]["saves"] = stats[i].saves;
        json["stats"][i]["assists"] = stats[i].assists;
    }

    for (int i = 0; i < aliases.size(); i++) {
        json["aliases"][i] = aliases[i];
    }

    return json;
}

void Player::table_init(SQLite::Database &db) {
    try {
        db.exec(
                "CREATE TABLE IF NOT EXISTS player_ids ("
                "player_id INTEGER NOT NULL, "
                "team_id INTEGER, "
                "usernames TEXT, "
                "PRIMARY KEY (player_id));"
        );
        std::cout << "player_ids table initialized successfully." << std::endl;
    } catch (const SQLite::Exception &e) {
        std::cerr << "SQLite error while initializing table: " << e.what() << std::endl;
    } catch (const std::exception &e) {
        std::cerr << "Standard error while initializing table: " << e.what() << std::endl;
    }
}

Utilities::ErrorCode Player::add_team(SQLite::Database &db, int64_t player_id, int64_t team_id) {

    SQLite::Statement query(db, R"(
        UPDATE player_ids
        SET team_id = ?
        WHERE player_id = ?
    )");

    // Bind the parameters: team_id and player_id
    query.bind(1, team_id);
    query.bind(2, player_id);

    if (query.exec() == 0)  // if there were no changes made
        return Utilities::ErrorCode::kPlayerNotRegistered;

    return Utilities::ErrorCode::kSuccess;
}

Utilities::ErrorCode Player::add_name(SQLite::Database &db, int64_t player_id, const std::string &username) {
    // Prepare a statement to fetch the current usernames as JSON
    SQLite::Statement select_query(db, R"(
            SELECT usernames
            FROM player_ids
            WHERE player_id = ?;
        )");
    select_query.bind(1, player_id);

    std::string currentUsernamesJson;

    // Execute the query and get the current usernames (if there are any)
    if (select_query.executeStep())
        currentUsernamesJson = select_query.getColumn(0).getText();

    // Convert current usernames JSON to a vector of strings
    nlohmann::json jsonUsernames;
    std::vector <std::string> usernames;

    // If the JSON is an array, extract the existing usernames
    if (!currentUsernamesJson.empty()) {
        jsonUsernames = nlohmann::json::parse(currentUsernamesJson);

        if (jsonUsernames.is_array())
            usernames = jsonUsernames.get < std::vector < std::string >> ();
    }

    // Check if the username already exists in the vector (exact match)
    if (std::find(usernames.begin(), usernames.end(), username) != usernames.end())
        return Utilities::ErrorCode::kUsernameExists;

    // Only add the new username if it doesn't already exist
    usernames.push_back(username);

    // Convert the updated usernames vector back to JSON
    nlohmann::json updatedUsernamesJson = usernames;
    std::string updatedUsernamesJsonString = updatedUsernamesJson.dump();

    // Prepare the upsert statement
    SQLite::Statement upsertQuery(db, R"(
            INSERT INTO player_ids (player_id, usernames) VALUES (?, ?)
            ON CONFLICT(player_id) DO UPDATE SET usernames = ?;
        )");

    // Bind the values to the query
    upsertQuery.bind(1, player_id);
    upsertQuery.bind(2, updatedUsernamesJsonString);
    upsertQuery.bind(3, updatedUsernamesJsonString);

    // Execute the upsert query
    if (upsertQuery.exec() > 0)
        return Utilities::ErrorCode::kSuccess;

    return Utilities::ErrorCode::kError;
}

int64_t Player::get_id(SQLite::Database &db, const string &username) {

    // Prepare the statement
    SQLite::Statement queryStmt(db, R"(
            SELECT player_id
            FROM player_ids
            JOIN json_each(player_ids.usernames) AS username
            WHERE username.value = ?
        )");
    queryStmt.bind(1, username); // Bind the input username

    // Execute the query and retrieve results
    if (queryStmt.executeStep())
        return queryStmt.getColumn(0).getInt64();

    return 0;
}

int64_t Player::get_team(SQLite::Database &db, int64_t player_id) {
    std::string query = R"(
            SELECT team_id
            FROM player_ids
            WHERE player_id = ?
        )";

    SQLite::Statement statement(db, query);
    statement.bind(1, player_id);

    if (statement.executeStep())
        return statement.getColumn(0).getInt64();

    return 0;
}

bool Player::has_id(SQLite::Database &db, int64_t player_id) {
    SQLite::Statement query(db, R"(SELECT COUNT(*) FROM player_ids WHERE player_id = ?)");
    query.bind(1, player_id);

    if (query.executeStep())
        return query.getColumn(0).getInt() > 0;

    return false;
}

double Player::get_war(SQLite::Database &db, int64_t player_id) {
    SQLite::Statement query(db, R"(
        WITH season_averages AS (
            SELECT
                AVG(goals) AS avg_goals,
                AVG(assists) AS avg_assists,
                AVG(shots) AS avg_shots,
                AVG(goals_against) AS avg_goals_against,
                AVG(shots_against) AS avg_shots_against,
                AVG(saves) AS avg_saves,
                AVG(amount_stolen) AS avg_amount_stolen,
                AVG(demos_inflicted) AS avg_demos_inflicted,
                AVG(demos_taken) AS avg_demos_taken
            FROM player_stats
        ),
        player_war AS (
            SELECT
                ps.player_id,
                (0.45 * (
                    (ps.goals - sa.avg_goals)
                    + (ps.assists - sa.avg_assists) * 0.75
                    + ((ps.shots - ps.goals) - (sa.avg_shots - sa.avg_goals)) * 0.33
                ) +
                0.45 * (
                    (ps.saves - sa.avg_saves) * 0.6
                    - (ps.shots_against - sa.avg_shots_against) * 0.15
                    - (ps.goals_against - sa.avg_goals_against) * 0.33
                ) +
                0.1 * (
                    (ps.demos_inflicted - sa.avg_demos_inflicted) * 0.1
                    - (ps.demos_taken - sa.avg_demos_taken) * 0.1
                    + (ps.amount_stolen - sa.avg_amount_stolen) * 0.005
                ) + 2) AS WAR
            FROM player_stats ps
            CROSS JOIN season_averages sa
            WHERE ps.player_id = ?
        )
        SELECT player_id, WAR FROM player_war;
    )");

    query.bind(1, player_id);

    if (query.executeStep())
        return query.getColumn(1).getDouble();

    return 0;
}

vector<string> Player::get_names(SQLite::Database &db, int64_t player_id) {
    // Prepare the query to select the usernames for the specified player_id
    SQLite::Statement query(db, "SELECT usernames FROM player_ids WHERE player_id = ?");
    query.bind(1, player_id);

    // Execute the query and check if there's a result
    if (query.executeStep()) {
        // Get the JSON string from the result
        json usernames_json = json::parse(query.getColumn(0).getString());

        // Extract the usernames into the vector
        return usernames_json.get<vector<string>>();
    }

    return {};
}



