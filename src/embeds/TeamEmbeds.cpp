//
// Created by ankit on 9/7/22.
//

#include "TeamEmbeds.h"
#include <ranges>

dpp::embed TeamEmbeds::teamUnregisteredEmbed(dpp::role home, dpp::role away) {
    std::ostringstream unregistered;

    if (!RecordBook::teams.contains(home.id)){
        unregistered << home.get_mention() << "\n";
    }
    if (!RecordBook::teams.contains(away.id)){
        unregistered << away.get_mention() << " \n";
    }

    dpp::embed embed = UtilityEmbeds::embedTemplate()
            .set_title("Error")
            .add_field(
                    "These team(s) have not been registered: ",
                    unregistered.str() + "\nUse "
                    + dpp::utility::slashcommand_mention(Utilities::cmd_map["team"], "team", "register")
                    + " to create a team."
            );

    return embed;
}

dpp::embed TeamEmbeds::teamUnregisteredEmbed(dpp::role team) {
    std::ostringstream unregistered;
    unregistered << "";
    if (!RecordBook::teams.contains(team.id)){
        unregistered << team.get_mention() << "\n";
    }
    dpp::embed embed = UtilityEmbeds::embedTemplate()
            .set_title("Error")
            .add_field(
                    "These team(s) have not been registered: ",
                    unregistered.str() + "\nUse "
                    + dpp::utility::slashcommand_mention(Utilities::cmd_map["team"], "team", "register")
                    + " to create a team."
            );

    return embed;
}

dpp::embed TeamEmbeds::teamRegisteredEmbed(dpp::role team) {

    dpp::embed embed = UtilityEmbeds::embedTemplate()
            .set_title("Team Created")
            .add_field(
                    "Registered team: ",
                    team.get_mention() + "\n\nUse "
                    + dpp::utility::slashcommand_mention(Utilities::cmd_map["team"], "team", "add")
                    + " to fill the team roster."
            );

    return embed;
}

dpp::embed TeamEmbeds::teamDelistedEmbed(dpp::role team) {

    dpp::embed embed = UtilityEmbeds::embedTemplate()
            .set_title("Team Removed")
            .add_field(
                    "Delisted team: ",
                    team.get_mention() + "\n\nUse "
                    + dpp::utility::slashcommand_mention(Utilities::cmd_map["team"], "team", "register")
                    + " to re-register this team."
            );

    return embed;
}

dpp::embed TeamEmbeds::teamAddedPlayersEmbed(vector<dpp::user> players, dpp::role team) {

    std::ostringstream playersStr;
    for (auto user : players)
        playersStr << user.get_mention() + " ";

    dpp::embed embed = UtilityEmbeds::embedTemplate()
            .set_title("Player Added")
            .add_field(
                    "Registered player(s): ",
                    playersStr.str() + "to " + team.get_mention() + "\n\nUse "
                    + dpp::utility::slashcommand_mention(Utilities::cmd_map["player"], "player", "info")
                    + " to view the player(s)."
            );

    return embed;
}

dpp::embed TeamEmbeds::teamRemovedPlayerEmbed(dpp::user player, dpp::role team) {

    dpp::embed embed = UtilityEmbeds::embedTemplate()
            .set_title("Player Removed")
            .add_field(
                    "Unregistered player: ",
                    player.get_mention() + " from " + team.get_mention() + "\n\nUse "
                    + dpp::utility::slashcommand_mention(Utilities::cmd_map["player"], "player", "info")
                    + " to view this player."
            );

    return embed;
}

dpp::embed TeamEmbeds::teamPlayerAlreadyRegisteredEmbed(dpp::user player, dpp::role team) {

    dpp::embed embed = UtilityEmbeds::embedTemplate()
            .set_title("Error")
            .add_field(
                    "Player already registered: " ,
                    player.get_mention() + " is a member of " + team.get_mention() + "\n\nUse "
                    + dpp::utility::slashcommand_mention(Utilities::cmd_map["team"], "team", "view")
                    + " to view this team."
            );

    return embed;
}

dpp::embed TeamEmbeds::teamPlayerUnregisteredEmbed(dpp::user player, dpp::role team) {

    dpp::embed embed = UtilityEmbeds::embedTemplate()
            .set_title("Error")
            .add_field(
                    "Player not registered: " ,
                    player.get_mention() + "is not a member of " + team.get_mention() + "\n\nUse "
                    + dpp::utility::slashcommand_mention(Utilities::cmd_map["team"], "team", "view")
                    + " to view this team."
            );

    return embed;
}

dpp::embed TeamEmbeds::teamViewAllEmbed(map<unsigned long long, Team> teams) {

    std::ostringstream teamList;
    std::ostringstream teamDiff;

    if (teams.empty())
        teamList << "No registered teams.";

    auto team_vals = std::views::values(teams);
    std::vector<Team> team_vector{ team_vals.begin(), team_vals.end() };
    // Sort by game differential
    std::sort(team_vector.begin(), team_vector.end());
    for (auto team : team_vector){
        teamList << dpp::find_role(team.id)->get_mention() << "\n";

        if (team.differential > 0)
            teamDiff << "+" << team.differential << "\n";
        else if (team.differential < 0)
            teamDiff << team.differential << "\n";
        else
            teamDiff << " " << team.differential << "\n";
    }

    dpp::embed embed = UtilityEmbeds::embedTemplate()
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

dpp::embed TeamEmbeds::teamViewRoleEmbed(dpp::role team) {
    std::ostringstream players;
    std::ostringstream stats;

    Team thisTeam = RecordBook::teams[team.id];

    if (thisTeam.members.empty()){
        players << "None\n";
    }
    for (const auto& [id, _] : thisTeam.members){
        if (dpp::find_user(id) == nullptr)
            return UtilityEmbeds::errorEmbed("Member list is still being loaded. Try again in a few minutes.");

        players << dpp::find_user(id)->get_mention() << "\n";
    }

    stats << "Series Wins:          " << thisTeam.wins << "\n";
    stats << "Series Losses:        " << thisTeam.losses << "\n";
    if (thisTeam.differential > 0)
        stats << "Game Differential:   +" << thisTeam.differential << "\n";
    else if (thisTeam.differential < 0)
        stats << "Game Differential:   " << thisTeam.differential << "\n";
    else
        stats << "Game Differential:    " << thisTeam.differential << "\n";

    dpp::embed embed = UtilityEmbeds::embedTemplate()
            .set_title("Team Card")
            .add_field(
                    "Team",
                    team.get_mention(),
                    true
            )
            .add_field(
                    "Roster",
                    players.str(),
                    true
            )
            .add_field(
                    "Stats",
                    "```" + stats.str() + "```"
            );

    return embed;
}

dpp::embed TeamEmbeds::teamHelpEmbed() {
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

    dpp::embed embed = UtilityEmbeds::embedTemplate()
            .set_title("Help Page: Team")
            .add_field("Info", body.str(), false);

    return embed;
}