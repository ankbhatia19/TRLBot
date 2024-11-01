//
// Created by ankit on 8/28/22.
//

#include "MatchCommand.h"

dpp::slashcommand MatchCommand::cmd(dpp::snowflake botID) {

    dpp::slashcommand matchcmd("match", "Add, remove, or submit a match", botID);
    matchcmd.add_option(
            /* Create a subcommand type option for "add". */
            dpp::command_option(dpp::co_sub_command, "create", "Create a match")
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
    matchcmd.add_option(
            /* Create another subcommand type option for "help". */
            dpp::command_option(dpp::co_sub_command, "help", "The help page for this command")
    );

    return matchcmd;
}

dpp::message MatchCommand::msg(const dpp::slashcommand_t &event, dpp::cluster& bot) {

    dpp::interaction interaction = event.command;
    dpp::command_interaction cmd_data = interaction.get_command_interaction();
    auto subcommand = cmd_data.options[0];

    if (subcommand.name == "help") {
        return { event.command.channel_id, MatchEmbeds::help() };
    }
    else if (subcommand.name == "create") {

        /* Get the home role from the parameter */
        dpp::role home = interaction.get_resolved_role(
                subcommand.get_value<dpp::snowflake>(0)
        );
        dpp::role away = interaction.get_resolved_role(
                subcommand.get_value<dpp::snowflake>(1)
        );

        SQLite::Database db("rocket_league.db", SQLite::OPEN_READWRITE);
        auto id = Match::create(db, home.id, away.id);

        std::cout << "Created id: " << id << std::endl;

        return { event.command.channel_id, ScheduleEmbeds::view(id) };

    }
    else if (subcommand.name == "remove"){

        if (!Utilities::checkPerms(interaction))
            return { event.command.channel_id, UtilityEmbeds::error_missing_perms(interaction) };

        int matchID = std::get<int64_t>(subcommand.options[0].value);

        return { event.command.channel_id, UtilityEmbeds::test() };
    }
    else if (subcommand.name == "submit"){

        int matchID = std::get<int64_t>(subcommand.options[0].value);
        string interaction_token = event.command.token;

        std::cout << "Submitting match id " << matchID << std::endl;
        int totalNumReplays = subcommand.options.size() - 1;
        for (int replayNum = 1; replayNum < subcommand.options.size(); replayNum++){

            dpp::attachment replay = interaction.get_resolved_attachment(
                    subcommand.get_value<dpp::snowflake>(replayNum)
            );

            bot.request(replay.url, dpp::http_method::m_get, [&bot, &event, interaction_token, matchID, replayNum, totalNumReplays](const dpp::http_request_completion_t& response) {

                SQLite::Database db("rocket_league.db", SQLite::OPEN_READWRITE);

                // Make a new filename for the replay to be stored
                std::ostringstream filename;
                filename << matchID << "_Game" << replayNum << ".replay";

                // Download the replay
                std::filesystem::path path{"Replays"}; // creates a local replays folder
                path /= filename.str(); // Add a replay file
                std::filesystem::create_directories(path.parent_path()); // add directories based on the object path

                // Save the downloaded data to a local file
                std::ofstream ofs(path, std::ios::out | std::ios::binary);
                ofs << response.body;
                ofs.close();

                BallchasingClient ballchasing;

                // Upload replay to ballchasing
                // TODO: add ability to retry if uploadData["id"] is null
                json uploadData = ballchasing.upload(path, filename.str());

                // Create a new ballchasing subgroup (if not yet created)
                if (Match::get_ballchasing_id(db, matchID).empty()){
                    json groupData = ballchasing.create(matchID);
                    Match::set_ballchasing_id(db, matchID, groupData["id"].get<std::string>());
                }

                // Patch replay, assign name and group
                ballchasing.group(filename.str(), Match::get_ballchasing_id(db, matchID),
                                  uploadData["id"].get<std::string>());

                // Download replay data from ballchasing
                // TODO: add ability to retry if failed
                json replayData = ballchasing.pull(uploadData["id"].get<std::string>());

                // Track all players who are not fully setup
                vector<string> unregistered;
                vector<int64_t> teamless;

                std::pair<int64_t, int64_t> team_ids {
                    Match::get_team(db, matchID, Match::HOME),
                    Match::get_team(db, matchID, Match::AWAY)
                };

                // Insert match data to db
                for (auto player_json: replayData["blue"]["players"]) {
                    auto player_id = Player::get_id(db, player_json["name"]);
                    auto player_team = Player::get_team(db, player_id);

                    if (player_id == 0)
                        unregistered.emplace_back(player_json["name"]);
                    else if (player_team != team_ids.first && player_team != team_ids.second)
                        teamless.emplace_back(player_id);
                    else
                        Game::insert(db, player_id, matchID, replayNum, player_json["stats"]);
                }

                for (auto player_json: replayData["orange"]["players"]) {
                    auto player_id = Player::get_id(db, player_json["name"]);
                    auto player_team = Player::get_team(db, player_id);

                    if (player_id == 0)
                        unregistered.emplace_back(player_json["name"]);
                    else if (player_team != team_ids.first && player_team != team_ids.second)
                        teamless.emplace_back(player_id);
                    else
                        Game::insert(db, player_id, matchID, replayNum, player_json["stats"]);
                }

                if (replayNum == totalNumReplays) {
                    if (!unregistered.empty()) {
                        bot.interaction_response_edit(
                                interaction_token,
                                {event.command.channel_id, MatchEmbeds::error_missing_username(unregistered)}
                        );
                    }
                    else if (!teamless.empty()){
                        bot.interaction_response_edit(
                                interaction_token,
                                {event.command.channel_id, MatchEmbeds::error_missing_team(teamless, matchID)}
                        );
                    }
                    else {
                        Match::set_status(db, matchID, Match::status::PLAYED);
                        bot.interaction_response_edit(
                                interaction_token,
                                {event.command.channel_id, MatchEmbeds::complete(matchID)}
                        );
                    }
                }
                // Replay processing finished
            });
        }
        return { event.command.channel_id, MatchEmbeds::replay_processing(matchID) };
    }
    return { event.command.channel_id, UtilityEmbeds::test() };
}
