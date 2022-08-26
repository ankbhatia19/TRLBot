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

            return Embeds::matchEmbed(
                    00000,
                    home.name,
                    away.name,
                    home.get_members(),
                    away.get_members());
        }
    }
    return Embeds::testEmbed();
}

embed CommandProcessor::teamCommand(interaction interaction) {
    command_interaction cmd_data = interaction.get_command_interaction();
    auto subcommand = cmd_data.options[0];

    if (subcommand.name == "register"){
        if (subcommand.options.size() == 1){
            /* Get the team role from the parameter */
            role role = interaction.get_resolved_role(
                    subcommand.get_value<dpp::snowflake>(0)
            );
            Team team(role.id);
            Schedule::teams.push_back(team);
            cout << "Added team with id: " << team.id << endl;
        }
    }
    else  if (subcommand.name == "delist"){
        if (subcommand.options.size() == 1){
            /* Get the team role from the parameter */
            role role = interaction.get_resolved_role(
                    subcommand.get_value<dpp::snowflake>(0)
            );
            if (Schedule::hasTeam(role.id)){
                Schedule::teams.erase(Schedule::teams.begin() + Schedule::getTeam(role.id));
                cout << "Delisted team with id: " << role.id << endl;
            }
        }
    }
    else if (subcommand.name == "add"){

    }
    else if (subcommand.name == "remove"){

    }

    return Embeds::testEmbed();
}

embed CommandProcessor::scheduleCommand(interaction interaction) {
    return Embeds::testEmbed();
}

embed CommandProcessor::playerCommand(interaction interaction) {
    return Embeds::testEmbed();
}
