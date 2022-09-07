//
// Created by ankit on 9/7/22.
//

#include "include/MatchEmbeds.h"

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
    std::ostringstream title;
    title << "Match #" << matchID << " Complete";

    dpp::embed embed = UtilityEmbeds::embedTemplate()
            .set_title(title.str());

    embed.add_field("Home", dpp::find_role(RecordBook::schedule[matchID].homeID)->get_mention(), true);
    embed.add_field("Away", dpp::find_role(RecordBook::schedule[matchID].awayID)->get_mention(), true);

    string allStats;
    std::ostringstream headerLine;
    headerLine << "            " << "Home" << "    " << "Away";
    for (const auto& [key, _] : RecordBook::schedule[matchID].matchScores){
        int homeGoals = 0, awayGoals = 0;
        for (auto score : RecordBook::schedule[matchID].matchScores[key]){
            homeGoals += score.homeGoals;
            awayGoals += score.awayGoals;
        }
        std::ostringstream gameStats;
        gameStats << "Game #" << key << "      " << homeGoals << "       " << awayGoals;
        allStats += gameStats.str() + "\n";
    }
    embed.add_field("Game Stats", "```" + headerLine.str() + "\n" + allStats + "```", false);
    switch (RecordBook::schedule[matchID].matchWinner){
        case (Match::affiliation::HOME):
            embed.add_field("Winner", dpp::find_role(RecordBook::schedule[matchID].homeID)->get_mention(), false);
            break;
        case (Match::affiliation::AWAY):
            embed.add_field("Winner", dpp::find_role(RecordBook::schedule[matchID].awayID)->get_mention(), false);
            break;
        case Match::NONE:
            break;
    }

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