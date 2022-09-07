//
// Created by ankit on 9/7/22.
//

#include "include/UtilityEmbeds.h"

dpp::embed UtilityEmbeds::embedTemplate() {
    /* create the embed */
    dpp::embed embed = dpp::embed()
            .set_color(dpp::colors::wrx_blue)
            .set_footer(dpp::embed_footer().set_text("TRLBot • Created by Waycey"))
            .set_thumbnail("https://raw.githubusercontent.com/ankbhatia19/TRLBot/master/assets/TRL_logo_topright_noBG.png")
            .set_timestamp(time(0));

    return embed;
}

dpp::embed UtilityEmbeds::errorEmbed(string msg) {
    dpp::embed embed = embedTemplate()
            .set_title("Error")
            .add_field(msg, "Contact a dev for details");

    return embed;
}

dpp::embed UtilityEmbeds::loadingEmbed() {
    return errorEmbed("The user database is still loading. Please retry this command in a few minutes.");
}

dpp::embed UtilityEmbeds::testEmbed() {

    dpp::embed embed = embedTemplate()
            .set_title("Coming soon!")
            .add_field(
                    "This command is not yet finished.",
                    "Stay tuned for updates."
            );

    return embed;
}

dpp::embed UtilityEmbeds::pingEmbed(std::string uptime) {
    dpp::embed embed = embedTemplate()
            .set_title("Pong!")
            .add_field("Uptime", uptime);

    return embed;
}

dpp::embed UtilityEmbeds::insufficientPermsEmbed(dpp::interaction interaction) {

    dpp::embed embed = embedTemplate()
            .set_title("Error")
            .add_field(
                    "Insufficient Permissions",
                    interaction.get_issuing_user().get_mention() + " does not have the League Staff role.",
                    false
            );

    return embed;
}