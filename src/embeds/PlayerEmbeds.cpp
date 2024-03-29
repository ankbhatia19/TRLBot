//
// Created by ankit on 9/7/22.
//

#include "PlayerEmbeds.h"

dpp::embed PlayerEmbeds::playerView(dpp::user profile) {

    Player player = RecordBook::players[profile.id];

    std::ostringstream stats;
    if (player.stats.empty()){
        stats << "Goals:   " << 0 << "\n";
        stats << "Saves:   " << 0 << "\n";
        stats << "Shots:   " << 0 << "\n";
        stats << "Assists: " << 0 << "\n";
        stats << "Games:   " << 0 << "\n";
        stats << "TRL MMR: " << 0 << "\n";
    }
    else {
        stats << "Goals:   " << player.getStatistic(Player::GOALS)         << "   [" << setprecision(3) << player.getStatisticAvg(Player::GOALS)      << "]" << "\n";
        stats << "Saves:   " << player.getStatistic(Player::SAVES)         << "   [" << setprecision(3) << player.getStatisticAvg(Player::SAVES)      << "]" << "\n";
        stats << "Shots:   " << player.getStatistic(Player::SHOTS)         << "   [" << setprecision(3) << player.getStatisticAvg(Player::SHOTS)      << "]" << "\n";
        stats << "Assists: " << player.getStatistic(Player::ASSISTS)       << "   [" << setprecision(3) << player.getStatisticAvg(Player::ASSISTS)    << "]" << "\n";
        stats << "Games:   " << (int)player.getStatistic(Player::NUM_GAMES)       << "\n";
        stats << "TRL MMR: " << (int)player.getStatisticAvg(Player::AVG_MVPR)     << "\n";
    }

    std::ostringstream usernames;
    if (player.aliases.empty())
        usernames << "None";
    for (auto name : player.aliases)
        usernames << name << "\n";

    std::ostringstream team;
    if (player.teamID == 0)
        team << "None";
    else
        team << dpp::find_role(player.teamID)->get_mention();

    dpp::embed embed = UtilityEmbeds::embedTemplate()
            .set_title("Player Card")
            .add_field(
                    "Player",
                    find_user(profile.id)->get_mention(),
                    true
            )
            .add_field(
                    "Team",
                    team.str(),
                    true
            )
            .add_field(
                    "__Season Stats__",
                    "```" + stats.str() + "```",
                    false
            )
            .add_field(
                    "__Registered Usernames__",
                    usernames.str(),
                    false
            );

    return embed;
}

dpp::embed PlayerEmbeds::playerNotFound(dpp::user profile) {
    dpp::embed embed = UtilityEmbeds::embedTemplate()
            .set_title("Error")
            .add_field("This player is not registered: ",
                       profile.get_mention() + "\n\nUse "
                       + dpp::utility::slashcommand_mention(Utilities::cmd_map["player"], "player", "register")
                       + " to create a new player account."
            );
    return embed;
}

dpp::embed PlayerEmbeds::playerUsernameExists(dpp::user profile, string name){
    dpp::embed embed = UtilityEmbeds::embedTemplate()
            .set_title("Error")
            .add_field("Duplicate username entered: ",
                       profile.get_mention() + " already has registered the username " + name
                       + "\n\nUse "
                       + dpp::utility::slashcommand_mention(Utilities::cmd_map["player"], "player", "info")
                       + " to view all registered usernames."
            );
    return embed;
}

dpp::embed PlayerEmbeds::playerUsernameDoesNotExist(dpp::user profile, string name){
    dpp::embed embed = UtilityEmbeds::embedTemplate()
            .set_title("Error")
            .add_field("Username not found: ",
                       profile.get_mention() + " does not have the username " + name
                       + "\n\nUse "
                       + dpp::utility::slashcommand_mention(Utilities::cmd_map["player"], "player", "info")
                       + " to view all registered usernames."
            );
    return embed;
}

dpp::embed PlayerEmbeds::playerAddedUsername(dpp::user user, string username) {
    dpp::embed embed = UtilityEmbeds::embedTemplate()
            .set_title("Added username")
            .add_field(
                    "Rocket League username registered:",
                    username + " to " + user.get_mention() + "\n\nUse "
                    + dpp::utility::slashcommand_mention(Utilities::cmd_map["player"], "player", "info")
                    + " to view all registered usernames.",
                    false
            );

    return embed;
}

dpp::embed PlayerEmbeds::playerRemovedUsername(dpp::user user, string username) {
    dpp::embed embed = UtilityEmbeds::embedTemplate()
            .set_title("Removed username")
            .add_field(
                    "Rocket League username unregistered:",
                    username + " from " + user.get_mention() + "\n\nUse "
                    + dpp::utility::slashcommand_mention(Utilities::cmd_map["player"], "player", "info")
                    + " to view all registered usernames.",
                    false
            );

    return embed;
}

dpp::embed PlayerEmbeds::playerHelpEmbed() {
    std::ostringstream body;
    body << dpp::utility::slashcommand_mention(Utilities::cmd_map["player"], "player", "register");
    body << " `[Username]`: Register a Rocket League username. This step is required for all players.\n\n";
    body << dpp::utility::slashcommand_mention(Utilities::cmd_map["player"], "player", "register");
    body << " `[Username]` `[Player]`\\*: Register a Rocket League username to the provided player.\n\n";
    body << dpp::utility::slashcommand_mention(Utilities::cmd_map["player"], "player", "info");
    body << ": View your player card with all tracked statistics and registered usernames.\n\n";
    body << dpp::utility::slashcommand_mention(Utilities::cmd_map["player"], "player", "info");
    body << "`[Player]`: View the player card of the provided player.\n\n";
    body << "Commands marked by an asterisk (\\*) are only usable by League Staff.";

    dpp::embed embed = UtilityEmbeds::embedTemplate()
            .set_title("Help Page: Player")
            .add_field("Info", body.str(), false);

    return embed;
}
