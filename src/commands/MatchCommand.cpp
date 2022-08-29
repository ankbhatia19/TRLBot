//
// Created by ankit on 8/28/22.
//

#include "include/MatchCommand.h"

slashcommand MatchCommand::cmd(snowflake botID) {

    slashcommand matchcmd("match", "Add, remove, or submit a match", botID);
    matchcmd.add_option(
            /* Create a subcommand type option for "add". */
            dpp::command_option(dpp::co_sub_command, "add", "Add a match")
                    .add_option(dpp::command_option(dpp::co_role, "home", "The role of the home team", true))
                    .add_option(dpp::command_option(dpp::co_role, "away", "The role of the away team", true))
    );
    matchcmd.add_option(
            /* Create another subcommand type option for "remove". */
            dpp::command_option(dpp::co_sub_command, "remove", "Remove a match")
                    .add_option(dpp::command_option(dpp::co_number, "id", "The match ID to be removed", true))
    );

    return matchcmd;
}

message MatchCommand::msg(const slashcommand_t &event) {

    interaction interaction = event.command;
    command_interaction cmd_data = interaction.get_command_interaction();
    auto subcommand = cmd_data.options[0];

    if (subcommand.name == "add") {

        /* Get the home role from the parameter */
        role home = interaction.get_resolved_role(
                subcommand.get_value<dpp::snowflake>(0)
        );
        role away = interaction.get_resolved_role(
                subcommand.get_value<dpp::snowflake>(1)
        );
        if (!RecordBook::hasTeam(home.id) || !RecordBook::hasTeam(away.id)) {
            return {event.command.channel_id, Embeds::teamUnregisteredEmbed(home, away)};
        }
        Match match(&RecordBook::teams[RecordBook::getTeam(home.id)], &RecordBook::teams[RecordBook::getTeam(away.id)]);
        RecordBook::schedule.push_back(match);

        return {event.command.channel_id, Embeds::scheduleViewMatch(match.id) };

    }

    return { event.command.channel_id, Embeds::testEmbed() };
}
