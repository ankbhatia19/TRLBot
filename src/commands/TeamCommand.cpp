//
// Created by ankit on 8/28/22.
//

#include "TeamCommand.h"

dpp::slashcommand TeamCommand::cmd(dpp::snowflake botID) {
    dpp::slashcommand teamcmd("team", "Edit and view team information and members", botID);

    teamcmd.add_option(
            /* Create a subcommand type option for "register". */
            dpp::command_option(dpp::co_sub_command, "register", "Register a team")
                    .add_option(dpp::command_option(dpp::co_role, "team", "The team role", true))
    );
    teamcmd.add_option(
            /* Create a subcommand type option for "delist". */
            dpp::command_option(dpp::co_sub_command, "delist", "Delist a team")
                    .add_option(dpp::command_option(dpp::co_role, "team", "The team role", true))
    );
    teamcmd.add_option(
            /* Create a subcommand type option for "add". */
            dpp::command_option(dpp::co_sub_command, "add", "Add team members")
                    .add_option(dpp::command_option(dpp::co_role, "team", "The team role", true))
                    .add_option(dpp::command_option(dpp::co_user, "player1", "The player to add", true))
                    .add_option(dpp::command_option(dpp::co_user, "player2", "The player to add", false))
                    .add_option(dpp::command_option(dpp::co_user, "player3", "The player to add", false))
    );
    teamcmd.add_option(
            /* Create another subcommand type option for "remove". */
            dpp::command_option(dpp::co_sub_command, "remove", "Remove a player")
                    .add_option(dpp::command_option(dpp::co_role, "role", "The team role", true))
                    .add_option(dpp::command_option(dpp::co_user, "player", "The player to remove", true))
    );
    teamcmd.add_option(
            /* Create another subcommand type option for "view". */
            dpp::command_option(dpp::co_sub_command, "view", "View all teams or the indicated team")
                    .add_option(dpp::command_option(dpp::co_role, "role", "The team role", false))
    );
    teamcmd.add_option(
            /* Create another subcommand type option for "help". */
            dpp::command_option(dpp::co_sub_command, "help", "The help page for this command")
    );

    return teamcmd;
}

dpp::message TeamCommand::msg(const dpp::slashcommand_t &event, dpp::cluster& bot) {
    dpp::interaction interaction = event.command;
    dpp::command_interaction cmd_data = interaction.get_command_interaction();
    auto subcommand = cmd_data.options[0];

    if (subcommand.name == "help") {
        return { event.command.channel_id, TeamEmbeds::help() };
    }
    else if (subcommand.name == "register"){

        if (!Utilities::checkPerms(interaction))
            return { event.command.channel_id, UtilityEmbeds::error_missing_perms(interaction) };
        /* Get the team role from the parameter */
        dpp::role role = interaction.get_resolved_role(
                subcommand.get_value<dpp::snowflake>(0)
        );

        SQLite::Database db("rocket_league.db", SQLite::OPEN_READWRITE);

        Team::add_team(db, role.id);

        return { event.command.channel_id, TeamEmbeds::registered(role) };
    }
    else  if (subcommand.name == "delist"){

        if (!Utilities::checkPerms(interaction))
            return { event.command.channel_id, UtilityEmbeds::error_missing_perms(interaction) };

        /* Get the team role from the parameter */
        dpp::role role = interaction.get_resolved_role(
                subcommand.get_value<dpp::snowflake>(0)
        );

        return { event.command.channel_id, UtilityEmbeds::test() };
    }
    else if (subcommand.name == "add") {

        SQLite::Database db("rocket_league.db", SQLite::OPEN_READWRITE);

        if (!Utilities::checkPerms(interaction))
            return { event.command.channel_id, UtilityEmbeds::error_missing_perms(interaction) };

        /* Get the team role from the parameter */
        dpp::role role = interaction.get_resolved_role(
                subcommand.get_value<dpp::snowflake>(0)
        );

        vector<int64_t> added;
        for (int i = 1; i < subcommand.options.size(); i++){
            dpp::user profile = interaction.get_resolved_user(
                    subcommand.get_value<dpp::snowflake>(i)
            );

            Team::add_player(db, role.id, profile.id);
            Player::add_team(db, profile.id, role.id);
            added.emplace_back(profile.id);

        }
        return { event.command.channel_id, TeamEmbeds::added_players(added, role.id) };
    }
    else if (subcommand.name == "remove"){

        if (!Utilities::checkPerms(interaction))
            return { event.command.channel_id, UtilityEmbeds::error_missing_perms(interaction) };

        /* Get the team role from the parameter */
        dpp::role role = interaction.get_resolved_role(
                subcommand.get_value<dpp::snowflake>(0)
        );

        dpp::user profile = interaction.get_resolved_user(
                subcommand.get_value<dpp::snowflake>(1)
        );

        return { event.command.channel_id, UtilityEmbeds::test() };
    }
    else if (subcommand.name == "view"){
        return { event.command.channel_id, UtilityEmbeds::test() };
    }

    return { event.command.channel_id, UtilityEmbeds::test() };
}
