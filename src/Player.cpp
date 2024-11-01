//
// Created by ankit on 8/24/22.
//

#include "Player.h"


void Player::table_init(SQLite::Database &db) {

    db.exec(
            "CREATE TABLE IF NOT EXISTS player_ids ("
            "player_id INTEGER NOT NULL, "
            "team_id INTEGER, "
            "usernames TEXT, "
            "PRIMARY KEY (player_id));"
    );
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

bool Player::add_name(SQLite::Database &db, int64_t player_id, const string &username) {

    if (has_name(db, username))
        return false;

    if (!has_id(db, player_id)){
        SQLite::Statement query(db, R"(
            INSERT INTO player_ids (player_id, usernames) VALUES (?, ?)
        )");

        query.bind(1, player_id);
        query.bind(2, json(vector<string>{username}).dump());

        if (query.exec())
            return true;
    }
    else {
        SQLite::Statement query(db, R"(
            UPDATE player_ids
            SET usernames = json_set(usernames, '$[#]', ?)
            WHERE player_id = ?;
        )");

        query.bind(1, username);
        query.bind(2, player_id);

        if (query.exec())
            return true;
    }

    return false;
}

int64_t Player::get_id(SQLite::Database &db, const string &username) {

    // Prepare the statement
    SQLite::Statement query(db, R"(
            SELECT player_id
            FROM player_ids
            JOIN json_each(player_ids.usernames) AS username
            WHERE username.value = ?
        )");
    query.bind(1, username); // Bind the input username

    // Execute the query and retrieve results
    if (query.executeStep())
        return query.getColumn(0).getInt64();

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

bool Player::has_name(SQLite::Database &db, const string& username) {
    SQLite::Statement query(db, R"(
        SELECT
            COUNT(*)
        FROM
            player_ids,
            json_each(usernames) AS username
        WHERE
            username.value = ?;
    )");

    query.bind(1, username);

    if (query.executeStep())
        return query.getColumn(0).getInt() > 0;

    return false;
}



