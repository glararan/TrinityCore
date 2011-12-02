#include "Chat.h"
#include "World.h"
#include "WorldSession.h"

class recache_commandscript : public CommandScript
{
public:
	recache_commandscript() : CommandScript("recache_commandscript") { }

	ChatCommand* GetCommands() const
	{
		static ChatCommand recacheCommandTable[] = 
		{
			{ "item",			SEC_ADMINISTRATOR, true,	&HandleReCacheItemCommand,				"ReCache a item or item range for your current target.",				NULL },
			{ "gobject",		SEC_ADMINISTRATOR, true,	&HandleReCacheGameObjectCommand,		"ReCache a gameobject or gameobject range for your current target.",	NULL },
			{ "creature",		SEC_ADMINISTRATOR, true,	&HandleReCacheCreatureCommand,			"ReCache a creature or creature range for your current target.",		NULL },
			{ "playername",		SEC_ADMINISTRATOR, true,	&HandleReCachePlayerNameCommand,		"ReCache your current target name for self.",							NULL },
			{ "itemall",		SEC_ADMINISTRATOR, true,	&HandleReCacheItemForAllCommand,		"ReCache an item or item range for everyone online.",					NULL },
			{ "gobjectall",		SEC_ADMINISTRATOR, true,	&HandleReCacheGameObjectForAllCommand,	"ReCache an gameobject or gameobject range for evryone online.",		NULL },
			{ "creatureall",	SEC_ADMINISTRATOR, true,	&HandleReCacheCreatureForAllCommand,	"ReCache an creature or creature range for everyone online.",			NULL },
			{ "playernameall",	SEC_ADMINISTRATOR, true,	&HandleReCachePlayerNameForAllCommand,	"ReCache your current target's name for everyone online.",				NULL },
			{ NULL,				0,				   false,	NULL,									"",																		NULL }
		};

		static ChatCommand commandTable[] =
		{
			{ "recache",	SEC_ADMINISTRATOR,	true,	NULL,	"",	recacheCommandTable },
			{ NULL,			0,					false,	NULL,	"",	NULL }
		};

		return commandTable;
	}

	static bool HandleReCacheItemCommand(ChatHandler* handler, const char* args)
	{
		if(!args)
			return false;

		if(handler->GetSession() == NULL)
			return false;

		Player* p = handler->getSelectedPlayer();

		if(p = NULL)
			return false;

		uint32 start, end = 0;
		if(sscanf(args, "%u %u", &start, &end) != 2)
			if(sscanf(args, "%u", &start) != 1)
				return false;

		if(end != 0)
		{
			if(start > end)
				return false;

			if(end - start >= 200)
			{
				handler->GetSession()->CustomMessage("You appear to be trying to 'accidently' the server, would you like some help?");
				return true;
			}

			handler->GetSession()->CustomMessage("ReCaching items %u through %u", start, end);
			for(start; start != end;)
			{
				p->GetSession()->ReCacheItemInfo(start);
				start++;
			}
		}
		else
		{
			handler->GetSession()->CustomMessage("ReCaching item %u", start);
			p->GetSession()->ReCacheItemInfo(start);
		}

		return true;
	};

	static bool HandleReCacheCreatureCommand(ChatHandler* handler, const char* args)
	{
		if(!args)
			return false;
	
		if(handler->GetSession() == NULL)
			return false;

		Player* p = handler->getSelectedPlayer();

		if(p == NULL)
			return false;

		uint32 start, end = 0;

		if(sscanf(args, "%u %u", &start, &end) != 2)
			if(sscanf(args, "%u", &start) != 1)
				return false;

		if(end !=0)
		{
			if(start > end)
				return false;

			if(end - start >= 200) //lets limit this
			{
				handler->GetSession()->CustomMessage("You appear to be trying to 'accidently' yourself, would you like some help?");
				return true;
			}

			handler->GetSession()->CustomMessage("Recaching creatures %u through %u", start, end);
			for(start; start != end;)
			{
				p->GetSession()->ReCacheCreatureInfo(start);
				start++;
			}
		}
		else
		{
			handler->GetSession()->CustomMessage("Recaching creature %u", start);
			p->GetSession()->ReCacheCreatureInfo(start);
		}
		return true;
	}

	static bool HandleReCachePlayerNameCommand(ChatHandler* handler, const char* args)
	{
		if(!args)
			return false;

		if(handler->GetSession() == NULL)
			return false;

		Player* p = handler->getSelectedPlayer();
		if(p == NULL)
			return false;

		handler->GetSession()->CustomMessage("Recaching targeted player's name");
		handler->GetSession()->ReCachePlayerName(p->GetGUID());
		return true;
	}

