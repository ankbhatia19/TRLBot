//
// Created by ankit on 9/7/22.
//

#include "ScheduleEmbeds.h"


dpp::embed ScheduleEmbeds::scheduleViewMatch(int id) {
    SQLite::Database db("rocket_league.db", SQLite::OPEN_READWRITE);

    if (!Match::has_id(db, id))
        return MatchEmbeds::matchNotFound(id);

    if (Match::get_status(db, id) == Match::status::PLAYED)
        return MatchEmbeds::matchCompleteEmbed(id);

    std::ostringstream matchIDstr;
    std::ostringstream homeTeamPlayers;
    std::ostringstream awayTeamPlayers;
    matchIDstr << "Match ID: " << id;

    int64_t home_id = Match::get_team(db, id, Match::HOME);
    int64_t away_id = Match::get_team(db, id, Match::AWAY);


    vector<int64_t> home_players = Team::get_players(db, home_id);
    vector<int64_t> away_players = Team::get_players(db, away_id);

    homeTeamPlayers << dpp::find_role(home_id)->get_mention() << "\n_ _\n__Roster__\n";
    awayTeamPlayers << dpp::find_role(away_id)->get_mention() << "\n_ _\n__Roster__\n";

    if (home_players.empty())
        homeTeamPlayers << "None\n";

    if (away_players.empty())
        awayTeamPlayers << "None\n";

    for (auto player_id : home_players)
        homeTeamPlayers << dpp::find_user(player_id)->get_mention() << "\n";

    for (auto player_id : away_players)
        awayTeamPlayers << dpp::find_user(player_id)->get_mention() << "\n";

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
