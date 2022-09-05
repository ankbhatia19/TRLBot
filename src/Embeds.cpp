//
// Created by ankit on 8/24/22.
//

#include "include/Embeds.h"

embed Embeds::embedTemplate() {
    /* create the embed */
    embed embed = dpp::embed()
            .set_color(dpp::colors::wrx_blue)
            .set_footer(dpp::embed_footer().set_text("TRLBot • Created by Waycey"))
            .set_thumbnail("https://raw.githubusercontent.com/ankbhatia19/TRLBot/master/assets/TRL_logo_topright_noBG.png")
            .set_timestamp(time(0));

    return embed;
}

embed Embeds::errorEmbed(string msg) {
    embed embed = embedTemplate()
            .set_title("Error")
            .add_field(msg, "Contact a dev for details");

    return embed;
}

embed Embeds::testEmbed() {

    embed embed = embedTemplate()
            .set_title("Coming soon!")
            .add_field(
                    "This command is not yet finished.",
                    "Stay tuned for updates."
            );

    return embed;
}

embed Embeds::pingEmbed(std::string uptime) {
    embed embed = embedTemplate()
            .set_title("Pong!")
            .add_field("Uptime", uptime);

    return embed;
}

embed Embeds::teamUnregisteredEmbed(role home, role away) {
    std::ostringstream unregistered;

    if (!RecordBook::hasTeam(home.id)){
        unregistered << home.get_mention() << "\n";
    }
    if (!RecordBook::hasTeam(away.id)){
        unregistered << away.get_mention() << " \n";
    }

    embed embed = embedTemplate()
            .set_title("Error")
            .add_field(
                    "These team(s) have not been registered: ",
                    unregistered.str() + "Use `/team register` to create a team."
            );

    return embed;
}

embed Embeds::teamUnregisteredEmbed(role team) {
    std::ostringstream unregistered;
    unregistered << "";
    if (!RecordBook::hasTeam(team.id)){
        unregistered << team.get_mention();
    }
    unregistered << "\n";
    embed embed = embedTemplate()
            .set_title("Error")
            .add_field(
                    "These team(s) have not been registered: ",
                    unregistered.str() + "Use `/team register` to create a team."
            );

    return embed;
}

embed Embeds::teamRegisteredEmbed(role team) {

    embed embed = embedTemplate()
            .set_title("Team Created")
            .add_field(
                    "Registered team: ",
                    team.get_mention() + "\nUse `/team add` to fill the team roster."
            );

    return embed;
}

embed Embeds::teamDelistedEmbed(role team) {

    embed embed = embedTemplate()
            .set_title("Team Removed")
            .add_field(
                    "Delisted team: ",
                    team.get_mention() + "\nUse `/team register` to re-register this team."
            );

    return embed;
}

embed Embeds::teamAddedPlayersEmbed(user player1, user player2, user player3, role team) {

    std::ostringstream players;
    if (player1.id != snowflake(0))
        players << player1.get_mention() << " ";
    if (player2.id != snowflake(0))
        players << player2.get_mention() << " ";
    if (player3.id != snowflake(0))
        players << player3.get_mention() << " ";

    embed embed = embedTemplate()
            .set_title("Player Added")
            .add_field(
                    "Registered player(s): ",
                    players.str() + "to " + team.get_mention() + "\nUse `/player info` to view the player(s)."
            );

    return embed;
}

embed Embeds::teamRemovedPlayerEmbed(user player, role team) {

    embed embed = embedTemplate()
            .set_title("Player Removed")
            .add_field(
                    "Unregistered player: ",
                    player.get_mention() + " from " + team.get_mention() + "\nUse `/player info` to view this player."
            );

    return embed;
}

embed Embeds::teamPlayerAlreadyRegisteredEmbed(user player, role team) {

    embed embed = embedTemplate()
            .set_title("Error")
            .add_field(
                    "Player already registered: " ,
                    player.get_mention() + " is a member of " + team.get_mention() + "\nUse `/team view` to view this team."
            );

    return embed;
}

