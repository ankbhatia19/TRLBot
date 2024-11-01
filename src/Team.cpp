//
// Created by ankit on 8/24/22.
//

#include "Team.h"


void Team::table_init(SQLite::Database &db) {
    db.exec(
            "CREATE TABLE IF NOT EXISTS team_ids ("
            "team_id INTEGER NOT NULL, "
            "player_1 INTEGER, "
            "player_2 INTEGER, "
            "player_3 INTEGER, "
            "wins INTEGER, "
            "losses INTEGER, "
            "differential INTEGER, "
            "PRIMARY KEY (team_id));"
    );
}

Utilities::ErrorCode Team::add_player(SQLite::Database &db, int64_t team_id, int64_t player_id) {

    SQLite::Statement query(db, "SELECT player_1, player_2, player_3 FROM team_ids WHERE team_id = ?");
    query.bind(1, team_id);

    if (query.executeStep()) {
        // Get the values of player_1, player_2, and player_3
        int64_t player1 = query.getColumn(0).getInt64();
        int64_t player2 = query.getColumn(1).getInt64();
        int64_t player3 = query.getColumn(2).getInt64();

        // Check if the player_id is already in any of the slots
        if (player1 == player_id || player2 == player_id || player3 == player_id) {
            // If the player_id is already present, return an error
            return Utilities::kError;
        }

        std::string updateQuery;
        // Check which player slot is available and update accordingly
        if (player1 == 0) {
            updateQuery = "UPDATE team_ids SET player_1 = ? WHERE team_id = ?";
        } else if (player2 == 0) {
            updateQuery = "UPDATE team_ids SET player_2 = ? WHERE team_id = ?";
        } else if (player3 == 0) {
            updateQuery = "UPDATE team_ids SET player_3 = ? WHERE team_id = ?";
        } else {
            // If all slots are full, return an error
            return Utilities::kError;
        }

        // Prepare and execute the update query
        SQLite::Statement updateStmt(db, updateQuery);
        updateStmt.bind(1, player_id);
        updateStmt.bind(2, team_id);
        updateStmt.exec();

        return Utilities::kSuccess;
    } else {
        // If the team_id is not found, return an error
        return Utilities::kError;
    }
}

Utilities::ErrorCode Team::add_team(SQLite::Database &db, int64_t team_id) {
    // Prepare an insert statement to add a new team with initial values
    SQLite::Statement query(db, "INSERT INTO team_ids (team_id, player_1, player_2, player_3, wins, losses, differential) "
                                "VALUES (?, 0, 0, 0, 0, 0, 0)");

    // Bind the team_id to the query
    query.bind(1, team_id);

    // Execute the query
    query.exec();

    return Utilities::kSuccess;
}

vector<int64_t> Team::get_players(SQLite::Database &db, int64_t team_id) {

    SQLite::Statement query(db, "SELECT player_1, player_2, player_3 FROM team_ids WHERE team_id = ?");
    query.bind(1, team_id);

    vector<int64_t> roster;

    if (query.executeStep()) {
        // Get the values of player_1, player_2, and player_3
        int64_t player1 = query.getColumn(0).getInt64();
        int64_t player2 = query.getColumn(1).getInt64();
        int64_t player3 = query.getColumn(2).getInt64();

        if (player1 != 0)
            roster.emplace_back(player1);
        if (player2 != 0)
            roster.emplace_back(player2);
        if (player3 != 0)
            roster.emplace_back(player3);
    }
    return roster;
}
