//
// Created by ankit on 8/28/22.
//

#include "include/PlayerCommand.h"

slashcommand PlayerCommand::cmd(snowflake botID) {
    slashcommand playercmd("player", "Register, remove, and view stats of a player", botID);

    playercmd.add_option(
            /* Create a subcommand type option for "info". */
            command_option(dpp::co_sub_command, "info", "View the info of a player")
                    .add_option(command_option(dpp::co_user, "user", "The username to view", false))
    );
    playercmd.add_option(
            /* Create a subcommand type option for "register". */
            command_option(dpp::co_sub_command, "register", "Register a new Rocket League username")
                    .add_option(command_option(dpp::co_string, "name", "The username to add", true))
    );

    return playercmd;
}

message PlayerCommand::msg(const slashcommand_t &event) {
    return {event.command.channel_id, Embeds::testEmbed() };
}
