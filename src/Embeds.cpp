//
// Created by ankit on 8/24/22.
//

#include "include/Embeds.h"

embed Embeds::embedTemplate() {
    /* create the embed */
    embed embed = dpp::embed()
            .set_color(dpp::colors::wrx_blue)
            .set_footer(dpp::embed_footer().set_text("TRLBot • Created by Waycey"))
            .set_timestamp(time(0));

    return embed;
}

embed Embeds::matchCreatedEmbed(Match match, role home, role away){
    std::ostringstream matchIDstr;
    std::ostringstream homeTeamStr, homeTeamPlayers;
    std::ostringstream awayTeamStr, awayTeamPlayers;

    matchIDstr << "Match ID: " << match.id;
    homeTeamStr << "Home: " << home.name;
    awayTeamStr << "Away: " << away.name;

    if (Schedule::teams[Schedule::getTeam(home.id)].players.empty())
        homeTeamPlayers << "None\n";

    if (Schedule::teams[Schedule::getTeam(away.id)].players.empty())
        awayTeamPlayers << "None\n";

    for (auto member : Schedule::teams[Schedule::getTeam(home.id)].players)
        homeTeamPlayers << member.profile.get_mention() << "\n";

    for (auto member : Schedule::teams[Schedule::getTeam(away.id)].players)
        awayTeamPlayers << member.profile.get_mention() << "\n";

    embed embed = embedTemplate()
            .set_title(matchIDstr.str())
            .add_field(
                    homeTeamStr.str(),
                    homeTeamPlayers.str(),
                    true
            )
            .add_field(
                    "_          _",
                    "_          _",
                    true
            )
            .add_field(
                    awayTeamStr.str(),
                    awayTeamPlayers.str(),
                    true
            );

    return embed;
}

embed Embeds::errorEmbed(string msg) {
    embed embed = embedTemplate()
            .set_title("Error")
            .add_field(msg, "Contact a dev for details");

    return embed;
}

embed Embeds::teamUnregisteredEmbed(role home, role away) {
    std::ostringstream unregistered;

    if (!Schedule::hasTeam(home.id)){
        unregistered << home.get_mention() << "\n";
    }
    if (!Schedule::hasTeam(away.id)){
        unregistered << away.get_mention() << " \n";
    }

    embed embed = embedTemplate()
            .set_title("Error")
            .add_field(
                    "These team(s) have not been registered: ",
                    unregistered.str() + "Use `/team register` to create a team."
            );

    return embed;
}

embed Embeds::teamUnregisteredEmbed(role team) {
    std::ostringstream unregistered;
    unregistered << "";
    if (!Schedule::hasTeam(team.id)){
        unregistered << team.get_mention();
    }
    unregistered << "\n";
    embed embed = embedTemplate()
            .set_title("Error")
            .add_field(
                    "These team(s) have not been registered: ",
                    unregistered.str() + "Use `/team register` to create a team."
            );

    return embed;
}

embed Embeds::teamRegisteredEmbed(role team) {

    embed embed = embedTemplate()
            .set_title("Team Created")
            .add_field(
                    "Registered team: ",
                    team.get_mention() + "\nUse `/team add` to fill the team roster."
            );

    return embed;
}

embed Embeds::teamDelistedEmbed(role team) {

    embed embed = embedTemplate()
            .set_title("Team Removed")
            .add_field(
                    "Delisted team: ",
                    team.get_mention() + "\nUse `/team register` to re-register this team."
            );

    return embed;
}

embed Embeds::teamAddedPlayerEmbed(user player, role team) {

    embed embed = embedTemplate()
            .set_title("Player Added")
            .add_field(
                    "Registered player: ",
                    player.get_mention() + " to " + team.get_mention() + "\nUse `/player info` to view this player."
            );

    return embed;
}

embed Embeds::teamRemovedPlayerEmbed(user player, role team) {

    embed embed = embedTemplate()
            .set_title("Player Removed")
            .add_field(
                    "Unregistered player: ",
                    player.get_mention() + " from " + team.get_mention() + "\nUse `/player info` to view this player."
            );

    return embed;
}

embed Embeds::teamPlayerAlreadyRegisteredEmbed(user player, role team) {

    embed embed = embedTemplate()
            .set_title("Error")
            .add_field(
                    "Player already registered: " ,
                    player.get_mention() + "is a member of " + team.get_mention() + "\nUse `/team view` to view this team."
            );

    return embed;
}

embed Embeds::teamPlayerUnregisteredEmbed(user player, role team) {

    embed embed = embedTemplate()
            .set_title("Error")
            .add_field(
                    "Player not registered: " ,
                    player.get_mention() + "is not a member of " + team.get_mention() + "\nUse `/team view` to view this team."
            );

    return embed;
}

embed Embeds::teamViewAllEmbed(vector<Team> teams) {

    std::ostringstream teamList;
    std::ostringstream teamCount;

    if (teams.empty())
        teamList << "No registered teams.";

    for (auto team : teams){
        teamList << team.team.get_mention() << "\n";
    }
    teamCount << "Registered Teams: " << teams.size();

    embed embed = embedTemplate()
            .set_title("__Teams__")
            .add_field(teamCount.str(), teamList.str());

    return embed;
}

embed Embeds::teamViewRoleEmbed(role team) {
    std::ostringstream players;
    std::ostringstream stats;

    Team thisTeam = Schedule::teams[Schedule::getTeam(team.id)];

    if (thisTeam.players.empty()){
        players << "None\n";
    }
    for (auto member : thisTeam.players){
        players << member.profile.get_mention() << "\n";
    }

    stats << "Wins: " << thisTeam.wins << "\n";
    stats << "Losses: " << thisTeam.losses << "\n";
    if (thisTeam.differential > 0)
        stats << "Differential: +" << thisTeam.differential << "\n";
    else
        stats << "Differential: " << thisTeam.differential << "\n";

    embed embed = embedTemplate()
            .set_title(team.name)
            .add_field(
                    "__Roster__\n",
                    players.str(),
                    true
            )
            .add_field(
                    "_          _",
                    "_          _",
                    true
            )
            .add_field(
                    "__Stats__\n",
                    stats.str(),
                    true
            );

    return embed;
}

embed Embeds::testEmbed() {

    embed embed = embedTemplate()
            .set_title("Coming soon!")
            .add_field(
                    "This command is not yet finished.",
                    "Stay tuned for updates."
            );

    return embed;
}
