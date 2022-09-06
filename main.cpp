#include <dpp/dpp.h>

#include "src/commands/include/PingCommand.h"
#include "src/commands/include/TeamCommand.h"
#include "src/commands/include/MatchCommand.h"
#include "src/commands/include/PlayerCommand.h"
#include "src/commands/include/ScheduleCommand.h"

int main() {
    dpp::cluster bot(RecordBook::getBotToken(), dpp::intents::i_all_intents);

    bot.on_log(dpp::utility::cout_logger());

    bot.on_slashcommand([&bot](const dpp::slashcommand_t& event) {
        dpp::interaction interaction = event.command;
        dpp::command_interaction cmd_data = interaction.get_command_interaction();

        if (interaction.get_command_name() == "ping") {
            event.reply(PingCommand::msg(event, bot));
        }
        else if (interaction.get_command_name() == "match"){
            event.reply(MatchCommand::msg(event, bot));
        }
        else if (interaction.get_command_name() == "team"){
            event.reply(TeamCommand::msg(event, bot));
        }
        else if (interaction.get_command_name() == "schedule"){
            event.reply(ScheduleCommand::msg(event, bot));
        }
        else if (interaction.get_command_name() == "player"){
            event.reply(PlayerCommand::msg(event, bot));
        }
    });

    bot.on_ready([&bot](const dpp::ready_t& event) {
        if (dpp::run_once<struct register_bot_commands>()) {
            vector<slashcommand> CommandGroup = {
                    PingCommand::cmd(bot.me.id),
                    MatchCommand::cmd(bot.me.id),
                    TeamCommand::cmd(bot.me.id),
                    ScheduleCommand::cmd(bot.me.id),
                    PlayerCommand::cmd(bot.me.id)
            };

            bot.global_bulk_command_create(CommandGroup);
        }
    });

    bot.start(dpp::st_wait);
}
