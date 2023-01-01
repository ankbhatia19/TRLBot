//
// Created by ankit on 8/28/22.
//

#include "PlayerCommand.h"

dpp::slashcommand PlayerCommand::cmd(dpp::snowflake botID) {
    dpp::slashcommand playercmd("player", "Register, remove, and view stats of a player", botID);

    playercmd.add_option(
            /* Create a subcommand type option for "info". */
            dpp::command_option(dpp::co_sub_command, "info", "View the info of a player")
                    .add_option(dpp::command_option(dpp::co_user, "user", "The username to view", false))
    );
    playercmd.add_option(
            /* Create a subcommand type option for "register". */
            dpp::command_option(dpp::co_sub_command, "register", "Register a new Rocket League username")
                    .add_option(dpp::command_option(dpp::co_string, "name", "The username to add", true))
                    .add_option(dpp::command_option(dpp::co_user, "user", "The user to which the username should be added", false))
    );
    playercmd.add_option(
            /* Create a subcommand type option for "unregister". */
            dpp::command_option(dpp::co_sub_command, "unregister", "Unregister a Rocket League username")
                    .add_option(dpp::command_option(dpp::co_string, "name", "The username to remove", true))
                    .add_option(dpp::command_option(dpp::co_user, "user", "The user from which the username should be removed", false))
    );
    playercmd.add_option(
            /* Create another subcommand type option for "help". */
            dpp::command_option(dpp::co_sub_command, "help", "The help page for this command")
    );

    return playercmd;
}

dpp::message PlayerCommand::msg(const dpp::slashcommand_t &event, dpp::cluster& bot) {
    dpp::interaction interaction = event.command;
    dpp::command_interaction cmd_data = interaction.get_command_interaction();
    auto subcommand = cmd_data.options[0];

    if (subcommand.name == "help") {
        return { event.command.channel_id, PlayerEmbeds::playerHelpEmbed() };
    }
    else if (subcommand.name == "info"){
        dpp::user profile;
        if (subcommand.options.empty()){
            // view info of self
            profile = interaction.get_issuing_user();
        }
        else {
            // view info of tagged player
            profile = interaction.get_resolved_user(
                    subcommand.get_value<dpp::snowflake>(0)
            );
        }
        if (!RecordBook::players.contains(profile.id))
            return { event.command.channel_id, PlayerEmbeds::playerNotFound(profile) };

        cout << "Player ID: " << RecordBook::players[profile.id].id << endl;
        cout << "Team ID: " << RecordBook::players[profile.id].teamID << endl;
        cout << "Stats List: " << endl;
        for (auto stat : RecordBook::players[profile.id].stats){
            cout << "{" << stat.matchID << ", "<< stat.goals << ", "<< stat.shots << ", "<< stat.assists << ", " << stat.saves << "}" << endl;
        }
        cout << "Aliases List: " << endl;
        for (auto name : RecordBook::players[profile.id].aliases){
            cout << name << endl;
        }

        return { event.command.channel_id, PlayerEmbeds::playerView(profile) };
    }
    else if (subcommand.name == "register"){
        string username = std::get<string>(subcommand.options[0].value);
        dpp::user profile;
        if (subcommand.options.size() == 2){
            // ensure that permission requirements are met
            if (!Utilities::checkPerms(interaction))
                return { event.command.channel_id, UtilityEmbeds::insufficientPermsEmbed(interaction) };

            profile = interaction.get_resolved_user(
                    subcommand.get_value<dpp::snowflake>(1)
            );
        }
        else {
            profile = interaction.get_issuing_user();
        }

        if (!RecordBook::players.contains(profile.id)){
            RecordBook::players.insert({profile.id, {(unsigned long long)profile.id}});
            std::ostringstream log_info;
            log_info << "Player created: " << profile.id;
            bot.log(dpp::loglevel::ll_info, log_info.str());
        }

        for (const auto& [id, _] : RecordBook::players){
            if (RecordBook::players[id].containsAlias(username))
                return { event.command.channel_id, PlayerEmbeds::playerUsernameExists(*dpp::find_user(id), username) };
        }

        RecordBook::players[profile.id].aliases.push_back(username);
        RecordBook::save_player(profile.id);

        return { event.command.channel_id, PlayerEmbeds::playerAddedUsername(profile, username) };
    }
    else if (subcommand.name == "unregister"){
        string username = std::get<string>(subcommand.options[0].value);
        dpp::user profile;
        if (subcommand.options.size() == 2){
            // ensure that permission requirements are met
            if (!Utilities::checkPerms(interaction))
                return { event.command.channel_id, UtilityEmbeds::insufficientPermsEmbed(interaction) };

            profile = interaction.get_resolved_user(
                    subcommand.get_value<dpp::snowflake>(1)
            );
        }
        else {
            profile = interaction.get_issuing_user();
        }

        if (!RecordBook::players[profile.id].containsAlias(username)){
            return { event.command.channel_id, PlayerEmbeds::playerUsernameDoesNotExist(profile, username) };
        }

        RecordBook::players[profile.id].aliases.erase(
                std::remove(
                    RecordBook::players[profile.id].aliases.begin(),
                    RecordBook::players[profile.id].aliases.end(),
                    username
                ),
                RecordBook::players[profile.id].aliases.end()
        );

        RecordBook::save_player(profile.id);

        return { event.command.channel_id, PlayerEmbeds::playerRemovedUsername(profile, username) };
    }

    return { event.command.channel_id, UtilityEmbeds::testEmbed() };
}