embed Embeds::teamPlayerUnregisteredEmbed(user player, role team) {

    embed embed = embedTemplate()
            .set_title("Error")
            .add_field(
                    "Player not registered: " ,
                    player.get_mention() + "is not a member of " + team.get_mention() + "\nUse `/team view` to view this team."
            );

    return embed;
}

embed Embeds::teamViewAllEmbed(vector<Team> teams) {

    std::ostringstream teamList;
    std::ostringstream teamCount;

    if (teams.empty())
        teamList << "No registered teams.";

    for (auto team : teams){
        teamList << team.team.get_mention() << "\n";
    }
    teamCount << "Registered Teams: " << teams.size();

    embed embed = embedTemplate()
            .set_title("__Teams__")
            .add_field(teamCount.str(), teamList.str());

    return embed;
}

embed Embeds::teamViewRoleEmbed(role team) {
    std::ostringstream players;
    std::ostringstream stats;

    Team thisTeam = RecordBook::teams[RecordBook::getTeam(team.id)];

    if (thisTeam.players.empty()){
        players << "None\n";
    }
    for (auto member : thisTeam.players){
        players << member.profile.get_mention() << "\n";
    }

    stats << "Series Wins:          " << thisTeam.wins << "\n";
    stats << "Series Losses:        " << thisTeam.losses << "\n";
    if (thisTeam.differential > 0)
        stats << "Game Differential:   +" << thisTeam.differential << "\n";
    else if (thisTeam.differential < 0)
        stats << "Game Differential:   " << thisTeam.differential << "\n";
    else
        stats << "Game Differential:    " << thisTeam.differential << "\n";

    embed embed = embedTemplate()
            .set_title("Team Card")
            .add_field(
                "Team",
                team.get_mention(),
                true
            )
            .add_field(
                    "Roster",
                    players.str(),
                    true
            )
            .add_field(
                    "Stats",
                    "```" + stats.str() + "```"
            );

    return embed;
}

embed Embeds::scheduleViewAllMatches() {
    std::ostringstream  unplayedMatchIDs;
    std::ostringstream unplayedHomeTeams, unplayedAwayTeams;

    std::ostringstream  playedMatchIDs;
    std::ostringstream playedHomeTeams, playedAwayTeams;
    vector<Match> playedMatches, unplayedMatches;

    // Sort matches by stats
    for (Match match : RecordBook::schedule){
        if (match.matchStatus == Match::status::UNPLAYED){
            unplayedMatches.emplace_back(match);
        }
        else if (match.matchStatus == Match::status::PLAYED){
            playedMatches.emplace_back(match);
        }
    }

    if (playedMatches.empty()){
        playedMatchIDs << "None";
        playedHomeTeams << "None";
        playedAwayTeams << "None";
    }
    if (unplayedMatches.empty()){
        unplayedMatchIDs << "None";
        unplayedHomeTeams << "None";
        unplayedAwayTeams << "None";
    }
    for (Match match : unplayedMatches){
        unplayedMatchIDs << match.id << "\n";
        unplayedHomeTeams << match.home->team.get_mention() << "\n";
        unplayedAwayTeams << match.away->team.get_mention() << "\n";
    }
    for (Match match : playedMatches){
        playedMatchIDs << match.id << "\n";
        playedHomeTeams << match.home->team.get_mention() << "\n";
        playedAwayTeams << match.away->team.get_mention() << "\n";
    }

    embed embed = embedTemplate()
            .set_title("All Scheduled Matches")
            .add_field(
                    "__Unplayed__",
                    "_ _",
                    false
            )
            .add_field(
                    "Match ID",
                    unplayedMatchIDs.str(),
                    true
            )
            .add_field(
                    "Home",
                    unplayedHomeTeams.str(),
                    true
            )
            .add_field(
                    "Away",
                    unplayedAwayTeams.str(),
                    true
            )
            .add_field(
                    "__Completed__",
                    "_ _",
                    false
            )
            .add_field(
                    "Match ID",
                    playedMatchIDs.str(),
                    true
            )
            .add_field(
                    "Home",
                    playedHomeTeams.str(),
                    true
            )
            .add_field(
                    "Away",
                    playedAwayTeams.str(),
                    true
            );

    return embed;
}

