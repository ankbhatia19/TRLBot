//
// Created by ankit on 8/28/22.
//

#include "PlayerCommand.h"

dpp::slashcommand PlayerCommand::cmd(dpp::snowflake botID) {
    dpp::slashcommand playercmd("player", "Register, remove, and view stats of a player", botID);

    playercmd.add_option(
            /* Create a subcommand type option for "info". */
            dpp::command_option(dpp::co_sub_command, "info", "View the info of a player")
                    .add_option(dpp::command_option(dpp::co_user, "user", "The username to view", false))
    );
    playercmd.add_option(
            /* Create a subcommand type option for "register". */
            dpp::command_option(dpp::co_sub_command, "register", "Register a new Rocket League username")
                    .add_option(dpp::command_option(dpp::co_string, "name", "The username to add", true))
                    .add_option(dpp::command_option(dpp::co_user, "user", "The user to which the username should be added", false))
    );
    playercmd.add_option(
            /* Create a subcommand type option for "unregister". */
            dpp::command_option(dpp::co_sub_command, "unregister", "Unregister a Rocket League username")
                    .add_option(dpp::command_option(dpp::co_string, "name", "The username to remove", true))
                    .add_option(dpp::command_option(dpp::co_user, "user", "The user from which the username should be removed", false))
    );
    playercmd.add_option(
            /* Create another subcommand type option for "help". */
            dpp::command_option(dpp::co_sub_command, "help", "The help page for this command")
    );

    return playercmd;
}

dpp::message PlayerCommand::msg(const dpp::slashcommand_t &event, dpp::cluster& bot) {
    dpp::interaction interaction = event.command;
    dpp::command_interaction cmd_data = interaction.get_command_interaction();
    auto subcommand = cmd_data.options[0];

    if (subcommand.name == "help") {
        return { event.command.channel_id, PlayerEmbeds::help() };
    }
    else if (subcommand.name == "info"){

        SQLite::Database db("rocket_league.db", SQLite::OPEN_READWRITE);

        dpp::user profile;
        if (subcommand.options.empty())
            // view info of self
            profile = interaction.get_issuing_user();
        else
            // view info of tagged player
            profile = interaction.get_resolved_user(
                    subcommand.get_value<dpp::snowflake>(0)
            );

        if (!Player::has_id(db, profile.id))
            return { event.command.channel_id, PlayerEmbeds::error_not_found(profile) };

        return { event.command.channel_id, PlayerEmbeds::view(profile) };
    }
    else if (subcommand.name == "register"){
        string username = std::get<string>(subcommand.options[0].value);
        dpp::user profile;
        if (subcommand.options.size() == 2){
            // ensure that permission requirements are met
            if (!Utilities::checkPerms(interaction))
                return { event.command.channel_id, UtilityEmbeds::error_missing_perms(interaction) };

            profile = interaction.get_resolved_user(
                    subcommand.get_value<dpp::snowflake>(1)
            );
        }
        else {
            profile = interaction.get_issuing_user();
        }

        SQLite::Database db("rocket_league.db", SQLite::OPEN_READWRITE);

        if (Player::add_name(db, profile.id, username))
            return { event.command.channel_id, PlayerEmbeds::added_username(profile, username) };
        else
            return { event.command.channel_id,
                     PlayerEmbeds::error_duplicate_username(*dpp::find_user(Player::get_id(db, username)), username) };
    }
    else if (subcommand.name == "unregister"){

        return { event.command.channel_id, UtilityEmbeds::test() };
    }

    return { event.command.channel_id, UtilityEmbeds::test() };
}
