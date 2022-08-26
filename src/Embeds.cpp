//
// Created by ankit on 8/24/22.
//

#include "include/Embeds.h"
//
// Created by ankit on 8/24/22.
//


embed Embeds::matchEmbed(int matchID, string home, string away, members_container homePlayers, members_container awayPlayers){
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
        homeTeamPlayers << member.second.get_user()->username << "\n";
    }
    for (auto member : awayPlayers){
        awayTeamPlayers << member.second.get_user()->username << "\n";
    }

    cout << matchIDstr.str() << " " << homeTeamStr.str() << " " << homeTeamPlayers.str() << endl;

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