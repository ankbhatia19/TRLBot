//
// Created by ankit on 8/28/22.
//

#include "include/MatchCommand.h"

string MatchCommand::token = RecordBook::getBallchasingToken();
string MatchCommand::url = RecordBook::getBallchasingURL();

slashcommand MatchCommand::cmd(snowflake botID) {

    slashcommand matchcmd("match", "Add, remove, or submit a match", botID);
    matchcmd.add_option(
            /* Create a subcommand type option for "add". */
            dpp::command_option(dpp::co_sub_command, "add", "Add a match")
                    .add_option(dpp::command_option(dpp::co_role, "home", "The role of the home team", true))
                    .add_option(dpp::command_option(dpp::co_role, "away", "The role of the away team", true))
    );
    matchcmd.add_option(
            dpp::command_option(dpp::co_sub_command, "submit", "Submit a match with the given ID")
                .add_option(dpp::command_option(dpp::co_integer, "id", "The match ID", true)
                    .set_min_value(10000)
                    .set_max_value(99999)
                )
                .add_option(dpp::command_option(dpp::co_attachment, "game1", "Game 1 .replay file", true))
                .add_option(dpp::command_option(dpp::co_attachment, "game2", "Game 2 .replay file", true))
                .add_option(dpp::command_option(dpp::co_attachment, "game3", "Game 3 .replay file", true))
                .add_option(dpp::command_option(dpp::co_attachment, "game4", "Game 4 .replay file", false))
                .add_option(dpp::command_option(dpp::co_attachment, "game5", "Game 5 .replay file", false))
    );
    matchcmd.add_option(
            /* Create another subcommand type option for "remove". */
            dpp::command_option(dpp::co_sub_command, "remove", "Remove a match")
                    .add_option(dpp::command_option(dpp::co_number, "id", "The match ID to be removed", true))
    );

    return matchcmd;
}

message MatchCommand::msg(const slashcommand_t &event, cluster& bot) {

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
    else if (subcommand.name == "submit"){
        int matchID = std::get<int64_t>(subcommand.options[0].value);

        for (int i = 1; i < subcommand.options.size(); i++){

            attachment replay = interaction.get_resolved_attachment(
                    subcommand.get_value<dpp::snowflake>(i)
            );

            std::ostringstream filename;
            filename << matchID << "_Game" << i << ".replay";
            string replayName = filename.str();

            bot.request(replay.url, http_method::m_get, [&bot, replayName](const http_request_completion_t& response) {

                std::filesystem::path path{ "Replays" }; // creates a local replays folder
                path /= replayName; // Add a replay file
                std::filesystem::create_directories(path.parent_path()); // add directories based on the object path

                std::ofstream ofs(path, std::ios::out | std::ios::binary);
                ofs << response.body;
                ofs.close();

                string uploadURL = MatchCommand::url + "v2/upload?visibility=public";

                bot.post_rest_multipart(uploadURL, "Authorization", MatchCommand::token, http_method::m_post, "", [](json&, const http_request_completion_t& response){
                    cout << "Response: " << response.body << endl;
                    cout << "Response code: " << response.status << endl;

                }, {"file"}, {utility::read_file(replayName)});
                /*bot.request(uploadURL, http_method::m_post, [](const http_request_completion_t& response) {

                    cout << "Server response code: " << response.status << endl;
                    cout << "Ballchasing upload link: " << response.body << endl;

                }, dpp::utility::read_file(replayName), "multipart/form-data", {{"Authorization", MatchCommand::token}});*/
            });
        }
    }
    return { event.command.channel_id, Embeds::testEmbed() };
}