embed Embeds::scheduleViewMatch(int id) {
    Match match = RecordBook::schedule[RecordBook::getMatch(id)];
    if (match.matchStatus == Match::status::PLAYED)
        return matchCompleteEmbed(id);

    std::ostringstream matchIDstr;
    std::ostringstream homeTeamPlayers;
    std::ostringstream awayTeamPlayers;
    std::ostringstream scheduledTime;
    std::ostringstream lobbyInfo;

    matchIDstr << "Match ID: " << match.id;

    homeTeamPlayers << match.home->team.get_mention() << "\n_ _\n__Roster__\n";
    awayTeamPlayers << match.away->team.get_mention() << "\n_ _\n__Roster__\n";
    if (RecordBook::teams[RecordBook::getTeam(match.home->team.id)].players.empty())
        homeTeamPlayers << "None\n";

    if (RecordBook::teams[RecordBook::getTeam(match.away->team.id)].players.empty())
        awayTeamPlayers << "None\n";

    for (auto member : RecordBook::teams[RecordBook::getTeam(match.home->team.id)].players)
        homeTeamPlayers << member.profile.get_mention() << "\n";

    for (auto member : RecordBook::teams[RecordBook::getTeam(match.away->team.id)].players)
        awayTeamPlayers << member.profile.get_mention() << "\n";

    if (match.matchTime == nullptr)
        scheduledTime << "None";

    lobbyInfo << "**User: ** trl\n**Pass: ** " << match.id;
    embed embed = embedTemplate()
            .set_title(matchIDstr.str())
            .add_field(
                    "Home",
                    homeTeamPlayers.str(),
                    true
            )
            .add_field(
                    "_          _",
                    "_          _",
                    true
            )
            .add_field(
                    "Away",
                    awayTeamPlayers.str(),
                    true
            )
            /*.add_field(
                    "Scheduled Time",
                    scheduledTime.str(),
                    false
            )*/
            .add_field(
                    "Lobby Info",
                    lobbyInfo.str(),
                    true
            );

    return embed;
}

embed Embeds::scheduleMatchDoesNotExist(int id) {

    std::ostringstream matchError;
    matchError << "Match ID " << id << " does not exist.";
    embed embed = embedTemplate()
            .set_title("Error")
            .add_field(
                    matchError.str(),
                    "Use `/match add` to create a match."
            );

    return embed;
}

embed Embeds::playerView(user profile) {

    Player player = RecordBook::players[RecordBook::getPlayer(profile.id)];

    std::ostringstream stats;
    if (player.stats.empty()){
        stats << "Goals:        " << 0 << "\n";
        stats << "Saves:        " << 0 << "\n";
        stats << "Shots:        " << 0 << "\n";
        stats << "Assists:      " << 0 << "\n";
        stats << "TRL MMR:      " << 0 << "\n";
    }
    else {
        stats << "Goals:        " << player.getStatistic(Player::GOALS) << "\n";
        stats << "Saves:        " << player.getStatistic(Player::SAVES) << "\n";
        stats << "Shots:        " << player.getStatistic(Player::SHOTS) << "\n";
        stats << "Assists:      " << player.getStatistic(Player::ASSISTS) << "\n";
        stats << "TRL MMR:      " << player.getStatistic(Player::AVG_MVPR) << "\n";
    }

    std::ostringstream usernames;
    if (player.aliases.empty())
        usernames << "None";
    for (auto name : player.aliases)
        usernames << name << "\n";

    std::ostringstream team;
    if (player.team == nullptr)
        team << "None";
    else
        team << player.team->team.get_mention();

    embed embed = embedTemplate()
            .set_title("Player Card")
            .add_field(
                    "Player",
                    player.profile.get_mention(),
                    true
            )
            .add_field(
                    "Team",
                    team.str(),
                    true
            )
            .add_field(
                    "__Season Stats__",
                    "```" + stats.str() + "```",
                    false
            )
            .add_field(
                    "__Registered Usernames__",
                    usernames.str(),
                    false
            );

    return embed;
}

