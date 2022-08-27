//
// Created by ankit on 8/25/22.
//

#include "include/CommandProcessor.h"

embed CommandProcessor::pingCommand(interaction interaction){
    return Embeds::testEmbed();
}

embed CommandProcessor::matchCommand(interaction interaction) {
    command_interaction cmd_data = interaction.get_command_interaction();
    auto subcommand = cmd_data.options[0];

    if (subcommand.name == "add") {
        /* Checks if the subcommand has any options. */
        if (subcommand.options.size() == 2) {
            /* Get the home role from the parameter */
            dpp::role home = interaction.get_resolved_role(
                    subcommand.get_value<dpp::snowflake>(0)
            );
            dpp::role away = interaction.get_resolved_role(
                    subcommand.get_value<dpp::snowflake>(1)
            );
            if (!Schedule::hasTeam(home.id) || !Schedule::hasTeam(away.id)){
                return Embeds::teamUnregisteredEmbed(home, away);
            }
            Match match(&Schedule::teams[Schedule::getTeam(home.id)], &Schedule::teams[Schedule::getTeam(away.id)]);
            Schedule::schedule.push_back(match);
            return Embeds::matchCreatedEmbed(
                    match.id,
                    home.name,
                    away.name,
                    Schedule::teams[Schedule::getTeam(home.id)].players,
                    Schedule::teams[Schedule::getTeam(away.id)].players);
        }
    }
    return Embeds::testEmbed();
}

embed CommandProcessor::teamCommand(interaction interaction) {
    command_interaction cmd_data = interaction.get_command_interaction();
    auto subcommand = cmd_data.options[0];

    if (subcommand.name == "register"){

        /* Get the team role from the parameter */
        role role = interaction.get_resolved_role(
                subcommand.get_value<dpp::snowflake>(0)
        );
        if (Schedule::hasTeam(role.id))
            return Embeds::errorEmbed("Team [" + role.name + "] already exists.");

        Team team(role);
        Schedule::teams.push_back(team);
        return Embeds::teamRegisteredEmbed(role);
    }
    else  if (subcommand.name == "delist"){

        /* Get the team role from the parameter */
        role role = interaction.get_resolved_role(
                subcommand.get_value<dpp::snowflake>(0)
        );

        if (!Schedule::hasTeam(role.id))
            return Embeds::teamUnregisteredEmbed(role);

        Schedule::teams.erase(Schedule::teams.begin() + Schedule::getTeam(role.id));
        return Embeds::teamDelistedEmbed(role);
    }
    else if (subcommand.name == "add") {

        /* Get the team role from the parameter */
        role role = interaction.get_resolved_role(
                subcommand.get_value<dpp::snowflake>(0)
        );

        if (!Schedule::hasTeam(role.id))
            return Embeds::teamUnregisteredEmbed(role);

        user profile = interaction.get_resolved_user(
                subcommand.get_value<dpp::snowflake>(1)
        );
        if (Schedule::teams[Schedule::getTeam(role.id)].hasPlayer(profile.id))
            return Embeds::teamPlayerAlreadyRegisteredEmbed(profile, role);

        Player player(profile);
        Schedule::teams[Schedule::getTeam(role.id)].players.push_back(player);
        return Embeds::teamAddedPlayerEmbed(profile, role);
    }
    else if (subcommand.name == "remove"){
        /* Get the team role from the parameter */
        role role = interaction.get_resolved_role(
                subcommand.get_value<dpp::snowflake>(0)
        );

        if (!Schedule::hasTeam(role.id))
            return Embeds::teamUnregisteredEmbed(role);

        user profile = interaction.get_resolved_user(
                subcommand.get_value<dpp::snowflake>(1)
        );
        Team team = Schedule::teams[Schedule::getTeam(role.id)];
        if (!team.hasPlayer(profile.id))
            return Embeds::teamPlayerUnregisteredEmbed(profile, role);

        team.players.erase(team.players.begin() + team.getPlayer(role.id));

        return Embeds::teamRemovedPlayerEmbed(profile, role);
    }
    else if (subcommand.name == "view"){
        if (subcommand.options.empty()){
            return Embeds::teamViewAllEmbed(Schedule::teams);
        }
        else {
            /* Get the team role from the parameter */
            role role = interaction.get_resolved_role(
                    subcommand.get_value<dpp::snowflake>(0)
            );

            if (!Schedule::hasTeam(role.id)) {
                return Embeds::teamUnregisteredEmbed(role);
            }
            return Embeds::teamViewRoleEmbed(role);
        }
    }

    return Embeds::testEmbed();
}

embed CommandProcessor::scheduleCommand(interaction interaction) {
    command_interaction cmd_data = interaction.get_command_interaction();
    auto subcommand = cmd_data.options[0];

    cout << "Schedule command called" << endl;

    if (subcommand.name == "view"){
        if (subcommand.options.empty()){
            // view all scheduled matches
        }
        else {

            /* Get the match integer from the parameter */
            //int msg = std::get<int64_t>(interaction.get_parameter("view"));

            //cout << "Msg: " << msg.content;
        }
    }

    return Embeds::testEmbed();
}

embed CommandProcessor::playerCommand(interaction interaction) {

    return Embeds::testEmbed();
}
