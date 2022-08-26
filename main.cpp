#include <dpp/dpp.h>
#include "src/include/Match.h"
#include "src/include/Embeds.h"
#include "src/include/Commands.h"
#include "src/include/CommandProcessor.h"

const std::string    BOT_TOKEN    = ":)";

int main() {
    dpp::cluster bot(BOT_TOKEN);

    bot.on_log(dpp::utility::cout_logger());

    bot.on_slashcommand([&bot](const dpp::slashcommand_t& event) {
        dpp::interaction interaction = event.command;
        dpp::command_interaction cmd_data = interaction.get_command_interaction();

        if (interaction.get_command_name() == "ping") {
            event.reply(dpp::message(event.command.channel_id, CommandProcessor::teamCommand(interaction)));
        }
        else if (interaction.get_command_name() == "match"){
            event.reply(dpp::message(event.command.channel_id, CommandProcessor::matchCommand(interaction)));
        }
        else if (interaction.get_command_name() == "team"){
            event.reply(dpp::message(event.command.channel_id, CommandProcessor::teamCommand(interaction)));
        }
        else if (interaction.get_command_name() == "schedule"){
            event.reply(dpp::message(event.command.channel_id, CommandProcessor::scheduleCommand(interaction)));
        }
        else if (interaction.get_command_name() == "player"){
            event.reply(dpp::message(event.command.channel_id, CommandProcessor::playerCommand(interaction)));
        }

    });

    bot.on_ready([&bot](const dpp::ready_t& event) {
        if (dpp::run_once<struct register_bot_commands>()) {
            bot.global_command_create(Commands::pingCommand(bot.me.id));
            bot.global_command_create(Commands::matchCommand(bot.me.id));
            bot.global_command_create(Commands::teamCommand(bot.me.id));
            bot.global_command_create(Commands::scheduleCommand(bot.me.id));
            bot.global_command_create(Commands::playerCommand(bot.me.id));
        }
    });

    bot.start(dpp::st_wait);
}