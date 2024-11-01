//
// Created by ankit on 9/7/22.
//

#include "TeamEmbeds.h"
#include <ranges>

dpp::embed TeamEmbeds::registered(dpp::role team) {

    dpp::embed embed = UtilityEmbeds::base()
            .set_title("Team Created")
            .add_field(
                    "Registered team: ",
                    team.get_mention() + "\n\nUse "
                    + dpp::utility::slashcommand_mention(Utilities::cmd_map["team"], "team", "add")
                    + " to fill the team roster."
            );

    return embed;
}

dpp::embed TeamEmbeds::delisted(dpp::role team) {

    dpp::embed embed = UtilityEmbeds::base()
            .set_title("Team Removed")
            .add_field(
                    "Delisted team: ",
                    team.get_mention() + "\n\nUse "
                    + dpp::utility::slashcommand_mention(Utilities::cmd_map["team"], "team", "register")
                    + " to re-register this team."
            );

    return embed;
}

dpp::embed TeamEmbeds::added_players(vector<int64_t> users, int64_t team) {

    std::ostringstream players;
    for (auto user : users)
        players << dpp::find_user(user)->get_mention() + " ";

    dpp::embed embed = UtilityEmbeds::base()
            .set_title("Player Added")
            .add_field(
                    "Registered player(s): ",
                    players.str() + "to " + dpp::find_role(team)->get_mention() + "\n\nUse "
                    + dpp::utility::slashcommand_mention(Utilities::cmd_map["player"], "player", "info")
                    + " to view the player(s)."
            );

    return embed;
}

dpp::embed TeamEmbeds::removed_player(dpp::user player, dpp::role team) {

    dpp::embed embed = UtilityEmbeds::base()
            .set_title("Player Removed")
            .add_field(
                    "Unregistered player: ",
                    player.get_mention() + " from " + team.get_mention() + "\n\nUse "
                    + dpp::utility::slashcommand_mention(Utilities::cmd_map["player"], "player", "info")
                    + " to view this player."
            );

    return embed;
}

dpp::embed TeamEmbeds::error_duplicate_player(dpp::user player, dpp::role team) {

    dpp::embed embed = UtilityEmbeds::base()
            .set_title("Error")
            .add_field(
                    "Player already registered: " ,
                    player.get_mention() + " is a member of " + team.get_mention() + "\n\nUse "
                    + dpp::utility::slashcommand_mention(Utilities::cmd_map["team"], "team", "view")
                    + " to view this team."
            );

    return embed;
}

dpp::embed TeamEmbeds::error_player_not_on_team(dpp::user player, dpp::role team) {

    dpp::embed embed = UtilityEmbeds::base()
            .set_title("Error")
            .add_field(
                    "Player not registered: " ,
                    player.get_mention() + "is not a member of " + team.get_mention() + "\n\nUse "
                    + dpp::utility::slashcommand_mention(Utilities::cmd_map["team"], "team", "view")
                    + " to view this team."
            );

    return embed;
}

dpp::embed TeamEmbeds::view(vector<int64_t> teams) {

    std::ostringstream teamList;
    std::ostringstream teamDiff;


    dpp::embed embed = UtilityEmbeds::base()
            .set_title("Team Leaderboard")
            .add_field(
                    "Team",
                    teamList.str(),
                    true
            )
            .add_field(
                    "Differential",
                    teamDiff.str(),
                    true
            );

    return embed;
}


dpp::embed TeamEmbeds::help() {
    std::ostringstream body;
    body << dpp::utility::slashcommand_mention(Utilities::cmd_map["team"], "team", "register");
    body << " `[Team Role]`\\*: Register a role as a valid TRL team.\n\n";
    body << dpp::utility::slashcommand_mention(Utilities::cmd_map["team"], "team", "delist");
    body << " `[Team Role]`\\*: Remove a role from being a valid TRL team.\n\n";
    body << dpp::utility::slashcommand_mention(Utilities::cmd_map["team"], "team", "add");
    body << " `[Team Role]` `[Players]`\\*: Add players to the provided team. Supports a minimum of one and a maximum of three players to be added at the same time.\n"
            "**Players may only join one team at a time, and will automatically be removed from other teams they are registered in.**\n\n";
    body << dpp::utility::slashcommand_mention(Utilities::cmd_map["team"], "team", "view");
    body << ": View all registered teams.\n\n";
    body << dpp::utility::slashcommand_mention(Utilities::cmd_map["team"], "team", "view");
    body << " `[Team Role]`: View the team card and statistics of the provided team.\n\n";
    body << "Commands marked by an asterisk (\\*) are only usable by League Staff.";

    dpp::embed embed = UtilityEmbeds::base()
            .set_title("Help Page: Team")
            .add_field("Info", body.str(), false);

    return embed;
}