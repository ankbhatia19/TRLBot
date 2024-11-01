//
// Created by ankit on 8/28/22.
//

#include "PingCommand.h"

dpp::slashcommand PingCommand::cmd(dpp::snowflake botID) {
    dpp::slashcommand pingcmd("ping", "Pong!", botID);

    return pingcmd;
}

dpp::message PingCommand::msg(const dpp::slashcommand_t &event, dpp::cluster& bot) {
    return { event.command.channel_id, UtilityEmbeds::pong(bot.uptime().to_string()) };
}
