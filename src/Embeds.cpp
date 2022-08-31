//
// Created by ankit on 8/24/22.
//

#include "include/Embeds.h"

embed Embeds::embedTemplate() {
    /* create the embed */
    embed embed = dpp::embed()
            .set_color(dpp::colors::wrx_blue)
            .set_footer(dpp::embed_footer().set_text("TRLBot • Created by Waycey"))
            .set_timestamp(time(0));

    return embed;
}

embed Embeds::errorEmbed(string msg) {
    embed embed = embedTemplate()
            .set_title("Error")
            .add_field(msg, "Contact a dev for details");

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

    stats << "Wins: " << thisTeam.wins << "\n";
    stats << "Losses: " << thisTeam.losses << "\n";
    if (thisTeam.differential > 0)
        stats << "Differential: +" << thisTeam.differential << "\n";
    else
        stats << "Differential: " << thisTeam.differential << "\n";

    embed embed = embedTemplate()
            .set_title(team.name)
            .add_field(
                    "__Roster__\n",
                    players.str(),
                    true
            )
            .add_field(
                    "_          _",
                    "_          _",
                    true
            )
            .add_field(
                    "__Stats__\n",
                    stats.str(),
                    true
            );

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

embed Embeds::scheduleViewAllMatches() {
    std::ostringstream  matchIDs;
    std::ostringstream homeTeams, awayTeams;

    if (RecordBook::schedule.empty()){
        matchIDs << "None";
        homeTeams << "None";
        awayTeams << "None";
    }
    for (Match match : RecordBook::schedule){
        matchIDs << match.id << "\n";
        homeTeams << match.home->team.get_mention() << "\n";
        awayTeams << match.away->team.get_mention() << "\n";
    }

    embed embed = embedTemplate()
            .set_title("All Scheduled Matches")
            .add_field(
                    "Match ID",
                    matchIDs.str(),
                    true
            )
            .add_field(
                    "Home",
                    homeTeams.str(),
                    true
            )
            .add_field(
                    "Away",
                    awayTeams.str(),
                    true
            );

    return embed;
}

embed Embeds::scheduleViewMatch(int id) {
    Match match = RecordBook::schedule[RecordBook::getMatch(id)];

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
        stats << "TRL Rating:   " << 0 << "\n";
    }
    else {
        stats << "Goals:        " << player.getStatistic(Player::GOALS) << "\n";
        stats << "Saves:        " << player.getStatistic(Player::SAVES) << "\n";
        stats << "Shots:        " << player.getStatistic(Player::SHOTS) << "\n";
        stats << "Assists:      " << player.getStatistic(Player::ASSISTS) << "\n";
        stats << "TRL Rating:   " << player.getStatistic(Player::AVG_MVPR) << "\n";
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

embed Embeds::matchReplayProcessingComplete(int matchID) {
    std::ostringstream processing;
    processing << "Replays have been processed for " << matchID;

    embed embed = embedTemplate()
            .set_title("Replays Submitted")
            .add_field(
                    processing.str(),
                    "Use `/player view` to view updated stats!",
                    false
            );

    return embed;
}


