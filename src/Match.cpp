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

void Match::table_init(SQLite::Database &db) {
    try {
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
        std::cout << "matches table initialized successfully." << std::endl;
    } catch (const SQLite::Exception &e) {
        std::cerr << "SQLite error while initializing table: " << e.what() << std::endl;
    } catch (const std::exception &e) {
        std::cerr << "Standard error while initializing table: " << e.what() << std::endl;
    }
}

bool matchExists(SQLite::Database& db, int64_t match_id) {
    // Prepare the query
    std::string query = "SELECT COUNT(1) FROM matches WHERE match_id = ?";

    // Prepare the statement
    SQLite::Statement queryStmt(db, query);
    queryStmt.bind(1, match_id);  // Bind the input match_id

    // Execute the query
    queryStmt.executeStep();

    // Get the result
    int count = queryStmt.getColumn(0).getInt();
    return count > 0;
}

int64_t Match::create(SQLite::Database &db, int64_t home_id, int64_t away_id) {

    int64_t id;

    do {
        id = generateID();
    } while (matchExists(db, id));

    // Prepare the insert statement
    SQLite::Statement insertStmt(db, R"(
        INSERT INTO matches (match_id, home_team_id, away_team_id, match_time, match_status, ballchasing_id)
        VALUES (?, ?, ?, ?, ?, "")
    )");

    // Bind values
    insertStmt.bind(1, id);
    insertStmt.bind(2, home_id);
    insertStmt.bind(3, away_id);
    insertStmt.bind(4, std::time(nullptr));
    insertStmt.bind(5, Match::status::UNPLAYED);

    // Execute the insert statement
    insertStmt.exec();

    return id;
}

vector<Match::score> Match::tally(SQLite::Database &db, int64_t match_id) {

    SQLite::Statement query(db, R"(
        SELECT
            ps.game_num,
            home_team.team_id AS home_team_id,
            away_team.team_id AS away_team_id,
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
            ps.match_id = ?  -- Replace with the specific match ID
        GROUP BY
            ps.game_num;

    )");

    query.bind(1, match_id);

    vector<Match::score> scores;

    while (query.executeStep()){
        std::cout << query.getColumn(0).getInt() << "\t";
        std::cout << query.getColumn(1).getInt64() << "\t";
        std::cout << query.getColumn(2).getInt64() << "\t";
        std::cout << query.getColumn(3).getInt() << "\t";
        std::cout << query.getColumn(4).getInt() << std::endl;

        scores.emplace_back(Match::score{
            query.getColumn(0).getInt(),
            query.getColumn(1).getInt64(),
            query.getColumn(2).getInt64(),
            query.getColumn(3).getInt(),
            query.getColumn(4).getInt()
        });
    }
    return scores;
}

void Match::set_status(SQLite::Database &db, int64_t match_id, Match::status stat) {
    // Prepare the SQL query to update match_status for the given match_id
    SQLite::Statement queryStmt(db, R"(
        UPDATE matches
        SET match_status = ?
        WHERE match_id = ?
    )");

    // Bind the parameters to the prepared statement
    queryStmt.bind(1, static_cast<int>(stat));  // Convert Match::status to int for storage
    queryStmt.bind(2, match_id);

    // Execute the statement
    queryStmt.exec();
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


