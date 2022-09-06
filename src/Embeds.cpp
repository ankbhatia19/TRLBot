//
// Created by ankit on 8/24/22.
//

#include "include/Embeds.h"

dpp::embed Embeds::embedTemplate() {
    /* create the embed */
    dpp::embed embed = dpp::embed()
            .set_color(dpp::colors::wrx_blue)
            .set_footer(dpp::embed_footer().set_text("TRLBot • Created by Waycey"))
            .set_thumbnail("https://raw.githubusercontent.com/ankbhatia19/TRLBot/master/assets/TRL_logo_topright_noBG.png")
            .set_timestamp(time(0));

    return embed;
}

dpp::embed Embeds::errorEmbed(string msg) {
    dpp::embed embed = embedTemplate()
            .set_title("Error")
            .add_field(msg, "Contact a dev for details");

    return embed;
}

dpp::embed Embeds::loadingEmbed() {
    return errorEmbed("The user database is still loading. Please retry this command in a few minutes.");
}

dpp::embed Embeds::testEmbed() {

    dpp::embed embed = embedTemplate()
            .set_title("Coming soon!")
            .add_field(
                    "This command is not yet finished.",
                    "Stay tuned for updates."
            );

    return embed;
}

dpp::embed Embeds::pingEmbed(std::string uptime) {
    dpp::embed embed = embedTemplate()
            .set_title("Pong!")
            .add_field("Uptime", uptime);

    return embed;
}

dpp::embed Embeds::teamUnregisteredEmbed(dpp::role home, dpp::role away) {
    std::ostringstream unregistered;

    if (!RecordBook::teams.contains(home.id)){
        unregistered << home.get_mention() << "\n";
    }
    if (!RecordBook::teams.contains(away.id)){
        unregistered << away.get_mention() << " \n";
    }

    dpp::embed embed = embedTemplate()
            .set_title("Error")
            .add_field(
                    "These team(s) have not been registered: ",
                    unregistered.str() + "Use `/team register` to create a team."
            );

    return embed;
}

dpp::embed Embeds::teamUnregisteredEmbed(dpp::role team) {
    std::ostringstream unregistered;
    unregistered << "";
    if (!RecordBook::teams.contains(team.id)){
        unregistered << team.get_mention();
    }
    unregistered << "\n";
    dpp::embed embed = embedTemplate()
            .set_title("Error")
            .add_field(
                    "These team(s) have not been registered: ",
                    unregistered.str() + "Use `/team register` to create a team."
            );

    return embed;
}

dpp::embed Embeds::teamRegisteredEmbed(dpp::role team) {

    dpp::embed embed = embedTemplate()
            .set_title("Team Created")
            .add_field(
                    "Registered team: ",
                    team.get_mention() + "\nUse `/team add` to fill the team roster."
            );

    return embed;
}

dpp::embed Embeds::teamDelistedEmbed(dpp::role team) {

    dpp::embed embed = embedTemplate()
            .set_title("Team Removed")
            .add_field(
                    "Delisted team: ",
                    team.get_mention() + "\nUse `/team register` to re-register this team."
            );

    return embed;
}

dpp::embed Embeds::teamAddedPlayersEmbed(vector<dpp::user> players, dpp::role team) {

    std::ostringstream playersStr;
    for (auto user : players)
        playersStr << user.get_mention() + " ";

    dpp::embed embed = embedTemplate()
            .set_title("Player Added")
            .add_field(
                    "Registered player(s): ",
                    playersStr.str() + "to " + team.get_mention() + "\nUse `/player info` to view the player(s)."
            );

    return embed;
}

dpp::embed Embeds::teamRemovedPlayerEmbed(dpp::user player, dpp::role team) {

    dpp::embed embed = embedTemplate()
            .set_title("Player Removed")
            .add_field(
                    "Unregistered player: ",
                    player.get_mention() + " from " + team.get_mention() + "\nUse `/player info` to view this player."
            );

    return embed;
}

dpp::embed Embeds::teamPlayerAlreadyRegisteredEmbed(dpp::user player, dpp::role team) {

    dpp::embed embed = embedTemplate()
            .set_title("Error")
            .add_field(
                    "Player already registered: " ,
                    player.get_mention() + " is a member of " + team.get_mention() + "\nUse `/team view` to view this team."
            );

    return embed;
}

dpp::embed Embeds::teamPlayerUnregisteredEmbed(dpp::user player, dpp::role team) {

    dpp::embed embed = embedTemplate()
            .set_title("Error")
            .add_field(
                    "Player not registered: " ,
                    player.get_mention() + "is not a member of " + team.get_mention() + "\nUse `/team view` to view this team."
            );

    return embed;
}

