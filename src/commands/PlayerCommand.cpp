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
                    .add_option(command_option(dpp::co_user, "user", "The user to which the username should be added", false))
    );

    return playercmd;
}

message PlayerCommand::msg(const slashcommand_t &event, cluster& bot) {
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
        if (!RecordBook::players.contains(profile.id))
            return { event.command.channel_id, Embeds::playerNotFound(profile) };

        return { event.command.channel_id, Embeds::playerView(profile) };
    }
    else if (subcommand.name == "register"){
        string username = std::get<string>(subcommand.options[0].value);
        user profile;
        if (subcommand.options.size() == 2){
            // ensure that permission requirements are met
            if (!Utilities::checkPerms(interaction))
                return { event.command.channel_id, Embeds::insufficientPermsEmbed(interaction) };

            profile = interaction.get_resolved_user(
                    subcommand.get_value<dpp::snowflake>(1)
            );
        }
        else {
            profile = interaction.get_issuing_user();
        }

        if (!RecordBook::players.contains(profile.id)){
            RecordBook::players.insert({profile.id, {profile.id}});
            std::ostringstream log_info;
            log_info << "Player created: " << profile.id;
            bot.log(dpp::loglevel::ll_info, log_info.str());
        }

        if (RecordBook::players[profile.id].containsAlias(username))
            return { event.command.channel_id, Embeds::playerUsernameExists(profile, username) };

        RecordBook::players[profile.id].aliases.push_back(username);

        return { event.command.channel_id, Embeds::playerAddedUsername(profile, username) };
    }

    return { event.command.channel_id, Embeds::testEmbed() };
}