embed Embeds::playerNotFound(user profile) {
    embed embed = embedTemplate()
            .set_title("Error")
            .add_field("This player is not registered: ",
                       profile.get_mention() + "\nUse `/player register` to create a new player account."
            );
    return embed;
}

embed Embeds::playerAddedUsername(user user, string username) {
    embed embed = embedTemplate()
            .set_title("Added username")
            .add_field(
                    "Rocket League username registered:",
                    username + " to " + user.get_mention() + "\nUse `/player info` to view all registered usernames.",
                    false
            );

    return embed;
}

embed Embeds::matchReplayProcessing(int matchID) {
    std::ostringstream processing;
    processing << "Processing replay for match " << matchID;

    embed embed = embedTemplate()
            .set_title("Replay Processing")
            .add_field(
                    processing.str(),
                    "Please do not use any other bot commands during this time.",
                    false
            );

    return embed;
}

embed Embeds::matchNotFound(int matchID) {
    std::ostringstream error;
    error << "Match not found for Match #" << matchID;

    embed embed = embedTemplate()
            .set_title("Error")
            .add_field(
                    error.str(),
                    "Use `/match create` to create a new match.",
                    false
            );

    return embed;
}

embed Embeds::matchAlreadyPlayed(int matchID) {
    std::ostringstream error;
    error << "Match #" << matchID << " has already been submitted.";

    embed embed = embedTemplate()
            .set_title("Error")
            .add_field(
                    error.str(),
                    "Use `/match create` to create a new match.",
                    false
            );

    return embed;
}

embed Embeds::matchCompleteEmbed(int matchID) {
    std::ostringstream title;
    title << "Match #" << matchID << " Complete";

    embed embed = embedTemplate()
            .set_title(title.str());

    embed.add_field("Home", RecordBook::schedule[RecordBook::getMatch(matchID)].home->team.get_mention(), true);
    embed.add_field("Away", RecordBook::schedule[RecordBook::getMatch(matchID)].away->team.get_mention(), true);

    string allStats;
    std::ostringstream headerLine;
    headerLine << "            " << "Home" << "    " << "Away";
    for (const auto& [key, _] : RecordBook::schedule[RecordBook::getMatch(matchID)].matchScores){
        int homeGoals = 0, awayGoals = 0;
        for (auto score : RecordBook::schedule[RecordBook::getMatch(matchID)].matchScores[key]){
            homeGoals += score.homeGoals;
            awayGoals += score.awayGoals;
        }
        std::ostringstream gameStats;
        gameStats << "Game #" << key << "      " << homeGoals << "       " << awayGoals;
        allStats += gameStats.str() + "\n";
    }
    embed.add_field("Game Stats", "```" + headerLine.str() + "\n" + allStats + "```", false);
    switch (RecordBook::schedule[RecordBook::getMatch(matchID)].matchWinner){
        case (Match::affiliation::HOME):
            embed.add_field("Winner", RecordBook::schedule[RecordBook::getMatch(matchID)].home->team.get_mention(), false);
            break;
        case (Match::affiliation::AWAY):
            embed.add_field("Winner", RecordBook::schedule[RecordBook::getMatch(matchID)].away->team.get_mention(), false);
            break;
        case Match::NONE:
            break;
    }

    return embed;
}

embed Embeds::playersNotRegistered(vector<string> unregistered) {

    std::ostringstream unregisteredString;
    for (string username : unregistered){
        unregisteredString << "\"" << username << "\" ";
    }

    embed embed = embedTemplate()
            .set_title("Error Submitting Replays")
            .add_field(
                "The following players are unregistered:",
                unregisteredString.str() + "\nUse `player register` to add unknown players.",
                false
            );

    return embed;
}

embed Embeds::insufficientPermsEmbed(interaction interaction) {

    embed embed = embedTemplate()
            .set_title("Error")
            .add_field(
                "Insufficient Permissions",
                interaction.get_issuing_user().get_mention() + " does not have the League Staff role.",
                false
            );

    return embed;
}
