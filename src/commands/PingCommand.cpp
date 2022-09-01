//
// Created by ankit on 8/28/22.
//

#include "include/PingCommand.h"

slashcommand PingCommand::cmd(snowflake botID) {
    slashcommand pingcmd("ping", "Pong!", botID);

    return pingcmd;
}

message PingCommand::msg(const slashcommand_t &event, cluster& bot) {
    return { event.command.channel_id, Embeds::testEmbed() };
}
