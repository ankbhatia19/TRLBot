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

message TeamCommand::msg(const slashcommand_t& event) {
    dpp::interaction interaction = event.command;
    command_interaction cmd_data = interaction.get_command_interaction();
    auto subcommand = cmd_data.options[0];

    if (subcommand.name == "register"){

        /* Get the team role from the parameter */
        role role = interaction.get_resolved_role(
                subcommand.get_value<dpp::snowflake>(0)
        );
        if (Schedule::hasTeam(role.id))
            return { event.command.channel_id, Embeds::errorEmbed("Team [" + role.name + "] already exists.") };

        Team team(role);
        Schedule::teams.push_back(team);
        return { event.command.channel_id, Embeds::teamRegisteredEmbed(role) };
    }
    else  if (subcommand.name == "delist"){

        /* Get the team role from the parameter */
        role role = interaction.get_resolved_role(
                subcommand.get_value<dpp::snowflake>(0)
        );

        if (!Schedule::hasTeam(role.id))
            return { event.command.channel_id, Embeds::teamUnregisteredEmbed(role) };

        Schedule::teams.erase(Schedule::teams.begin() + Schedule::getTeam(role.id));
        return { event.command.channel_id, Embeds::teamDelistedEmbed(role) };
    }
    else if (subcommand.name == "add") {

        /* Get the team role from the parameter */
        role role = interaction.get_resolved_role(
                subcommand.get_value<dpp::snowflake>(0)
        );

        if (!Schedule::hasTeam(role.id))
            return { event.command.channel_id, Embeds::teamUnregisteredEmbed(role) };

        user profile = interaction.get_resolved_user(
                subcommand.get_value<dpp::snowflake>(1)
        );
        if (Schedule::teams[Schedule::getTeam(role.id)].hasPlayer(profile.id))
            return { event.command.channel_id, Embeds::teamPlayerAlreadyRegisteredEmbed(profile, role)};

        Player player(profile);
        Schedule::teams[Schedule::getTeam(role.id)].players.push_back(player);
        return { event.command.channel_id, Embeds::teamAddedPlayerEmbed(profile, role) };
    }
    else if (subcommand.name == "remove"){
        /* Get the team role from the parameter */
        role role = interaction.get_resolved_role(
                subcommand.get_value<dpp::snowflake>(0)
        );

        if (!Schedule::hasTeam(role.id))
            return { event.command.channel_id, Embeds::teamUnregisteredEmbed(role) };

        user profile = interaction.get_resolved_user(
                subcommand.get_value<dpp::snowflake>(1)
        );
        Team team = Schedule::teams[Schedule::getTeam(role.id)];
        if (!team.hasPlayer(profile.id))
            return { event.command.channel_id, Embeds::teamPlayerUnregisteredEmbed(profile, role) };

        team.players.erase(team.players.begin() + team.getPlayer(role.id));

        return { event.command.channel_id, Embeds::teamRemovedPlayerEmbed(profile, role)};
    }
    else if (subcommand.name == "view"){
        if (subcommand.options.empty()){
            return { event.command.channel_id, Embeds::teamViewAllEmbed(Schedule::teams) };
        }
        else {
            /* Get the team role from the parameter */
            role role = interaction.get_resolved_role(
                    subcommand.get_value<dpp::snowflake>(0)
            );

            if (!Schedule::hasTeam(role.id)) {
                return { event.command.channel_id, Embeds::teamUnregisteredEmbed(role) };
            }
            return { event.command.channel_id, Embeds::teamViewRoleEmbed(role) };
        }
    }

    return { event.command.channel_id, Embeds::testEmbed() };
}
