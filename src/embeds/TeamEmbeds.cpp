//
// Created by ankit on 9/7/22.
//

#include "include/TeamEmbeds.h"

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
    std::ostringstream teamCount;

    if (teams.empty())
        teamList << "No registered teams.";

    for (const auto& [key, _] : teams){
        teamList << dpp::find_role(key)->get_mention() << "\n";
    }
    teamCount << "Registered Teams: " << teams.size();

    dpp::embed embed = UtilityEmbeds::embedTemplate()
            .set_title("__Teams__")
            .add_field(teamCount.str(), teamList.str());

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