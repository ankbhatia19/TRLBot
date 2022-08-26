//
// Created by ankit on 8/24/22.
//

#include "include/Commands.h"

slashcommand Commands::pingCommand(snowflake botID) {
    slashcommand pingcmd("ping", "Pong!", botID);

    return pingcmd;
}

slashcommand Commands::matchCommand(snowflake botID){
    slashcommand matchcmd("match", "Add, remove, or submit a match", botID);
    matchcmd.add_option(
            /* Create a subcommand type option for "add". */
            dpp::command_option(dpp::co_sub_command, "add", "Add a match")
                    .add_option(dpp::command_option(dpp::co_role, "home", "The role of the home team", false))
                    .add_option(dpp::command_option(dpp::co_role, "away", "The role of the away team", false))
    );
    matchcmd.add_option(
            /* Create another subcommand type option for "remove". */
            dpp::command_option(dpp::co_sub_command, "remove", "Remove a match")
                    .add_option(dpp::command_option(dpp::co_number, "id", "The match ID to be removed", false))
    );

    return matchcmd;
}

slashcommand Commands::teamCommand(snowflake botID) {
    slashcommand teamcmd("team", "Edit and view team information and members", botID);

    teamcmd.add_option(
            /* Create a subcommand type option for "register". */
            command_option(dpp::co_sub_command, "register", "Register a team")
                            .add_option(command_option(dpp::co_role, "team", "The team role", false))
    );
    teamcmd.add_option(
            /* Create a subcommand type option for "delist". */
            command_option(dpp::co_sub_command, "delist", "Delist a team")
                    .add_option(command_option(dpp::co_role, "team", "The team role", false))
    );
    teamcmd.add_option(
            /* Create a subcommand type option for "add". */
            command_option(dpp::co_sub_command, "add", "Add team members")
                    .add_option(command_option(dpp::co_user, "player1", "The player to add", false))
                    .add_option(command_option(dpp::co_user, "player2", "The player to add", false))
                    .add_option(command_option(dpp::co_user, "player3", "The player to add", false))
    );
    teamcmd.add_option(
            /* Create another subcommand type option for "remove". */
            command_option(dpp::co_sub_command, "remove", "Remove a player").
                    add_option(command_option(dpp::co_user, "player", "The player to remove", false))
    );

    return teamcmd;
}

slashcommand Commands::scheduleCommand(snowflake botID) {
    slashcommand schedcmd("schedule", "View and edit the schedule", botID);

    schedcmd.add_option(
            /* Create a subcommand type option for "view". */
            command_option(dpp::co_sub_command, "view", "View all scheduled matches")
    );
    schedcmd.add_option(
            /* Create another subcommand type option for "edit". */
            command_option(dpp::co_sub_command, "edit", "Edit a match")
                    .add_option(command_option(dpp::co_number, "id", "The match to edit", false))
                    .add_option(command_option(dpp::co_string, "date", "The date to schedule this match", false))
                    .add_option(command_option(dpp::co_string, "time", "The time to schedule this match", false))
    );

    return schedcmd;
}

slashcommand Commands::playerCommand(snowflake botID) {
    slashcommand playercmd("player", "Register, remove, and view stats of a player", botID);

    playercmd.add_option(
            /* Create a subcommand type option for "info". */
            command_option(dpp::co_sub_command, "info", "View the info of a player")
                    .add_option(command_option(dpp::co_user, "user", "The username to view", false))
    );
    playercmd.add_option(
            /* Create a subcommand type option for "info". */
            command_option(dpp::co_sub_command, "register", "Register a new Rocket League username")
                    .add_option(command_option(dpp::co_string, "name", "The username to add", false))
    );

    return playercmd;
}


