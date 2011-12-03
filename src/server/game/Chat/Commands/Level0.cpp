/*
 * Copyright (C) 2008-2011 TrinityCore <http://www.trinitycore.org/>
 * Copyright (C) 2005-2009 MaNGOS <http://getmangos.com/>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "Common.h"
#include "DatabaseEnv.h"
#include "World.h"
#include "Player.h"
#include "Opcodes.h"
#include "Chat.h"
#include "ObjectAccessor.h"
#include "Language.h"
#include "AccountMgr.h"
#include "SystemConfig.h"
#include "revision.h"
#include "Util.h"
#include <cstring>
#include <time.h>

bool ChatHandler::HandleRepAddCommand(const char* args)
{
	Player* SelectedPlayer = getSelectedPlayer();
	Player* me = m_session->GetPlayer();
	if(!SelectedPlayer)
	{
		SendSysMessage(LANG_NO_CHAR_SELECTED);
        SetSentErrorMessage(true);
        return false;
	}

	uint32 myacc = m_session->GetAccountId();
	uint32 targetacc = SelectedPlayer->GetSession()->GetAccountId();

	time_t now;
	struct tm ts;
	char   buf[80];
	time(&now);
	ts = *localtime(&now);
	strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", &ts);

	QueryResult query1 = CharacterDatabase.PQuery("SELECT points FROM rep_system WHERE account = %u", targetacc);
	QueryResult query2 = CharacterDatabase.PQuery("INSERT INTO rep_system (account, points) VALUES('%u', '-1')", targetacc);
	QueryResult query3 = CharacterDatabase.PQuery("INSERT INTO rep_system_check (sender, receiver, date) VALUES('%u', '%u', '%s')", myacc, targetacc, buf);

	QueryResult query4 = CharacterDatabase.PQuery("UPDATE rep_system SET points = 'points' + '1' WHERE account = %u", targetacc);

	if(myacc != targetacc)
	{
		if(!query1)
		{
			if(!query2)
			{
				if(!query3)
					return false;

				return false;
			}

			return false;
		}
		else
		{
			if(!query4)
			{
				if(!query3)
					return false;

				return false;
			}

			return false;
		}

		return false;
	}
	else
	{
		SendSysMessage(LANG_REP_CANT_TARGET_SELF);
		SetSentErrorMessage(true);
		return false;
	}

	ChatHandler(SelectedPlayer).PSendSysMessage(LANG_REP_ADD);
	PSendSysMessage(LANG_REP_ADDED, SelectedPlayer->GetName());

	return true;
}

bool ChatHandler::HandleRepDelCommand(const char* args)
{
	Player* SelectedPlayer = getSelectedPlayer();
	Player* me = m_session->GetPlayer();
	if(!SelectedPlayer)
	{
		SendSysMessage(LANG_NO_CHAR_SELECTED);
        SetSentErrorMessage(true);
        return false;
	}

	uint32 myacc = m_session->GetAccountId();
	uint32 targetacc = SelectedPlayer->GetSession()->GetAccountId();

	time_t now;
	struct tm ts;
	char   buf[80];
	time(&now);
	ts = *localtime(&now);
	strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", &ts);

	QueryResult query1 = CharacterDatabase.PQuery("SELECT points FROM rep_system WHERE account = %u", targetacc);
	QueryResult query2 = CharacterDatabase.PQuery("INSERT INTO rep_system (account, points) VALUES('%u', '-1')", targetacc);
	QueryResult query3 = CharacterDatabase.PQuery("INSERT INTO rep_system_check (sender, receiver, date) VALUES('%u', '%u', '%s')", myacc, targetacc, buf);

	QueryResult query4 = CharacterDatabase.PQuery("UPDATE rep_system SET points = 'points' - '1' WHERE account = %u", targetacc);

	if(myacc != targetacc)
	{
		if(!query1)
		{
			if(!query2)
			{
				if(!query3)
					return false;

				return false;
			}

			return false;
		}
		else
		{
			if(!query4)
			{
				if(!query3)
					return false;

				return false;
			}

			return false;
		}
	}
	else
	{
		SendSysMessage(LANG_REP_CANT_TARGET_SELF);
		SetSentErrorMessage(true);
		return false;
	}

	ChatHandler(SelectedPlayer).PSendSysMessage(LANG_REP_DEL);
	PSendSysMessage(LANG_REP_DELETED, SelectedPlayer->GetName());

	return true;
}

bool ChatHandler::HandleRepInfoCommand(const char* args)
{
	Player* SelectedPlayer = getSelectedPlayer();
	if(!SelectedPlayer)
	{
		SendSysMessage(LANG_NO_CHAR_SELECTED);
        SetSentErrorMessage(true);
        return false;
	}

	uint32 playerguid = SelectedPlayer->GetGUIDLow();
	uint32 paccid = SelectedPlayer->GetSession()->GetAccountId();
	
	QueryResult query1 = CharacterDatabase.PQuery("SELECT points FROM rep_system WHERE account = %u", paccid);

	Field* result = query1->Fetch();
	std::string points = result[0].GetString();

	if(!query1)
		PSendSysMessage(LANG_REP_HASNT_POINTS, SelectedPlayer->GetName());
	else
		PSendSysMessage(LANG_REP_SHOWINFO, SelectedPlayer->GetName(), points.c_str());

	return true;
}

bool ChatHandler::HandleHelpCommand(const char* args)
{
    char* cmd = strtok((char*)args, " ");
    if (!cmd)
    {
        ShowHelpForCommand(getCommandTable(), "help");
        ShowHelpForCommand(getCommandTable(), "");
    }
    else
    {
        if (!ShowHelpForCommand(getCommandTable(), cmd))
            SendSysMessage(LANG_NO_HELP_CMD);
    }

    return true;
}

bool ChatHandler::HandleCommandsCommand(const char* /*args*/)
{
    ShowHelpForCommand(getCommandTable(), "");
    return true;
}

