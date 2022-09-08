//
// Created by ankit on 8/28/22.
//

#include "include/MatchCommand.h"

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

    return matchcmd;
}

dpp::message MatchCommand::msg(const dpp::slashcommand_t &event, dpp::cluster& bot) {

    dpp::interaction interaction = event.command;
    dpp::command_interaction cmd_data = interaction.get_command_interaction();
    auto subcommand = cmd_data.options[0];

    if (subcommand.name == "create") {

        if (!Utilities::checkPerms(interaction))
            return { event.command.channel_id, UtilityEmbeds::insufficientPermsEmbed(interaction) };

        /* Get the home role from the parameter */
        dpp::role home = interaction.get_resolved_role(
                subcommand.get_value<dpp::snowflake>(0)
        );
        dpp::role away = interaction.get_resolved_role(
                subcommand.get_value<dpp::snowflake>(1)
        );
        if (!RecordBook::teams.contains(home.id) || !RecordBook::teams.contains(away.id)) {
            return {event.command.channel_id, TeamEmbeds::teamUnregisteredEmbed(home, away)};
        }

        Match match(home.id, away.id);
        RecordBook::schedule.insert({match.id, match});

        return {event.command.channel_id, ScheduleEmbeds::scheduleViewMatch(match.id) };

    }
    else if (subcommand.name == "submit"){
        int matchID = std::get<int64_t>(subcommand.options[0].value);
        string interaction_token = event.command.token;

        if (!RecordBook::schedule.contains(matchID)){
            return { event.command.channel_id, MatchEmbeds::matchNotFound(matchID) };
        }
        if (RecordBook::schedule[matchID].matchStatus == Match::status::PLAYED){
            return { event.command.channel_id, MatchEmbeds::matchAlreadyPlayed(matchID) };
        }

        int totalNumReplays = subcommand.options.size() - 1;
        for (int replayNum = 1; replayNum < subcommand.options.size(); replayNum++){

            dpp::attachment replay = interaction.get_resolved_attachment(
                    subcommand.get_value<dpp::snowflake>(replayNum)
            );

            std::ostringstream filename;
            filename << matchID << "_Game" << replayNum << ".replay";
            string replayName = filename.str();

            bot.request(replay.url, dpp::http_method::m_get, [&bot, &event, interaction_token, matchID, replayName, replayNum, totalNumReplays](const dpp::http_request_completion_t& response) {

                // Download a replay
                std::filesystem::path path{ "Replays" }; // creates a local replays folder
                path /= replayName; // Add a replay file
                std::filesystem::create_directories(path.parent_path()); // add directories based on the object path

                std::ofstream ofs(path, std::ios::out | std::ios::binary);
                ofs << response.body;
                ofs.close();

                BallchasingClient ballchasing;

                // 1. Upload replay to ballchasing
                json uploadData = ballchasing.upload(path, replayName);

                // 2. Patch replay, assign name and group
                ballchasing.group(matchID, replayName, uploadData["id"].get<std::string>());

                // 3. Download replay data from ballchasing
                json replayData = ballchasing.pull(uploadData["id"].get<std::string>());

                // Pre-process replays
                // Create a map such that get(username) will return color, index, player ID, and home/away
                std::map<string, struct MatchCommand::PlayerRecord> playerMap;
                for (int i = 0; i < replayData["blue"]["players"].size(); i++){
                    for (const auto& [key, _] : RecordBook::players){
                        for (string username : RecordBook::players[key].aliases){
                            if (replayData["blue"]["players"][i]["name"].get<std::string>() == username){
                                if (RecordBook::players[key].teamID == 0){
                                    bot.interaction_response_edit(interaction_token, { event.command.channel_id, UtilityEmbeds::errorEmbed("Please ensure all players are registered to a team.") });
                                    std::ostringstream log_info;
                                    log_info << "Player.team is nullptr: " << RecordBook::players[key].id;
                                    bot.log(dpp::loglevel::ll_debug, log_info.str());
                                    return;
                                }
                                if (RecordBook::players[key].teamID == RecordBook::schedule[matchID].homeID)
                                    playerMap.insert({username, {"blue", i, RecordBook::players[key].id, Match::affiliation::HOME}});
                                else if (RecordBook::players[key].teamID == RecordBook::schedule[matchID].awayID)
                                    playerMap.insert({username, {"blue", i, RecordBook::players[key].id, Match::affiliation::AWAY}});
                                else
                                    playerMap.insert({username, {"blue", i, RecordBook::players[key].id, Match::affiliation::NONE}});
                            }
                        }
                    }
                }
                // Preprocessing for orange side
                for (int i = 0; i < replayData["orange"]["players"].size(); i++){
                    for (const auto& [key, _] : RecordBook::players){
                        for (string username : RecordBook::players[key].aliases){
                            if (replayData["orange"]["players"][i]["name"].get<std::string>() == username){
                                if (RecordBook::players[key].teamID == 0){
                                    bot.interaction_response_edit(interaction_token, { event.command.channel_id, UtilityEmbeds::errorEmbed("Please ensure all players are registered to a team.") });
                                    std::ostringstream log_info;
                                    log_info << "Player.team is nullptr: " << RecordBook::players[key].id;
                                    bot.log(dpp::loglevel::ll_debug, log_info.str());
                                    return;
                                }
                                if (RecordBook::players[key].teamID == RecordBook::schedule[matchID].homeID)
                                    playerMap.insert({username, {"orange", i, RecordBook::players[key].id, Match::affiliation::HOME}});
                                else if (RecordBook::players[key].teamID == RecordBook::schedule[matchID].awayID)
                                    playerMap.insert({username, {"orange", i, RecordBook::players[key].id, Match::affiliation::AWAY}});
                                else
                                    playerMap.insert({username, {"orange", i, RecordBook::players[key].id, Match::affiliation::NONE}});
                            }
                        }
                    }
                }

                // Check if all players are accounted for- if not, generate an error
                vector<string> unregistered;
                for (int i = 0; i < replayData["blue"]["players"].size(); i++){
                    if (!playerMap.contains(replayData["blue"]["players"][i]["name"].get<std::string>()))
                        unregistered.emplace_back(replayData["blue"]["players"][i]["name"].get<std::string>());
                }
                for (int i = 0; i < replayData["orange"]["players"].size(); i++){
                    if (!playerMap.contains(replayData["orange"]["players"][i]["name"].get<std::string>()))
                        unregistered.emplace_back(replayData["orange"]["players"][i]["name"].get<std::string>());
                }

                if (!unregistered.empty()){
                    bot.interaction_response_edit(interaction_token, {event.command.channel_id,
                                                                      MatchEmbeds::matchPlayersNotRegistered(unregistered)});
                    return;
                }

                // Process replays with the player map previously created
                for (const auto& [key, _] : playerMap){
                    std::ostringstream log_info;
                    log_info << "Found " << key << " in replay " << replayNum << "/" << totalNumReplays << " (match #" << matchID << ")";
                    bot.log(dpp::loglevel::ll_info, log_info.str());

                    string color = playerMap[key].color;
                    int index = playerMap[key].index;
                    dpp::snowflake playerID = playerMap[key].playerID;
                    enum Match::affiliation team = playerMap[key].team;

                    int goals = (int) replayData[color]["players"][index]["stats"]["core"]["goals"].get<int64_t>();
                    if (!RecordBook::schedule[matchID].matchScores.contains(replayNum))
                        RecordBook::schedule[matchID].matchScores.insert({replayNum, vector<Match::score>()});
                    switch (team){
                        case Match::HOME:
                            RecordBook::schedule[matchID].matchScores[replayNum].emplace_back(Match::score{goals, 0});
                            break;
                        case Match::AWAY:
                            RecordBook::schedule[matchID].matchScores[replayNum].emplace_back(Match::score{0, goals});
                            break;
                        case Match::NONE:
                            break;
                    }
                    RecordBook::players[playerID].stats.emplace_back(Player::MatchStatistic{
                            matchID,
                            (int) replayData[color]["players"][index]["stats"]["core"]["shots"].get<int64_t>(),
                            (int) replayData[color]["players"][index]["stats"]["core"]["goals"].get<int64_t>(),
                            (int) replayData[color]["players"][index]["stats"]["core"]["saves"].get<int64_t>(),
                            (int) replayData[color]["players"][index]["stats"]["core"]["assists"].get<int64_t>()
                    });
                }
                std::ostringstream log_info;
                log_info << "Submitted replay " << replayNum << "/" << totalNumReplays << " (match #" << matchID << ")";
                bot.log(dpp::loglevel::ll_info, log_info.str());
                if (replayNum == totalNumReplays) {
                    RecordBook::schedule[matchID].determineWinner();
                    bot.interaction_response_edit(interaction_token, {event.command.channel_id,
                                                                      MatchEmbeds::matchCompleteEmbed(matchID)});
                }
                // Replay processing finished
            });
        }
        return { event.command.channel_id, MatchEmbeds::matchReplayProcessing(matchID) };
    }
    return { event.command.channel_id, UtilityEmbeds::testEmbed() };
}