dpp::embed Embeds::teamViewAllEmbed(map<unsigned long long, Team> teams) {

    std::ostringstream teamList;
    std::ostringstream teamCount;

    if (teams.empty())
        teamList << "No registered teams.";

    for (const auto& [key, _] : teams){
        teamList << dpp::find_role(key)->get_mention() << "\n";
    }
    teamCount << "Registered Teams: " << teams.size();

    dpp::embed embed = embedTemplate()
            .set_title("__Teams__")
            .add_field(teamCount.str(), teamList.str());

    return embed;
}

dpp::embed Embeds::teamViewRoleEmbed(dpp::role team) {
    std::ostringstream players;
    std::ostringstream stats;

    Team thisTeam = RecordBook::teams[team.id];

    if (thisTeam.members.empty()){
        players << "None\n";
    }
    for (const auto& [id, _] : thisTeam.members){
        if (dpp::find_user(id) == nullptr)
            return errorEmbed("Member list is still being loaded. Try again in a few minutes.");

        players << dpp::find_user(id)->get_mention() << "\n";
    }

    stats << "Series Wins:          " << thisTeam.wins << "\n";
    stats << "Series Losses:        " << thisTeam.losses << "\n";
    if (thisTeam.differential > 0)
        stats << "Game Differential:   +" << thisTeam.differential << "\n";
    else if (thisTeam.differential < 0)
        stats << "Game Differential:   " << thisTeam.differential << "\n";
    else
        stats << "Game Differential:    " << thisTeam.differential << "\n";

    dpp::embed embed = embedTemplate()
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

dpp::embed Embeds::scheduleViewAllMatches() {
    std::ostringstream  unplayedMatchIDs;
    std::ostringstream unplayedHomeTeams, unplayedAwayTeams;

    std::ostringstream  playedMatchIDs;
    std::ostringstream playedHomeTeams, playedAwayTeams;
    vector<Match> playedMatches, unplayedMatches;

    // Sort matches by stats
    for (const auto& [key, _] : RecordBook::schedule){
        if (RecordBook::schedule[key].matchStatus == Match::status::UNPLAYED){
            unplayedMatches.emplace_back(RecordBook::schedule[key]);
        }
        else if (RecordBook::schedule[key].matchStatus == Match::status::PLAYED){
            playedMatches.emplace_back(RecordBook::schedule[key]);
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
        unplayedHomeTeams << dpp::find_role(match.homeID)->get_mention() << "\n";
        unplayedAwayTeams << dpp::find_role(match.awayID)->get_mention() << "\n";
    }
    for (Match match : playedMatches){
        playedMatchIDs << match.id << "\n";
        playedHomeTeams << dpp::find_role(match.homeID)->get_mention() << "\n";
        playedAwayTeams << dpp::find_role(match.awayID)->get_mention() << "\n";
    }

    dpp::embed embed = embedTemplate()
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

dpp::embed Embeds::scheduleViewMatch(int id) {
    Match match = RecordBook::schedule[id];
    if (match.matchStatus == Match::status::PLAYED)
        return matchCompleteEmbed(id);

    std::ostringstream matchIDstr;
    std::ostringstream homeTeamPlayers;
    std::ostringstream awayTeamPlayers;
    std::ostringstream scheduledTime;
    std::ostringstream lobbyInfo;

    matchIDstr << "Match ID: " << match.id;

    homeTeamPlayers << dpp::find_role(match.homeID)->get_mention() << "\n_ _\n__Roster__\n";
    awayTeamPlayers << dpp::find_role(match.awayID)->get_mention() << "\n_ _\n__Roster__\n";
    if (RecordBook::teams[match.homeID].members.empty())
        homeTeamPlayers << "None\n";

    if (RecordBook::teams[match.awayID].members.empty())
        awayTeamPlayers << "None\n";

    for (const auto& [id, _] : RecordBook::teams[match.homeID].members)
        homeTeamPlayers << dpp::find_user(id)->get_mention() << "\n";

    for (const auto& [id, _] : RecordBook::teams[match.awayID].members)
        awayTeamPlayers << dpp::find_user(id)->get_mention() << "\n";

    if (match.matchTime == nullptr)
        scheduledTime << "None";

    lobbyInfo << "**User: ** trl\n**Pass: ** " << match.id;
    dpp::embed embed = embedTemplate()
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

dpp::embed Embeds::scheduleMatchDoesNotExist(int id) {

    std::ostringstream matchError;
    matchError << "Match ID " << id << " does not exist.";
    dpp::embed embed = embedTemplate()
            .set_title("Error")
            .add_field(
                    matchError.str(),
                    "Use `/match add` to create a match."
            );

    return embed;
}

dpp::embed Embeds::playerView(dpp::user profile) {

    Player player = RecordBook::players[profile.id];

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
    if (player.teamID == 0)
        team << "None";
    else
        team << dpp::find_role(player.teamID)->get_mention();

    dpp::embed embed = embedTemplate()
            .set_title("Player Card")
            .add_field(
                    "Player",
                    find_user(profile.id)->get_mention(),
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

dpp::embed Embeds::playerNotFound(dpp::user profile) {
    dpp::embed embed = embedTemplate()
            .set_title("Error")
            .add_field("This player is not registered: ",
                       profile.get_mention() + "\nUse `/player register` to create a new player account."
            );
    return embed;
}

dpp::embed Embeds::playerUsernameExists(dpp::user profile, string name){
    dpp::embed embed = embedTemplate()
            .set_title("Error")
            .add_field("Duplicate username entered: ",
                       profile.get_mention() + " already has registered the username " + name
                       + "\nUse `/player info` to view all registered usernames."
            );
    return embed;
}

dpp::embed Embeds::playerAddedUsername(dpp::user user, string username) {
    dpp::embed embed = embedTemplate()
            .set_title("Added username")
            .add_field(
                    "Rocket League username registered:",
                    username + " to " + user.get_mention() + "\nUse `/player info` to view all registered usernames.",
                    false
            );

    return embed;
}

dpp::embed Embeds::matchReplayProcessing(int matchID) {
    std::ostringstream processing;
    processing << "Processing replay for match " << matchID;

    dpp::embed embed = embedTemplate()
            .set_title("Replay Processing")
            .add_field(
                    processing.str(),
                    "Please do not use any other bot commands during this time.",
                    false
            );

    return embed;
}

dpp::embed Embeds::matchNotFound(int matchID) {
    std::ostringstream error;
    error << "Match not found for Match #" << matchID;

    dpp::embed embed = embedTemplate()
            .set_title("Error")
            .add_field(
                    error.str(),
                    "Use `/match create` to create a new match.",
                    false
            );

    return embed;
}

dpp::embed Embeds::matchAlreadyPlayed(int matchID) {
    std::ostringstream error;
    error << "Match #" << matchID << " has already been submitted.";

    dpp::embed embed = embedTemplate()
            .set_title("Error")
            .add_field(
                    error.str(),
                    "Use `/match create` to create a new match.",
                    false
            );

    return embed;
}

dpp::embed Embeds::matchCompleteEmbed(int matchID) {
    std::ostringstream title;
    title << "Match #" << matchID << " Complete";

    dpp::embed embed = embedTemplate()
            .set_title(title.str());

    embed.add_field("Home", dpp::find_role(RecordBook::schedule[matchID].homeID)->get_mention(), true);
    embed.add_field("Away", dpp::find_role(RecordBook::schedule[matchID].awayID)->get_mention(), true);

    string allStats;
    std::ostringstream headerLine;
    headerLine << "            " << "Home" << "    " << "Away";
    for (const auto& [key, _] : RecordBook::schedule[matchID].matchScores){
        int homeGoals = 0, awayGoals = 0;
        for (auto score : RecordBook::schedule[matchID].matchScores[key]){
            homeGoals += score.homeGoals;
            awayGoals += score.awayGoals;
        }
        std::ostringstream gameStats;
        gameStats << "Game #" << key << "      " << homeGoals << "       " << awayGoals;
        allStats += gameStats.str() + "\n";
    }
    embed.add_field("Game Stats", "```" + headerLine.str() + "\n" + allStats + "```", false);
    switch (RecordBook::schedule[matchID].matchWinner){
        case (Match::affiliation::HOME):
            embed.add_field("Winner", dpp::find_role(RecordBook::schedule[matchID].homeID)->get_mention(), false);
            break;
        case (Match::affiliation::AWAY):
            embed.add_field("Winner", dpp::find_role(RecordBook::schedule[matchID].awayID)->get_mention(), false);
            break;
        case Match::NONE:
            break;
    }

    return embed;
}

dpp::embed Embeds::playersNotRegistered(vector<string> unregistered) {

    std::ostringstream unregisteredString;
    for (string username : unregistered){
        unregisteredString << "\"" << username << "\" ";
    }

    dpp::embed embed = embedTemplate()
            .set_title("Error Submitting Replays")
            .add_field(
                "The following players are unregistered:",
                unregisteredString.str() + "\nUse `player register` to add unknown players.",
                false
            );

    return embed;
}

dpp::embed Embeds::insufficientPermsEmbed(dpp::interaction interaction) {

    dpp::embed embed = embedTemplate()
            .set_title("Error")
            .add_field(
                "Insufficient Permissions",
                interaction.get_issuing_user().get_mention() + " does not have the League Staff role.",
                false
            );

    return embed;
}

