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

        if (!Utilities::checkPerms(interaction, bot))
            return { event.command.channel_id, Embeds::insufficientPermsEmbed(interaction) };
        /* Get the team role from the parameter */
        role role = interaction.get_resolved_role(
                subcommand.get_value<dpp::snowflake>(0)
        );
        if (RecordBook::teams.contains(role.id))
            return { event.command.channel_id, Embeds::errorEmbed("Team [" + role.name + "] already exists.") };

        RecordBook::teams.insert({role.id, {role.id}});
        return { event.command.channel_id, Embeds::teamRegisteredEmbed(role) };
    }
    else  if (subcommand.name == "delist"){

        if (!Utilities::checkPerms(interaction, bot))
            return { event.command.channel_id, Embeds::insufficientPermsEmbed(interaction) };

        /* Get the team role from the parameter */
        role role = interaction.get_resolved_role(
                subcommand.get_value<dpp::snowflake>(0)
        );

        if (!RecordBook::teams.contains(role.id))
            return { event.command.channel_id, Embeds::teamUnregisteredEmbed(role) };

        RecordBook::teams.erase(role.id);
        return { event.command.channel_id, Embeds::teamDelistedEmbed(role) };
    }
    else if (subcommand.name == "add") {

        if (!Utilities::checkPerms(interaction, bot))
            return { event.command.channel_id, Embeds::insufficientPermsEmbed(interaction) };

        /* Get the team role from the parameter */
        role role = interaction.get_resolved_role(
                subcommand.get_value<dpp::snowflake>(0)
        );

        if (!RecordBook::teams.contains(role.id))
            return { event.command.channel_id, Embeds::teamUnregisteredEmbed(role) };

        vector<user> addedPlayers;
        for (int i = 1; i < subcommand.options.size(); i++){
            user profile = interaction.get_resolved_user(
                    subcommand.get_value<dpp::snowflake>(i)
            );
            if (!RecordBook::players.contains(profile.id))
                return { event.command.channel_id, Embeds::playerNotFound(profile) };

            if (RecordBook::teams[role.id].members.contains(profile.id))
                return { event.command.channel_id, Embeds::teamPlayerAlreadyRegisteredEmbed(profile, role)};

            RecordBook::teams[role.id].members.insert(
                    {profile.id, RecordBook::players[profile.id]}
            );
            RecordBook::players[profile.id].teamID = role.id;
            addedPlayers.push_back(profile);
        }
        return { event.command.channel_id, Embeds::teamAddedPlayersEmbed(addedPlayers, role) };
    }
    else if (subcommand.name == "remove"){

        if (!Utilities::checkPerms(interaction, bot))
            return { event.command.channel_id, Embeds::insufficientPermsEmbed(interaction) };

        /* Get the team role from the parameter */
        role role = interaction.get_resolved_role(
                subcommand.get_value<dpp::snowflake>(0)
        );

        if (!RecordBook::teams.contains(role.id))
            return { event.command.channel_id, Embeds::teamUnregisteredEmbed(role) };

        user profile = interaction.get_resolved_user(
                subcommand.get_value<dpp::snowflake>(1)
        );

        if (!RecordBook::teams[role.id].members.contains(profile.id))
            return { event.command.channel_id, Embeds::teamPlayerUnregisteredEmbed(profile, role) };

        RecordBook::teams[role.id].members.erase(profile.id);

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

            if (!RecordBook::teams.contains(role.id)) {
                return { event.command.channel_id, Embeds::teamUnregisteredEmbed(role) };
            }
            return { event.command.channel_id, Embeds::teamViewRoleEmbed(role, interaction.guild_id)};
        }
    }

    return { event.command.channel_id, Embeds::testEmbed() };
}
