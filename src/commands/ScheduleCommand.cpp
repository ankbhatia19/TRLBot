//
// Created by ankit on 8/28/22.
//

#include "ScheduleCommand.h"

dpp::slashcommand ScheduleCommand::cmd(dpp::snowflake botID) {
    dpp::slashcommand schedcmd("schedule", "View and edit the schedule", botID);

    schedcmd.add_option(
            /* Create a subcommand type option for "view". */
            dpp::command_option(
                    dpp::co_sub_command, "view", "View all scheduled matches or the indicated match")
                    .add_option(
                            dpp::command_option(dpp::co_integer, "id", "The match ID to view", false)
                                    .set_min_value(10000)
                                    .set_max_value(99999)
                    )
    );
    schedcmd.add_option(
            /* Create another subcommand type option for "edit". */
            dpp::command_option(dpp::co_sub_command, "edit", "Edit a match")
                    .add_option(
                            dpp::command_option(dpp::co_integer, "id", "The match ID to edit", true)
                                    .set_min_value(10000)
                                    .set_max_value(99999)
                    )
                    .add_option(dpp::command_option(dpp::co_integer, "month", "The month to schedule this match", true)
                                    .set_min_value(1)
                                    .set_max_value(12)
                    )
                    .add_option(dpp::command_option(dpp::co_integer, "day", "The day to schedule this match", true)
                                        .set_min_value(1)
                                        .set_max_value(31)
                    )
                    .add_option(dpp::command_option(dpp::co_string, "time", "The time to schedule this match", true))
    );
    schedcmd.add_option(
            /* Create another subcommand type option for "help". */
            dpp::command_option(dpp::co_sub_command, "help", "The help page for this command")
    );

    return schedcmd;
}

dpp::message ScheduleCommand::msg(const dpp::slashcommand_t &event, dpp::cluster& bot) {
    dpp::interaction interaction = event.command;
    dpp::command_interaction cmd_data = interaction.get_command_interaction();
    auto subcommand = cmd_data.options[0];

    if (subcommand.name == "help") {
        return { event.command.channel_id, ScheduleEmbeds::scheduleHelpEmbed() };
    }
    else if (subcommand.name == "view"){
        if (subcommand.options.empty()){

            return { event.command.channel_id, UtilityEmbeds::testEmbed() };
        }
        else {
            SQLite::Database db("rocket_league.db", SQLite::OPEN_READWRITE);
            int match_id = std::get<int64_t>(subcommand.options[0].value);

            if (!Match::has_id(db, match_id))
                return { event.command.channel_id, ScheduleEmbeds::scheduleMatchDoesNotExist(match_id) };

            return { event.command.channel_id, ScheduleEmbeds::scheduleViewMatch(match_id) };
        }
    }
    else if (subcommand.name == "edit"){
        int matchID = std::get<int64_t>(subcommand.options[0].value);

        return { event.command.channel_id, UtilityEmbeds::testEmbed() };
    }

    return { event.command.channel_id, UtilityEmbeds::testEmbed() };
}
