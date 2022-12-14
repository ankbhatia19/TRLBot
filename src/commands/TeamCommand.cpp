//
// Created by ankit on 8/28/22.
//

#include "include/TeamCommand.h"

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
        return { event.command.channel_id, TeamEmbeds::teamHelpEmbed() };
    }
    else if (subcommand.name == "register"){

        if (!Utilities::checkPerms(interaction))
            return { event.command.channel_id, UtilityEmbeds::insufficientPermsEmbed(interaction) };
        /* Get the team role from the parameter */
        dpp::role role = interaction.get_resolved_role(
                subcommand.get_value<dpp::snowflake>(0)
        );
        if (RecordBook::teams.contains(role.id))
            return { event.command.channel_id, UtilityEmbeds::errorEmbed("Team [" + role.name + "] already exists.") };

        RecordBook::teams.insert({role.id, {(unsigned long long)role.id}});
        RecordBook::save_team(role.id);
        return { event.command.channel_id, TeamEmbeds::teamRegisteredEmbed(role) };
    }
    else  if (subcommand.name == "delist"){

        if (!Utilities::checkPerms(interaction))
            return { event.command.channel_id, UtilityEmbeds::insufficientPermsEmbed(interaction) };

        /* Get the team role from the parameter */
        dpp::role role = interaction.get_resolved_role(
                subcommand.get_value<dpp::snowflake>(0)
        );

        if (!RecordBook::teams.contains(role.id))
            return { event.command.channel_id, TeamEmbeds::teamUnregisteredEmbed(role) };

        RecordBook::teams.erase(role.id);
        std::ostringstream log_info;
        log_info << "Removed team: " << role.id;
        bot.log(dpp::loglevel::ll_info, log_info.str());
        return { event.command.channel_id, TeamEmbeds::teamDelistedEmbed(role) };
    }
    else if (subcommand.name == "add") {

        if (!Utilities::checkPerms(interaction))
            return { event.command.channel_id, UtilityEmbeds::insufficientPermsEmbed(interaction) };

        /* Get the team role from the parameter */
        dpp::role role = interaction.get_resolved_role(
                subcommand.get_value<dpp::snowflake>(0)
        );

        if (!RecordBook::teams.contains(role.id))
            return { event.command.channel_id, TeamEmbeds::teamUnregisteredEmbed(role) };

        vector<dpp::user> addedPlayers;
        for (int i = 1; i < subcommand.options.size(); i++){
            dpp::user profile = interaction.get_resolved_user(
                    subcommand.get_value<dpp::snowflake>(i)
            );
            if (!RecordBook::players.contains(profile.id))
                return { event.command.channel_id, PlayerEmbeds::playerNotFound(profile) };

            if (RecordBook::teams[role.id].members.contains(profile.id))
                return { event.command.channel_id, TeamEmbeds::teamPlayerAlreadyRegisteredEmbed(profile, role)};

            RecordBook::teams[role.id].members.insert(
                    {profile.id, RecordBook::players[profile.id]}
            );

            if (RecordBook::players[profile.id].teamID != 0)
                RecordBook::teams[RecordBook::players[profile.id].teamID].members.erase(profile.id);

            RecordBook::players[profile.id].teamID = role.id;
            addedPlayers.push_back(profile);
            RecordBook::save_player(profile.id);
        }
        RecordBook::save_team(role.id);
        return { event.command.channel_id, TeamEmbeds::teamAddedPlayersEmbed(addedPlayers, role) };
    }
    else if (subcommand.name == "remove"){

        if (!Utilities::checkPerms(interaction))
            return { event.command.channel_id, UtilityEmbeds::insufficientPermsEmbed(interaction) };

        /* Get the team role from the parameter */
        dpp::role role = interaction.get_resolved_role(
                subcommand.get_value<dpp::snowflake>(0)
        );

        if (!RecordBook::teams.contains(role.id))
            return { event.command.channel_id, TeamEmbeds::teamUnregisteredEmbed(role) };

        dpp::user profile = interaction.get_resolved_user(
                subcommand.get_value<dpp::snowflake>(1)
        );

        if (!RecordBook::teams[role.id].members.contains(profile.id))
            return { event.command.channel_id, TeamEmbeds::teamPlayerUnregisteredEmbed(profile, role) };

        RecordBook::teams[role.id].members.erase(profile.id);
        RecordBook::players[profile.id].teamID = 0;
        RecordBook::save_player(profile.id);
        RecordBook::save_team(role.id);
        return { event.command.channel_id, TeamEmbeds::teamRemovedPlayerEmbed(profile, role)};
    }
    else if (subcommand.name == "view"){
        if (subcommand.options.empty()){
            return { event.command.channel_id, TeamEmbeds::teamViewAllEmbed(RecordBook::teams) };
        }
        else {
            /* Get the team role from the parameter */
            dpp::role role = interaction.get_resolved_role(
                    subcommand.get_value<dpp::snowflake>(0)
            );

            if (!RecordBook::teams.contains(role.id)) {
                return { event.command.channel_id, TeamEmbeds::teamUnregisteredEmbed(role) };
            }
            return { event.command.channel_id, TeamEmbeds::teamViewRoleEmbed(role)};
        }
    }

    return { event.command.channel_id, UtilityEmbeds::testEmbed() };
}
