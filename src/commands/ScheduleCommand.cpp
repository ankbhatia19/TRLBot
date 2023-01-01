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
            return { event.command.channel_id, ScheduleEmbeds::scheduleViewAllMatches() };
        }
        else {
            int matchID = std::get<int64_t>(subcommand.options[0].value);
            if (!RecordBook::schedule.contains(matchID))
                return { event.command.channel_id, ScheduleEmbeds::scheduleMatchDoesNotExist(matchID) };
            return { event.command.channel_id, ScheduleEmbeds::scheduleViewMatch(matchID) };
        }
    }
    else if (subcommand.name == "edit"){
        int matchID = std::get<int64_t>(subcommand.options[0].value);
        if (!RecordBook::schedule.contains(matchID))
            return { event.command.channel_id, ScheduleEmbeds::scheduleMatchDoesNotExist(matchID) };

        string time = std::get<string>(subcommand.options[3].value);

        if (!std::regex_match(time, std::regex("((1[0-2]|0?[1-9]):([0-5][0-9]) ?([AaPp][Mm]))"))){
            return { event.command.channel_id, ScheduleEmbeds::scheduleInvalidTime() };
        }

        // Convert to all lowercase
        std::transform(time.begin(), time.end(), time.begin(),
                       [](unsigned char c){ return std::tolower(c); });

        RecordBook::schedule[matchID].matchTime.tm_hour = std::stoi(time.substr(0, time.find(':')));
        RecordBook::schedule[matchID].matchTime.tm_min = std::stoi(time.substr(time.find(':') + 1, 2));
        RecordBook::schedule[matchID].matchTime.tm_mon = std::get<int64_t>(subcommand.options[1].value) - 1;
        RecordBook::schedule[matchID].matchTime.tm_mday = std::get<int64_t>(subcommand.options[2].value);

        if (time.find("pm") != std::string::npos)
            RecordBook::schedule[matchID].matchTime.tm_hour += 12;
        if (time.find("am") && (RecordBook::schedule[matchID].matchTime.tm_hour == 12))
            RecordBook::schedule[matchID].matchTime.tm_hour -= 12;

        return { event.command.channel_id, ScheduleEmbeds::scheduleViewMatch(matchID) };
    }

    return { event.command.channel_id, UtilityEmbeds::testEmbed() };
}
