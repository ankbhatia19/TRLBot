//
// Created by ankit on 9/7/22.
//

#include "MatchEmbeds.h"

dpp::embed MatchEmbeds::replay_processing(int matchID) {
    std::ostringstream processing;
    processing << "Processing replay for match " << matchID;

    dpp::embed embed = UtilityEmbeds::base()
            .set_title("Replay Processing")
            .add_field(
                    processing.str(),
                    "Please do not use any other bot commands during this time.",
                    false
            );

    return embed;
}

dpp::embed MatchEmbeds::error_not_found(int matchID) {
    std::ostringstream error;
    error << "Match not found for Match #" << matchID;

    dpp::embed embed = UtilityEmbeds::base()
            .set_title("Error")
            .add_field(
                    error.str(),
                    "Use "
                    + dpp::utility::slashcommand_mention(Utilities::cmd_map["match"], "match", "create")
                    + " to create a new match.",
                    false
            );

    return embed;
}

dpp::embed MatchEmbeds::error_duplicate_submission(int matchID) {
    std::ostringstream error;
    error << "Match #" << matchID << " has already been submitted.";

    dpp::embed embed = UtilityEmbeds::base()
            .set_title("Error")
            .add_field(
                    error.str(),
                    "Use "
                    + dpp::utility::slashcommand_mention(Utilities::cmd_map["match"], "match", "create")
                    + " to create a new match.",
                    false
            );

    return embed;
}

dpp::embed MatchEmbeds::complete(int matchID) {

    SQLite::Database db("rocket_league.db", SQLite::OPEN_READWRITE);

    dpp::embed embed = UtilityEmbeds::base()
            .set_title((std::ostringstream{} << "Match #" << matchID).str());

    // first = home, second = away
    auto series_score = Match::score(db, matchID);
    auto game_scores = Match::tally(db, matchID);

    embed.add_field("Home", dpp::find_role(series_score.home_id)->get_mention(), true);
    embed.add_field("Away", dpp::find_role(series_score.away_id)->get_mention(), true);


    std::ostringstream stats;
    stats << "            " << "Home" << "    " << "Away" << std::endl;

    for (auto score : game_scores)
        stats << "Game #" << score.game_num << "      " << score.home_goals << "       " << score.away_goals << std::endl;

    embed.add_field("Game Stats", "```" + stats.str() + "```", false);

    std::ostringstream winner;
    if (series_score.home_score > series_score.away_score) {
        winner << dpp::find_role(series_score.home_id)->get_mention();
        winner << " **(" << series_score.home_score;
        winner << " - " << series_score.away_score << ")**";
    }
    if (series_score.away_score > series_score.home_score) {
        winner << dpp::find_role(series_score.away_id)->get_mention();
        winner << " **(" << series_score.away_score;
        winner << " - " << series_score.home_score << ")**";
    }

    embed.add_field("Winner", winner.str(), false);

    embed.add_field("Ballchasing Group",
                    "https://ballchasing.com/group/" + Match::get_ballchasing_id(db, matchID),
                    false
    );

    return embed;
}

dpp::embed MatchEmbeds::error_missing_username(vector<string> unregistered) {

    std::ostringstream unregisteredString;
    for (string username : unregistered){
        unregisteredString << "\"" << username << "\" ";
    }

    dpp::embed embed = UtilityEmbeds::base()
            .set_title("Error Submitting Replays")
            .add_field(
                    "The following players are unregistered:",
                    unregisteredString.str() + "\n\nUse "
                    + dpp::utility::slashcommand_mention(Utilities::cmd_map["player"], "player", "register")
                    + " to add unknown players.",
                    false
            );

    return embed;
}

dpp::embed MatchEmbeds::error_missing_team(vector<int64_t> teamless, int matchID) {
    std::ostringstream unregisteredString;
    for (auto id : teamless){
        unregisteredString << dpp::find_user(id)->get_mention() << " ";
    }

    dpp::embed embed = UtilityEmbeds::base()
            .set_title("Error Submitting Replays")
            .add_field(
                    "Please ensure all players are on a team.",
                    "The following players are not registered to either team: "
                    //+ dpp::find_role(RecordBook::schedule[matchID].homeID)->get_mention()
                    //+ " or "
                    //+ dpp::find_role(RecordBook::schedule[matchID].awayID)->get_mention()
                    //+ ":\n\n"
                    + unregisteredString.str()
                    + "\n\nUse "
                    + dpp::utility::slashcommand_mention(Utilities::cmd_map["team"], "team", "add")
                    + " to add players to a team.",
                    false
            );

    return embed;
}

dpp::embed MatchEmbeds::help() {
    std::ostringstream body;
    body << dpp::utility::slashcommand_mention(Utilities::cmd_map["match"], "match", "create");
    body << " `[Home Role]` `[Away Role]`\\*: Create a match between two teams. ";
    body << "The generated ID will be used to submit replays after match completion.\n\n";
    body << dpp::utility::slashcommand_mention(Utilities::cmd_map["match"], "match", "submit");
    body << " `[Match ID]` `[Replay Files]`: Submit replays for a given match. Currently supports BO5 format (minimum of 3 and maximum of 5 .replay files).\n\n";
    body << dpp::utility::slashcommand_mention(Utilities::cmd_map["match"], "match", "remove");
    body << "`[Match ID]`\\*: Remove a given match.\n\n";
    body << "Commands marked by an asterisk (\\*) are only usable by League Staff.";

    dpp::embed embed = UtilityEmbeds::base()
            .set_title("Help Page: Match")
            .add_field("Info", body.str(), false);

    return embed;
}

dpp::embed MatchEmbeds::removed(int matchID) {
    dpp::embed embed = UtilityEmbeds::base()
            .set_title("Match Removed")
            .add_field("The following match has been removed:", std::to_string(matchID), false);

    return embed;
}
