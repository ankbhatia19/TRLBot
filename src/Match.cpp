//
// Created by ankit on 8/24/22.
//

#include "Match.h"

int generateID(){
    // Get a random number
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_real_distribution<double> dist(10000, 100000);
    return dist(mt); // always return a 5 digit random id
}


void Match::table_init(SQLite::Database &db) {

    db.exec(
            "CREATE TABLE IF NOT EXISTS matches ("
            "match_id INTEGER NOT NULL, "
            "home_team_id INTEGER, "
            "away_team_id INTEGER, "
            "match_time INTEGER, "
            "match_status INTEGER, "
            "ballchasing_id TEXT, "
            "PRIMARY KEY (match_id));"
    );
}

int64_t Match::create(SQLite::Database &db, int64_t home_id, int64_t away_id) {

    int64_t id;

    do {
        id = generateID();
    } while (has_id(db, id));

    // Prepare the insert statement
    SQLite::Statement query(db, R"(
        INSERT INTO matches (match_id, home_team_id, away_team_id, match_time, match_status, ballchasing_id)
        VALUES (?, ?, ?, ?, ?, "")
    )");

    // Bind values
    query.bind(1, id);
    query.bind(2, home_id);
    query.bind(3, away_id);
    query.bind(4, std::time(nullptr));
    query.bind(5, Match::status::UNPLAYED);

    // Execute the insert statement
    query.exec();

    return id;
}

vector<Game::score> Match::tally(SQLite::Database &db, int64_t match_id) {

    SQLite::Statement query(db, R"(
        SELECT
            ps.game_num,
            home_team.team_id AS home_team_id,
            away_team.team_id AS away_team_id,
            COALESCE(SUM(CASE WHEN ps.player_id IN (away_team.player_1, away_team.player_2, away_team.player_3) THEN ps.goals END), 0) AS away_team_goals,
            COALESCE(SUM(CASE WHEN ps.player_id IN (home_team.player_1, home_team.player_2, home_team.player_3) THEN ps.goals END), 0) AS home_team_goals

        FROM
            player_stats AS ps
        JOIN
            matches AS m ON ps.match_id = m.match_id
        JOIN
            team_ids AS home_team ON m.home_team_id = home_team.team_id
        JOIN
            team_ids AS away_team ON m.away_team_id = away_team.team_id
        WHERE
            ps.match_id = ?
        GROUP BY
            ps.game_num;
    )");

    query.bind(1, match_id);

    vector<Game::score> scores;

    while (query.executeStep()){

        scores.emplace_back(Game::score{
            query.getColumn(0).getInt(),
            query.getColumn(3).getInt(),
            query.getColumn(4).getInt()
        });
    }
    return scores;
}

void Match::set_status(SQLite::Database &db, int64_t match_id, Match::status stat) {
    // Prepare the SQL query to update match_status for the given match_id
    SQLite::Statement query(db, R"(
        UPDATE matches
        SET match_status = ?
        WHERE match_id = ?
    )");

    // Bind the parameters to the prepared statement
    query.bind(1, stat);
    query.bind(2, match_id);

    // Execute the statement
    query.exec();
}


void Match::set_ballchasing_id(SQLite::Database &db, int64_t match_id, std::string ballchasing_id) {
    // Prepare the SQL query to update ballchasing_id for the given match_id
    SQLite::Statement queryStmt(db, R"(
        UPDATE matches
        SET ballchasing_id = ?
        WHERE match_id = ?
    )");

    // Bind the parameters to the prepared statement
    queryStmt.bind(1, ballchasing_id);  // Bind ballchasing_id as a string
    queryStmt.bind(2, match_id);

    // Execute the statement
    queryStmt.exec();
}

Match::status Match::get_status(SQLite::Database &db, int64_t match_id) {
    // Prepare the SQL query to select match_status for the given match_id
    SQLite::Statement queryStmt(db, R"(
        SELECT match_status
        FROM matches
        WHERE match_id = ?
    )");

    // Bind the match_id to the query
    queryStmt.bind(1, match_id);

    // Execute the query and retrieve the match_status
    if (queryStmt.executeStep()) {
        return static_cast<Match::status>(queryStmt.getColumn(0).getInt());
    } else {
        return Match::status::UNPLAYED;
    }
}

std::string Match::get_ballchasing_id(SQLite::Database &db, int64_t match_id) {
    // Prepare the SQL query to select ballchasing_id for the given match_id
    SQLite::Statement queryStmt(db, R"(
        SELECT ballchasing_id
        FROM matches
        WHERE match_id = ?
    )");

    // Bind the match_id to the query
    queryStmt.bind(1, match_id);

    // Execute the query and retrieve the ballchasing_id
    if (queryStmt.executeStep()) {
        return queryStmt.getColumn(0).getText();
    } else {
        return "";
    }
}

int64_t Match::get_team(SQLite::Database &db, int64_t match_id, Match::affiliation affilation) {

    if (affilation == HOME) {
        SQLite::Statement query(db, R"(SELECT home_team_id FROM matches WHERE match_id = ?)");

        query.bind(1, match_id);
        if (query.executeStep())
            return query.getColumn(0).getInt64();
        return 0;
    }

    if (affilation == AWAY){
        SQLite::Statement query(db, R"(SELECT away_team_id FROM matches WHERE match_id = ?)");

        query.bind(1, match_id);
        if (query.executeStep())
            return query.getColumn(0).getInt64();
        return 0;
    }
    return 0;
}

bool Match::has_id(SQLite::Database &db, int64_t match_id) {

    SQLite::Statement query(db, R"(SELECT COUNT(*) FROM matches WHERE match_id = ?)");

    query.bind(1, match_id);

    if (query.executeStep())
        return query.getColumn(0).getInt() > 0;

    return false;
}

Match::series_score Match::score(SQLite::Database &db, int64_t match_id) {

    SQLite::Statement query(db, R"(
        WITH game_results AS (
            SELECT
                ps.game_num,
                m.home_team_id AS home_team_id,
                m.away_team_id AS away_team_id,
                COALESCE(SUM(CASE WHEN ps.player_id IN (home_team.player_1, home_team.player_2, home_team.player_3) THEN ps.goals END), 0) AS home_team_goals,
                COALESCE(SUM(CASE WHEN ps.player_id IN (away_team.player_1, away_team.player_2, away_team.player_3) THEN ps.goals END), 0) AS away_team_goals
            FROM
                player_stats AS ps
            JOIN
                matches AS m ON ps.match_id = m.match_id
            JOIN
                team_ids AS home_team ON m.home_team_id = home_team.team_id
            JOIN
                team_ids AS away_team ON m.away_team_id = away_team.team_id
            WHERE
                ps.match_id = ?
            GROUP BY
                ps.game_num
        )
        SELECT
            home_team_id,
            away_team_id,
            SUM(CASE WHEN home_team_goals > away_team_goals THEN 1 ELSE 0 END) AS home_score,
            SUM(CASE WHEN home_team_goals < away_team_goals THEN 1 ELSE 0 END) AS away_score
        FROM
            game_results
        WHERE
            home_team_goals IS NOT NULL OR away_team_goals IS NOT NULL;
    )");
    query.bind(1, match_id);

    if (query.executeStep())
        return {
                query.getColumn(0).getInt64(),
                query.getColumn(1).getInt64(),
                query.getColumn(2).getInt(),
                query.getColumn(3).getInt()
        };

    return {0, 0, 0, 0};
}


