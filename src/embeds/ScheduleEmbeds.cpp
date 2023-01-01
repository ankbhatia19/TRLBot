//
// Created by ankit on 9/7/22.
//

#include "ScheduleEmbeds.h"

dpp::embed ScheduleEmbeds::scheduleViewAllMatches() {
    std::ostringstream  unplayedMatchIDs;
    std::ostringstream unplayedHomeTeams, unplayedAwayTeams;
    vector<Match> unplayedMatches;

    // Sort matches by stats
    for (const auto& [key, _] : RecordBook::schedule){
        if (RecordBook::schedule[key].matchStatus == Match::status::UNPLAYED){
            unplayedMatches.emplace_back(RecordBook::schedule[key]);
        }
    }

    if (unplayedMatches.empty()){
        unplayedMatchIDs << "None";
        unplayedHomeTeams << "None";
        unplayedAwayTeams << "None";
    }
    for (Match match : unplayedMatches){
        unplayedMatchIDs << match.id << "\n";
        unplayedHomeTeams << dpp::find_role(match.homeID)->get_mention() << "\n";
        unplayedAwayTeams << dpp::find_role(match.awayID)->get_mention() << "\n";
    }

    dpp::embed embed = UtilityEmbeds::embedTemplate()
            .set_title("All Scheduled Matches")
            .add_field(
                    "__Unplayed__",
                    "_ _",
                    false
            )
            .add_field(
                    "Match ID",
                    unplayedMatchIDs.str(),
                    true
            )
            .add_field(
                    "Home",
                    unplayedHomeTeams.str(),
                    true
            )
            .add_field(
                    "Away",
                    unplayedAwayTeams.str(),
                    true
            );

    return embed;
}

dpp::embed ScheduleEmbeds::scheduleViewMatch(int id) {
    Match match = RecordBook::schedule[id];
    if (match.matchStatus == Match::status::PLAYED)
        return MatchEmbeds::matchCompleteEmbed(id);

    std::ostringstream matchIDstr;
    std::ostringstream homeTeamPlayers;
    std::ostringstream awayTeamPlayers;
    std::ostringstream matchStatus;
    std::ostringstream lobbyInfo;

    matchIDstr << "Match ID: " << match.id;

    homeTeamPlayers << dpp::find_role(match.homeID)->get_mention() << "\n_ _\n__Roster__\n";
    awayTeamPlayers << dpp::find_role(match.awayID)->get_mention() << "\n_ _\n__Roster__\n";
    if (RecordBook::teams[match.homeID].members.empty())
        homeTeamPlayers << "None\n";

    if (RecordBook::teams[match.awayID].members.empty())
        awayTeamPlayers << "None\n";

    for (const auto& [id, _] : RecordBook::teams[match.homeID].members)
        homeTeamPlayers << dpp::find_user(id)->get_mention() << "\n";

    for (const auto& [id, _] : RecordBook::teams[match.awayID].members)
        awayTeamPlayers << dpp::find_user(id)->get_mention() << "\n";

    if (match.matchStatus == Match::status::PLAYED)
        matchStatus << "Complete";
    else{
        char formatted_time[50];
        std::strftime(formatted_time, sizeof(formatted_time), "%x at %I:%M %p", &match.matchTime);
        matchStatus << "To be played: " << formatted_time << " PST.";
    }

    lobbyInfo << "**User: ** trl\n**Pass: ** " << match.id;
    dpp::embed embed = UtilityEmbeds::embedTemplate()
            .set_title(matchIDstr.str())
            .add_field(
                    "Home",
                    homeTeamPlayers.str(),
                    true
            )
            .add_field(
                    "_          _",
                    "_          _",
                    true
            )
            .add_field(
                    "Away",
                    awayTeamPlayers.str(),
                    true
            )
            .add_field(
                    "Match Status",
                    matchStatus.str(),
                    false
            )
            .add_field(
                    "Lobby Info",
                    lobbyInfo.str(),
                    true
            );

    return embed;
}

dpp::embed ScheduleEmbeds::scheduleMatchDoesNotExist(int id) {

    std::ostringstream matchError;
    matchError << "Match ID " << id << " does not exist.";
    dpp::embed embed = UtilityEmbeds::embedTemplate()
            .set_title("Error")
            .add_field(
                    matchError.str(),
                    "Use "
                    + dpp::utility::slashcommand_mention(Utilities::cmd_map["match"], "match", "create")
                    + " to create a match."
            );

    return embed;
}

dpp::embed ScheduleEmbeds::scheduleHelpEmbed() {
    std::ostringstream body;
    body << dpp::utility::slashcommand_mention(Utilities::cmd_map["schedule"], "schedule", "view");
    body << ": View all matches, divided into unplayed and completed groups.\n\n";
    body << dpp::utility::slashcommand_mention(Utilities::cmd_map["schedule"], "schedule", "view");
    body << " `[Match ID]`: View the details of the desired match. The result differs depending on whether the match has been completed or not.\n\n";
    body << dpp::utility::slashcommand_mention(Utilities::cmd_map["schedule"], "schedule", "edit");
    body << " `[Match ID]` `[Date]` `[Time]` (Under Construction): Set the desired match to be played at the provided time.\n\n";

    body << "Commands marked by an asterisk (\\*) are only usable by League Staff.";

    dpp::embed embed = UtilityEmbeds::embedTemplate()
            .set_title("Help Page: Schedule")
            .add_field("Info", body.str(), false);

    return embed;
}

dpp::embed ScheduleEmbeds::scheduleInvalidTime() {
    std::ostringstream body;
    body << "You have entered an invalid time.";

    dpp::embed embed = UtilityEmbeds::embedTemplate()
            .set_title("Error")
            .add_field(
                    body.str(),
                    "Please use a 12-hour format with meridiems (AM/PM). Eg: 8:00pm"
            );

    return embed;
}
