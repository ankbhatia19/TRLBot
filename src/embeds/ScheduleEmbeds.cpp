//
// Created by ankit on 9/7/22.
//

#include "include/ScheduleEmbeds.h"

dpp::embed ScheduleEmbeds::scheduleViewAllMatches() {
    std::ostringstream  unplayedMatchIDs;
    std::ostringstream unplayedHomeTeams, unplayedAwayTeams;

    std::ostringstream  playedMatchIDs;
    std::ostringstream playedHomeTeams, playedAwayTeams;
    vector<Match> playedMatches, unplayedMatches;

    // Sort matches by stats
    for (const auto& [key, _] : RecordBook::schedule){
        if (RecordBook::schedule[key].matchStatus == Match::status::UNPLAYED){
            unplayedMatches.emplace_back(RecordBook::schedule[key]);
        }
        else if (RecordBook::schedule[key].matchStatus == Match::status::PLAYED){
            playedMatches.emplace_back(RecordBook::schedule[key]);
        }
    }

    if (playedMatches.empty()){
        playedMatchIDs << "None";
        playedHomeTeams << "None";
        playedAwayTeams << "None";
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
    for (Match match : playedMatches){
        playedMatchIDs << match.id << "\n";
        playedHomeTeams << dpp::find_role(match.homeID)->get_mention() << "\n";
        playedAwayTeams << dpp::find_role(match.awayID)->get_mention() << "\n";
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
            )
            .add_field(
                    "__Completed__",
                    "_ _",
                    false
            )
            .add_field(
                    "Match ID",
                    playedMatchIDs.str(),
                    true
            )
            .add_field(
                    "Home",
                    playedHomeTeams.str(),
                    true
            )
            .add_field(
                    "Away",
                    playedAwayTeams.str(),
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
    std::ostringstream scheduledTime;
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

    if (match.matchTime == nullptr)
        scheduledTime << "None";

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
                    /*.add_field(
                            "Scheduled Time",
                            scheduledTime.str(),
                            false
                    )*/
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