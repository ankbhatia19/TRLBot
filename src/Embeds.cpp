//
// Created by ankit on 8/24/22.
//

#include "include/Embeds.h"

embed Embeds::matchEmbed(int matchID, string home, string away, vector<Player> homePlayers, vector<Player> awayPlayers){
    std::ostringstream matchIDstr;
    std::ostringstream homeTeamStr, homeTeamPlayers;
    std::ostringstream awayTeamStr, awayTeamPlayers;

    matchIDstr << "Match ID: " << matchID;
    homeTeamStr << "Home: " << home;
    awayTeamStr << "Away: " << away;

    if (homePlayers.empty()){
        homeTeamPlayers << "None\n";
    }
    if (awayPlayers.empty()){
        awayTeamPlayers << "None\n";
    }
    for (auto member : homePlayers){
        homeTeamPlayers << member.profile.username << "\n";
    }
    for (auto member : awayPlayers){
        awayTeamPlayers << member.profile.username << "\n";
    }


    dpp::embed embed = dpp::embed().
            set_color(dpp::colors::sti_blue).
            set_title(matchIDstr.str()).
            add_field(
            homeTeamStr.str(),
            homeTeamPlayers.str(),
            true
    ).
            add_field(
            awayTeamStr.str(),
            awayTeamPlayers.str(),
            true
    ).
            set_timestamp(time(NULL));

    return embed;
}

embed Embeds::errorEmbed(string msg) {
    std::ostringstream error;

    error << "Error: " << msg << endl;
    embed embed = dpp::embed().
            set_color(dpp::colors::sti_blue).
            set_title("Error").
            add_field(
            error.str(),
            "Contact a dev for details").
            set_timestamp(time(0));

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
    embed embed = dpp::embed().
            set_color(dpp::colors::sti_blue).
            set_title("Error").
            add_field(
            unregistered.str(),
            "Use `/team register` to create a team.").
            set_timestamp(time(0));

    return embed;
}

embed Embeds::teamUnregisteredEmbed(role team) {
    std::ostringstream unregistered;
    unregistered << "These team(s) have not been registered: ";
    if (!Schedule::hasTeam(team.id)){
        unregistered << "[" << team.name << "] ";
    }
    embed embed = dpp::embed().
            set_color(dpp::colors::sti_blue).
            set_title("Error").
            add_field(
            unregistered.str(),
            "Use `/team register` to create a team.").
            set_timestamp(time(0));

    return embed;
}

embed Embeds::teamRegisteredEmbed(role team) {
    /* create the embed */
    embed embed = dpp::embed().
            set_color(dpp::colors::sti_blue).
            set_title("Team Created").
            add_field(
            "Added team [" + team.name + "]",
            "Use `/team add` to fill the team roster.").
            set_timestamp(time(0));

    return embed;
}

embed Embeds::teamDelistedEmbed(role team) {
    /* create the embed */
    embed embed = dpp::embed().
            set_color(dpp::colors::sti_blue).
            set_title("Delisted team [" + team.name + "]").
            set_timestamp(time(0));

    return embed;
}

embed Embeds::teamAddedPlayerEmbed(user player, role team) {
    embed embed = dpp::embed().
            set_color(dpp::colors::sti_blue).
            set_title("Player Added").
            add_field(
            "Added player [" + player.username + "] to [" + team.name + "]",
            "Use `/player info` to view this player.").
            set_timestamp(time(0));

    return embed;
}

embed Embeds::testEmbed() {
    /* create the embed */
    embed embed = dpp::embed().
            set_color(dpp::colors::sti_blue).
            set_title("Coming soon!").
            add_field(
            "This command is not yet finished.",
            "Stay tuned for updates.").
            set_timestamp(time(0));

    return embed;
}