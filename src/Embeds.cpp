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

embed Embeds::matchCreatedEmbed(int matchID, string home, string away, vector<Player> homePlayers, vector<Player> awayPlayers){
    std::ostringstream matchIDstr;
    std::ostringstream homeTeamStr, homeTeamPlayers;
    std::ostringstream awayTeamStr, awayTeamPlayers;

    matchIDstr << "Match ID: " << matchID;
    homeTeamStr << "Home: " << home;
    awayTeamStr << "Away: " << away;

    if (homePlayers.empty()){ homeTeamPlayers << "None\n"; }
    if (awayPlayers.empty()){ awayTeamPlayers << "None\n"; }
    for (auto member : homePlayers){
        homeTeamPlayers << member.profile.username << "\n";
    }
    for (auto member : awayPlayers){
        awayTeamPlayers << member.profile.username << "\n";
    }

    embed embed = embedTemplate()
            .set_title(matchIDstr.str())
            .add_field(
                    homeTeamStr.str(),
                    homeTeamPlayers.str(),
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
    unregistered << "These team(s) have not been registered: ";
    if (!Schedule::hasTeam(home.id)){
        unregistered << "[" << home.name << "] ";
    }
    if (!Schedule::hasTeam(away.id)){
        unregistered << "[" << away.name << "] ";
    }
    embed embed = embedTemplate()
            .set_title("Error")
            .add_field(
                    unregistered.str(),
                    "Use `/team register` to create a team."
            );

    return embed;
}

embed Embeds::teamUnregisteredEmbed(role team) {
    std::ostringstream unregistered;
    unregistered << "These team(s) have not been registered: ";
    if (!Schedule::hasTeam(team.id)){
        unregistered << "[" << team.name << "] ";
    }
    embed embed = embedTemplate()
            .set_title("Error")
            .add_field(
                    unregistered.str(),
                    "Use `/team register` to create a team."
            );

    return embed;
}

embed Embeds::teamRegisteredEmbed(role team) {

    embed embed = embedTemplate()
            .set_title("Team Created")
            .add_field(
                    "Added team [" + team.name + "]",
                    "Use `/team add` to fill the team roster."
            );

    return embed;
}

embed Embeds::teamDelistedEmbed(role team) {

    embed embed = embedTemplate()
            .set_title("Delisted team [" + team.name + "]");

    return embed;
}

embed Embeds::teamAddedPlayerEmbed(user player, role team) {

    embed embed = embedTemplate()
            .set_title("Player Added")
            .add_field(
                    "Added player [" + player.username + "] to [" + team.name + "]",
                    "Use `/player info` to view this player."
            );

    return embed;
}


embed Embeds::teamPlayerAlreadyRegisteredEmbed(user player, role team) {

    embed embed = embedTemplate()
            .set_title("Error")
            .add_field(
                    "Player " + player.username + "is already registered to [" + team.name + "]" ,
                    "Use `/team view` to view this team."
            );

    return embed;
}

embed Embeds::teamPlayerUnregisteredEmbed(user player, role team) {

    embed embed = embedTemplate()
            .set_title("Error")
            .add_field(
                    "Player " + player.username + "is not registered to [" + team.name + "]" ,
                    "Use `/team view` to view this team."
            );

    return embed;
}

embed Embeds::teamRemovedPlayerEmbed(user player, role team) {

    embed embed = embedTemplate()
            .set_title("Player Removed")
            .add_field(
                    "Player " + player.username + "has been removed from [" + team.name + "]" ,
                    "Use `/team view` to view this team."
            );

    return embed;
}

embed Embeds::teamViewAllEmbed(vector<Team> teams) {

    std::ostringstream teamList;
    std::ostringstream playerList;
    int playerCount = 0;

    if (teams.empty())
        teamList << "No registered teams.";

    for (auto team : teams){
        teamList << team.team.get_mention();
        playerCount += team.players.size();
    }
    playerList << "Registered Players: " << playerCount;

    embed embed = embedTemplate()
            .set_title("__Registered Teams__")
            .add_field(playerList.str(), teamList.str());

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
        players << member.profile.username << "\n";
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
