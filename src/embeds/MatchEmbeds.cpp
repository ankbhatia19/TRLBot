//
// Created by ankit on 9/7/22.
//

#include "MatchEmbeds.h"

dpp::embed MatchEmbeds::matchReplayProcessing(int matchID) {
    std::ostringstream processing;
    processing << "Processing replay for match " << matchID;

    dpp::embed embed = UtilityEmbeds::embedTemplate()
            .set_title("Replay Processing")
            .add_field(
                    processing.str(),
                    "Please do not use any other bot commands during this time.",
                    false
            );

    return embed;
}

dpp::embed MatchEmbeds::matchNotFound(int matchID) {
    std::ostringstream error;
    error << "Match not found for Match #" << matchID;

    dpp::embed embed = UtilityEmbeds::embedTemplate()
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

dpp::embed MatchEmbeds::matchAlreadyPlayed(int matchID) {
    std::ostringstream error;
    error << "Match #" << matchID << " has already been submitted.";

    dpp::embed embed = UtilityEmbeds::embedTemplate()
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

dpp::embed MatchEmbeds::matchCompleteEmbed(int matchID) {

    dpp::embed embed = UtilityEmbeds::embedTemplate()
            .set_title((std::ostringstream{} << "Match #" << matchID).str());


    SQLite::Database db("rocket_league.db", SQLite::OPEN_READWRITE);
    vector<Match::score> scores = Match::tally(db, matchID);

    embed.add_field("Home", dpp::find_role(scores[0].home_team)->get_mention(), true);
    embed.add_field("Away", dpp::find_role(scores[0].away_team)->get_mention(), true);

    string allStats;

    Match::score series_score{0, 0, 0, 0, 0};
    allStats += (std::ostringstream{} << "            " << "Home" << "    " << "Away\n").str();
    for (int i = 0; i < scores.size(); i++){
        allStats += (std::ostringstream{}<< "Game #" << i << "      " << scores[i].homeGoals << "       " << scores[i].awayGoals).str() + "\n";

        if (scores[i].homeGoals > scores[i].awayGoals)
            series_score.homeGoals++;
        if (scores[i].awayGoals > scores[i].homeGoals)
            series_score.awayGoals++;
    }

    embed.add_field("Game Stats", "```" + allStats + "```", false);

    std::ostringstream winnerLine;
    if (series_score.homeGoals > series_score.awayGoals) {
        winnerLine << dpp::find_role(scores[0].home_team)->get_mention();
        winnerLine << " **(" << series_score.homeGoals;
        winnerLine << " - " << series_score.awayGoals << ")**";
        embed.add_field("Winner", winnerLine.str(), false);
    }
    if (series_score.awayGoals > series_score.homeGoals) {
        winnerLine << dpp::find_role(scores[0].away_team)->get_mention();
        winnerLine << " **(" << series_score.awayGoals;
        winnerLine << " - " << series_score.homeGoals << ")**";
        embed.add_field("Winner", winnerLine.str(), false);
    }

    embed.add_field("Ballchasing Group",
                    "https://ballchasing.com/group/" + Match::get_ballchasing_id(db, matchID),
                    false
    );

    return embed;
}

dpp::embed MatchEmbeds::matchPlayersNotRegistered(vector<string> unregistered) {

    std::ostringstream unregisteredString;
    for (string username : unregistered){
        unregisteredString << "\"" << username << "\" ";
    }

    dpp::embed embed = UtilityEmbeds::embedTemplate()
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

dpp::embed MatchEmbeds::matchPlayersNotOnTeam(vector<int64_t> teamless, int matchID) {
    std::ostringstream unregisteredString;
    for (auto id : teamless){
        unregisteredString << dpp::find_user(id)->get_mention() << " ";
    }

    dpp::embed embed = UtilityEmbeds::embedTemplate()
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

dpp::embed MatchEmbeds::matchHelpEmbed() {
    std::ostringstream body;
    body << dpp::utility::slashcommand_mention(Utilities::cmd_map["match"], "match", "create");
    body << " `[Home Role]` `[Away Role]`\\*: Create a match between two teams. ";
    body << "The generated ID will be used to submit replays after match completion.\n\n";
    body << dpp::utility::slashcommand_mention(Utilities::cmd_map["match"], "match", "submit");
    body << " `[Match ID]` `[Replay Files]`: Submit replays for a given match. Currently supports BO5 format (minimum of 3 and maximum of 5 .replay files).\n\n";
    body << dpp::utility::slashcommand_mention(Utilities::cmd_map["match"], "match", "remove");
    body << "`[Match ID]`\\*: Remove a given match.\n\n";
    body << "Commands marked by an asterisk (\\*) are only usable by League Staff.";

    dpp::embed embed = UtilityEmbeds::embedTemplate()
            .set_title("Help Page: Match")
            .add_field("Info", body.str(), false);

    return embed;
}

dpp::embed MatchEmbeds::matchRemoved(int matchID) {
    dpp::embed embed = UtilityEmbeds::embedTemplate()
            .set_title("Match Removed")
            .add_field("The following match has been removed:", std::to_string(matchID), false);

    return embed;
}