	static bool HandleReCacheGameObjectCommand(ChatHandler* handler, const char* args)
	{
		if(!args)
			return false;

		if(handler->GetSession() == NULL)
			return false;

		Player* p = handler->getSelectedPlayer();
		if(p == NULL)
			return false;

		uint32 start, end = 0;
		if(sscanf(args, "%u %u", &start, &end) != 2)
			if(sscanf(args, "%u", &start) != 1)
				return false;

		if(end !=0)
		{
			if(start > end)
				return false;

			if(end - start >= 200) //lets limit this
			{
				handler->GetSession()->CustomMessage("You appear to be trying to 'accidently' the server, would you like some help?");
				return true;
			}

			handler->GetSession()->CustomMessage("Recaching gameobjects %u through %u", start, end);
			for(start; start != end;)
			{
				p->GetSession()->ReCacheGameObjectInfo(start);
				start++;
			}
		}
		else
		{
			handler->GetSession()->CustomMessage("Recaching gameobject %u", start);
			p->GetSession()->ReCacheGameObjectInfo(start);
		}
		return true;
	}

	static bool HandleReCacheItemForAllCommand(ChatHandler* handler, const char* args)
	{
		if(!args)
			return false;

		if(handler->GetSession() == NULL)
			return false;

		uint32 start, end = 0;
		if(sscanf(args, "%u %u", &start, &end) != 2)
			if(sscanf(args, "%u", &start) != 1)
				return false;

		if(end !=0)
		{
			if(start > end)
				return false;

			if(end - start >= 200) //lets limit this
			{
				handler->GetSession()->CustomMessage("You appear to be trying to 'accidently' the server, would you like some help?");
				return true;
			}

			handler->GetSession()->CustomMessage("Recaching items %u through %u", start, end);
			for(start; start != end;)
			{
				sWorld->ReCacheForAll(1, start);
				start++;
			}
		}
		else
		{
			handler->GetSession()->CustomMessage("Recaching item %u", start);
			sWorld->ReCacheForAll(1, start);
		}
		return true;
	}

	static bool HandleReCacheCreatureForAllCommand(ChatHandler* handler, const char* args)
	{
		if(!args)
			return false;

		if(handler->GetSession() == NULL)
			return false;

		uint32 start, end = 0;
		if(sscanf(args, "%u %u", &start, &end) != 2)
			if(sscanf(args, "%u", &start) != 1)
				return false;

		if(end !=0)
		{
			if(start > end)
				return false;

			if(end - start >= 200) //lets limit this
			{
				handler->GetSession()->CustomMessage("You appear to be trying to 'accidently' yourself, would you like some help?");
				return true;
			}

			handler->GetSession()->CustomMessage("Recaching creatures %u through %u", start, end);
			for(start; start != end;)
			{
				sWorld->ReCacheForAll(3, start);
				start++;
			}
		}
		else
		{
			handler->GetSession()->CustomMessage("Recaching creature %u", start);
			sWorld->ReCacheForAll(3, start);
		}
		return true;
	}

	static bool HandleReCachePlayerNameForAllCommand(ChatHandler* handler, const char* args)
	{
		if(!args)
			return false;

		if(handler->GetSession() == NULL)
			return false;

		Player* p = handler->getSelectedPlayer();
		if(p == NULL)
			return false;

		handler->GetSession()->CustomMessage("Recaching targeted player's name");
		sWorld->ReCacheForAll(2, p->GetGUID());
		return true;
	}

	static bool HandleReCacheGameObjectForAllCommand(ChatHandler* handler, const char* args)
	{
		if(!args)
			return false;

		if(handler->GetSession() == NULL)
			return false;

		uint32 start, end = 0;
		if(sscanf(args, "%u %u", &start, &end) != 2)
			if(sscanf(args, "%u", &start) != 1)
				return false;

		if(end !=0)
		{
			if(start > end)
				return false;

			if(end - start >= 200) //lets limit this
			{
				handler->GetSession()->CustomMessage("You appear to be trying to 'accidently' the server, would you like some help?");
				return true;
			}

			handler->GetSession()->CustomMessage("Recaching gameobjects %u through %u", start, end);
			for(start; start != end;)
			{
				sWorld->ReCacheForAll(4, start);
				start++;
			}
		}
		else
		{
			handler->GetSession()->CustomMessage("Recaching gameobject %u", start);
			sWorld->ReCacheForAll(4, start);
		}
		return true;
	}
};

void AddSC_recache_commandscript()
{
    new recache_commandscript();
}