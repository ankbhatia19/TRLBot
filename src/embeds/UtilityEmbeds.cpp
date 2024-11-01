//
// Created by ankit on 9/7/22.
//

#include "UtilityEmbeds.h"

dpp::embed UtilityEmbeds::base() {
    /* create the embed */
    dpp::embed embed = dpp::embed()
            .set_color(dpp::colors::wrx_blue)
            .set_footer(dpp::embed_footer().set_text(Utilities::getQuote()))
            .set_thumbnail("https://raw.githubusercontent.com/ankbhatia19/TRLBot/master/assets/TRL_logo_topright_noBG.png");

    return embed;
}

dpp::embed UtilityEmbeds::error(string msg) {
    dpp::embed embed = base()
            .set_title("Error")
            .add_field(msg, "Contact a dev for details");

    return embed;
}

dpp::embed UtilityEmbeds::loading() {
    return error("The user database is still loading. Please retry this command in a few minutes.");
}

dpp::embed UtilityEmbeds::test() {

    dpp::embed embed = base()
            .set_title("Coming soon!")
            .add_field(
                    "This command is not yet finished.",
                    "Stay tuned for updates."
            );

    return embed;
}

dpp::embed UtilityEmbeds::pong(string uptime) {
    dpp::embed embed = base()
            .set_title("Pong!")
            .add_field("Uptime", uptime);

    return embed;
}

dpp::embed UtilityEmbeds::help() {
    std::ostringstream body;

    body << "There are four groups of commands available for use:\n\n";
    body << "`/match`: Create, remove, and submit replays for a match.\n";
    body << "More info: " << dpp::utility::slashcommand_mention(Utilities::cmd_map["match"], "match", "help") << "\n\n";
    body << "`/team`: Manage and view registered teams as well as add or remove players from a team.\n";
    body << "More info: " << dpp::utility::slashcommand_mention(Utilities::cmd_map["team"], "team", "help") << "\n\n";
    body << "`/player`: Register Rocket League usernames and view stats of a player.\n";
    body << "More info: " << dpp::utility::slashcommand_mention(Utilities::cmd_map["player"], "player", "help") << "\n\n";
    body << "`/schedule`: View all scheduled matches, or the details of a particular match.\n";
    body << "More info: " << dpp::utility::slashcommand_mention(Utilities::cmd_map["schedule"], "schedule", "help") << "\n\n";
    body << "Tip: You can interact with the above mentioned slashcommands to automatically fill the command in for you!";

    dpp::embed embed = base()
            .set_title("Help Page")
            .add_field("Info", body.str(), false);

    return embed;
}

dpp::embed UtilityEmbeds::error_missing_perms(dpp::interaction interaction) {

    dpp::embed embed = base()
            .set_title("Error")
            .add_field(
                    "Insufficient Permissions",
                    interaction.get_issuing_user().get_mention() + " does not have the League Staff role.",
                    false
            );

    return embed;
}