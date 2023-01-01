//
// Created by ankit on 9/9/22.
//

#include "HelpCommand.h"

dpp::slashcommand HelpCommand::cmd(dpp::snowflake botID) {
    dpp::slashcommand helpcmd("help", "A description of all available functions.", botID);

    return helpcmd;
}

dpp::message HelpCommand::msg(const dpp::slashcommand_t &event, dpp::cluster& bot) {
    return { event.command.channel_id, UtilityEmbeds::helpEmbed() };
}