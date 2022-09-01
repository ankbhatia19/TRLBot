//
// Created by ankit on 8/28/22.
//

#include "include/TeamCommand.h"

slashcommand TeamCommand::cmd(snowflake botID) {
    slashcommand teamcmd("team", "Edit and view team information and members", botID);

    teamcmd.add_option(
            /* Create a subcommand type option for "register". */
            command_option(dpp::co_sub_command, "register", "Register a team")
                    .add_option(command_option(dpp::co_role, "team", "The team role", true))
    );
    teamcmd.add_option(
            /* Create a subcommand type option for "delist". */
            command_option(dpp::co_sub_command, "delist", "Delist a team")
                    .add_option(command_option(dpp::co_role, "team", "The team role", true))
    );
    teamcmd.add_option(
            /* Create a subcommand type option for "add". */
            command_option(dpp::co_sub_command, "add", "Add team members")
                    .add_option(command_option(dpp::co_role, "team", "The team role", true))
                    .add_option(command_option(dpp::co_user, "player1", "The player to add", true))
                    .add_option(command_option(dpp::co_user, "player2", "The player to add", false))
                    .add_option(command_option(dpp::co_user, "player3", "The player to add", false))
    );
    teamcmd.add_option(
            /* Create another subcommand type option for "remove". */
            command_option(dpp::co_sub_command, "remove", "Remove a player")
                    .add_option(command_option(dpp::co_role, "role", "The team role", true))
                    .add_option(command_option(dpp::co_user, "player", "The player to remove", true))
    );
    teamcmd.add_option(
            /* Create another subcommand type option for "view". */
            command_option(dpp::co_sub_command, "view", "View all teams or the indicated team")
                    .add_option(command_option(dpp::co_role, "role", "The team role", false))
    );

    return teamcmd;
}

message TeamCommand::msg(const slashcommand_t &event, cluster& bot) {
    interaction interaction = event.command;
    command_interaction cmd_data = interaction.get_command_interaction();
    auto subcommand = cmd_data.options[0];

    if (subcommand.name == "register"){

        /* Get the team role from the parameter */
        role role = interaction.get_resolved_role(
                subcommand.get_value<dpp::snowflake>(0)
        );
        if (RecordBook::hasTeam(role.id))
            return { event.command.channel_id, Embeds::errorEmbed("Team [" + role.name + "] already exists.") };

        Team team(role);
        RecordBook::teams.push_back(team);
        return { event.command.channel_id, Embeds::teamRegisteredEmbed(role) };
    }
    else  if (subcommand.name == "delist"){

        /* Get the team role from the parameter */
        role role = interaction.get_resolved_role(
                subcommand.get_value<dpp::snowflake>(0)
        );

        if (!RecordBook::hasTeam(role.id))
            return { event.command.channel_id, Embeds::teamUnregisteredEmbed(role) };

        RecordBook::teams.erase(RecordBook::teams.begin() + RecordBook::getTeam(role.id));
        return { event.command.channel_id, Embeds::teamDelistedEmbed(role) };
    }
    else if (subcommand.name == "add") {

        /* Get the team role from the parameter */
        role role = interaction.get_resolved_role(
                subcommand.get_value<dpp::snowflake>(0)
        );

        if (!RecordBook::hasTeam(role.id))
            return { event.command.channel_id, Embeds::teamUnregisteredEmbed(role) };

        user profile1, profile2, profile3;

        profile1 = interaction.get_resolved_user(
                subcommand.get_value<dpp::snowflake>(1)
        );
        if (!RecordBook::hasPlayer(profile1.id))
            return { event.command.channel_id, Embeds::playerNotFound(profile1) };

        if (RecordBook::teams[RecordBook::getTeam(role.id)].hasPlayer(profile1.id))
            return { event.command.channel_id, Embeds::teamPlayerAlreadyRegisteredEmbed(profile1, role)};

        RecordBook::teams[RecordBook::getTeam(role.id)].players.push_back(
                RecordBook::players[RecordBook::getPlayer(profile1.id)]
        );
        RecordBook::players[RecordBook::getPlayer(profile1.id)].team = &RecordBook::teams[RecordBook::getTeam(role.id)];

        if (subcommand.options.size() > 2){
            // Two users specified
            profile2 = interaction.get_resolved_user(
                    subcommand.get_value<dpp::snowflake>(2)
            );
            if (!RecordBook::hasPlayer(profile2.id))
                return { event.command.channel_id, Embeds::playerNotFound(profile2) };

            if (RecordBook::teams[RecordBook::getTeam(role.id)].hasPlayer(profile2.id))
                return { event.command.channel_id, Embeds::teamPlayerAlreadyRegisteredEmbed(profile2, role)};

            RecordBook::teams[RecordBook::getTeam(role.id)].players.push_back(
                    RecordBook::players[RecordBook::getPlayer(profile2.id)]
            );
            RecordBook::players[RecordBook::getPlayer(profile2.id)].team = &RecordBook::teams[RecordBook::getTeam(role.id)];
        }
        if (subcommand.options.size() > 3){
            // Three users specified
            profile3 = interaction.get_resolved_user(
                    subcommand.get_value<dpp::snowflake>(3)
            );
            if (!RecordBook::hasPlayer(profile3.id))
                return { event.command.channel_id, Embeds::playerNotFound(profile3) };

            if (RecordBook::teams[RecordBook::getTeam(role.id)].hasPlayer(profile3.id))
                return { event.command.channel_id, Embeds::teamPlayerAlreadyRegisteredEmbed(profile3, role)};

            RecordBook::teams[RecordBook::getTeam(role.id)].players.push_back(
                    RecordBook::players[RecordBook::getPlayer(profile3.id)]
            );
            RecordBook::players[RecordBook::getPlayer(profile3.id)].team = &RecordBook::teams[RecordBook::getTeam(role.id)];
        }

        return { event.command.channel_id, Embeds::teamAddedPlayersEmbed(profile1, profile2, profile3, role) };
    }
    else if (subcommand.name == "remove"){
        /* Get the team role from the parameter */
        role role = interaction.get_resolved_role(
                subcommand.get_value<dpp::snowflake>(0)
        );

        if (!RecordBook::hasTeam(role.id))
            return { event.command.channel_id, Embeds::teamUnregisteredEmbed(role) };

        user profile = interaction.get_resolved_user(
                subcommand.get_value<dpp::snowflake>(1)
        );

        Team team = RecordBook::teams[RecordBook::getTeam(role.id)];
        if (!team.hasPlayer(profile.id))
            return { event.command.channel_id, Embeds::teamPlayerUnregisteredEmbed(profile, role) };

        team.players.erase(team.players.begin() + team.getPlayer(profile.id));

        return { event.command.channel_id, Embeds::teamRemovedPlayerEmbed(profile, role)};
    }
    else if (subcommand.name == "view"){
        if (subcommand.options.empty()){
            return { event.command.channel_id, Embeds::teamViewAllEmbed(RecordBook::teams) };
        }
        else {
            /* Get the team role from the parameter */
            role role = interaction.get_resolved_role(
                    subcommand.get_value<dpp::snowflake>(0)
            );

            if (!RecordBook::hasTeam(role.id)) {
                return { event.command.channel_id, Embeds::teamUnregisteredEmbed(role) };
            }
            return { event.command.channel_id, Embeds::teamViewRoleEmbed(role) };
        }
    }

    return { event.command.channel_id, Embeds::testEmbed() };
}
