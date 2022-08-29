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
    interaction interaction = event.command;
    command_interaction cmd_data = interaction.get_command_interaction();
    auto subcommand = cmd_data.options[0];

    if (subcommand.name == "info"){
        user profile;
        if (subcommand.options.empty()){
            // view info of self
            profile = interaction.get_issuing_user();
        }
        else {
            // view info of tagged player
            profile = interaction.get_resolved_user(
                    subcommand.get_value<dpp::snowflake>(0)
            );
        }
        if (!RecordBook::hasPlayer(profile.id))
            return { event.command.channel_id, Embeds::playerNotFound(profile) };

        return { event.command.channel_id, Embeds::playerView(profile) };
    }
    else if (subcommand.name == "register"){
        string username = std::get<string>(subcommand.options[0].value);
        user profile = interaction.get_issuing_user();
        Player* player;

        if (!RecordBook::hasPlayer(profile.id))
            player = new Player(profile);
        else
            player = &RecordBook::players[RecordBook::getPlayer(profile.id)];

        player->aliases.push_back(username);
        RecordBook::players.push_back(*player);

        return { event.command.channel_id, Embeds::playerAddedUsername(interaction.get_issuing_user(), username) };
    }

    return { event.command.channel_id, Embeds::testEmbed() };
}
