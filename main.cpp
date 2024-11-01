#include <dpp/dpp.h>

#include "src/commands/include/PingCommand.h"
#include "src/commands/include/HelpCommand.h"
#include "src/commands/include/TeamCommand.h"
#include "src/commands/include/MatchCommand.h"
#include "src/commands/include/PlayerCommand.h"
#include "src/commands/include/ScheduleCommand.h"
#include "src/include/Utilities.h"

int main() {

    SQLite::Database db("rocket_league.db", SQLite::OPEN_READWRITE|SQLite::OPEN_CREATE);
    Player::table_init(db);
    Game::table_init(db);
    Team::table_init(db);
    Match::table_init(db);

    dpp::cluster bot(Utilities::getBotToken(), dpp::intents::i_all_intents);

    bot.on_log(dpp::utility::cout_logger());

    bot.on_slashcommand([&bot](const dpp::slashcommand_t& event) {
        dpp::interaction interaction = event.command;
        dpp::command_interaction cmd_data = interaction.get_command_interaction();

        if (interaction.get_command_name() == "ping") {
            event.reply(PingCommand::msg(event, bot));
        }
        else if (interaction.get_command_name() == "help"){
            event.reply(HelpCommand::msg(event, bot));
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
            vector<dpp::slashcommand> CommandGroup = {
                    PingCommand::cmd(bot.me.id),
                    HelpCommand::cmd(bot.me.id),
                    MatchCommand::cmd(bot.me.id),
                    TeamCommand::cmd(bot.me.id),
                    ScheduleCommand::cmd(bot.me.id),
                    PlayerCommand::cmd(bot.me.id)
            };

            bot.global_bulk_command_create(CommandGroup, [](const dpp::confirmation_callback_t& result){
                Utilities::cmd_init(std::get<dpp::slashcommand_map>(result.value));
            });
        }
    });

    bot.start(dpp::st_wait);

}