bool ChatHandler::HandleStartCommand(const char* /*args*/)
{
    Player* chr = m_session->GetPlayer();

    if (chr->isInFlight())
    {
        SendSysMessage(LANG_YOU_IN_FLIGHT);
        SetSentErrorMessage(true);
        return false;
    }

    if (chr->isInCombat())
    {
        SendSysMessage(LANG_YOU_IN_COMBAT);
        SetSentErrorMessage(true);
        return false;
    }

    if ((chr->isDead()) || (chr->HasFlag(PLAYER_FLAGS, PLAYER_FLAGS_GHOST)))
    {
    // if player is dead and stuck, send ghost to graveyard
    chr->RepopAtGraveyard();
    return true;
    }

    // cast spell Stuck
    chr->CastSpell(chr, 7355, false);
    return true;
}

bool ChatHandler::HandleServerInfoCommand(const char* /*args*/)
{
    uint32 PlayersNum = sWorld->GetPlayerCount();
    uint32 MaxPlayersNum = sWorld->GetMaxPlayerCount();
    uint32 activeClientsNum = sWorld->GetActiveSessionCount();
    uint32 queuedClientsNum = sWorld->GetQueuedSessionCount();
    uint32 maxActiveClientsNum = sWorld->GetMaxActiveSessionCount();
    uint32 maxQueuedClientsNum = sWorld->GetMaxQueuedSessionCount();
    std::string uptime = secsToTimeString(sWorld->GetUptime());
    uint32 updateTime = sWorld->GetUpdateTime();

    SendSysMessage(_FULLVERSION);
    PSendSysMessage(LANG_CONNECTED_PLAYERS, PlayersNum, MaxPlayersNum);
    PSendSysMessage(LANG_CONNECTED_USERS, activeClientsNum, maxActiveClientsNum, queuedClientsNum, maxQueuedClientsNum);
    PSendSysMessage(LANG_UPTIME, uptime.c_str());
    PSendSysMessage("Update time diff: %u.", updateTime);

    return true;
}

bool ChatHandler::HandleDismountCommand(const char* /*args*/)
{
    //If player is not mounted, so go out :)
    if (!m_session->GetPlayer()->IsMounted())
    {
        SendSysMessage(LANG_CHAR_NON_MOUNTED);
        SetSentErrorMessage(true);
        return false;
    }

    if (m_session->GetPlayer()->isInFlight())
    {
        SendSysMessage(LANG_YOU_IN_FLIGHT);
        SetSentErrorMessage(true);
        return false;
    }

    m_session->GetPlayer()->Unmount();
    m_session->GetPlayer()->RemoveAurasByType(SPELL_AURA_MOUNTED);
    return true;
}

bool ChatHandler::HandleSaveCommand(const char* /*args*/)
{
    Player* player = m_session->GetPlayer();

    // save GM account without delay and output message
    if (!AccountMgr::IsPlayerAccount(m_session->GetSecurity()))
    {
        if (Player* target = getSelectedPlayer())
            target->SaveToDB();
        else
            player->SaveToDB();
        SendSysMessage(LANG_PLAYER_SAVED);
        return true;
    }

    // save if the player has last been saved over 20 seconds ago
    uint32 save_interval = sWorld->getIntConfig(CONFIG_INTERVAL_SAVE);
    if (save_interval == 0 || (save_interval > 20*IN_MILLISECONDS && player->GetSaveTimer() <= save_interval - 20*IN_MILLISECONDS))
        player->SaveToDB();

    return true;
}

/// Display the 'Message of the day' for the realm
bool ChatHandler::HandleServerMotdCommand(const char* /*args*/)
{
    PSendSysMessage(LANG_MOTD_CURRENT, sWorld->GetMotd());
    return true;
}

