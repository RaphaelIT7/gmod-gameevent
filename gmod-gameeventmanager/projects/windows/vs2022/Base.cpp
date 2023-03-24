#include <GarrysMod/Lua/Interface.h>
#include <GarrysMod/FactoryLoader.hpp>
#include "util.h"
#include "GameEventListener.h"


static SourceSDK::FactoryLoader engine_loader("engine");

static IGameEventManager2* gameeventmanager = nullptr;

class CustomGameEventListener : public IGameEventListener2
{
public:
	CustomGameEventListener() = default;

	void FireGameEvent(IGameEvent* event)
	{
		const char* EventName = event->GetName();

		GlobalLUA->PushSpecial(GarrysMod::Lua::SPECIAL_GLOB);
		GlobalLUA->GetField(-1, "hook");
		GlobalLUA->GetField(-1, "Run");
		GlobalLUA->PushString(event->GetName());
		GlobalLUA->CreateTable();
		if (strcmp(EventName, "OnRequestFullUpdate") == 0) {
				GlobalLUA->PushString(event->GetString("networkid"));
				GlobalLUA->SetField(-2, "networkid");
				GlobalLUA->PushString(event->GetString("name"));
				GlobalLUA->SetField(-2, "name");
				GlobalLUA->PushNumber(event->GetInt("userid"));
				GlobalLUA->SetField(-2, "userid");
				GlobalLUA->PushNumber(event->GetInt("index"));
				GlobalLUA->SetField(-2, "index");
		}
		else if (strcmp(EventName, "achievement_earned") == 0) {
			GlobalLUA->PushNumber(event->GetInt("achievement"));
			GlobalLUA->SetField(-2, "achievement");
			GlobalLUA->PushNumber(event->GetInt("player"));
			GlobalLUA->SetField(-2, "player");
		}
		else if (strcmp(EventName, "achievement_event") == 0) {
			GlobalLUA->PushString(event->GetString("achievement_name"));
			GlobalLUA->SetField(-2, "achievement_name");
			GlobalLUA->PushNumber(event->GetInt("cur_val"));
			GlobalLUA->SetField(-2, "cur_val");
			GlobalLUA->PushNumber(event->GetInt("max_val"));
			GlobalLUA->SetField(-2, "max_val");
		}
		else if (strcmp(EventName, "break_breakable") == 0) {
			GlobalLUA->PushString(event->GetString("entindex"));
			GlobalLUA->SetField(-2, "entindex");
			GlobalLUA->PushNumber(event->GetInt("userid"));
			GlobalLUA->SetField(-2, "userid");
			GlobalLUA->PushNumber(event->GetInt("material"));
			GlobalLUA->SetField(-2, "material");
		}
		else if (strcmp(EventName, "break_prop") == 0) {
			GlobalLUA->PushString(event->GetString("entindex"));
			GlobalLUA->SetField(-2, "entindex");
			GlobalLUA->PushNumber(event->GetInt("userid"));
			GlobalLUA->SetField(-2, "userid");
		}
		else if (strcmp(EventName, "client_disconnect") == 0) {
			GlobalLUA->PushString(event->GetString("message"));
			GlobalLUA->SetField(-2, "message");
		}
		else if (strcmp(EventName, "client_beginconnect") == 0) {
			GlobalLUA->PushString(event->GetString("address"));
			GlobalLUA->SetField(-2, "address");
			GlobalLUA->PushNumber(event->GetInt("ip"));
			GlobalLUA->SetField(-2, "ip");
			GlobalLUA->PushNumber(event->GetInt("port"));
			GlobalLUA->SetField(-2, "port");
			GlobalLUA->PushString(event->GetString("source"));
			GlobalLUA->SetField(-2, "source");
		}
		else if (strcmp(EventName, "client_connected") == 0) {
			GlobalLUA->PushString(event->GetString("address"));
			GlobalLUA->SetField(-2, "address");
			GlobalLUA->PushNumber(event->GetInt("ip"));
			GlobalLUA->SetField(-2, "ip");
			GlobalLUA->PushNumber(event->GetInt("port"));
			GlobalLUA->SetField(-2, "port");
		}
		else if (strcmp(EventName, "entity_killed") == 0) {
			GlobalLUA->PushNumber(event->GetInt("entindex_inflictor"));
			GlobalLUA->SetField(-2, "entindex_inflictor");
			GlobalLUA->PushNumber(event->GetInt("entindex_attacker"));
			GlobalLUA->SetField(-2, "entindex_attacker");
			GlobalLUA->PushNumber(event->GetInt("damagebits"));
			GlobalLUA->SetField(-2, "damagebits");
			GlobalLUA->PushNumber(event->GetInt("entindex_killed"));
			GlobalLUA->SetField(-2, "entindex_killed");
		}
		else if (strcmp(EventName, "flare_ignite_npc") == 0) {
			GlobalLUA->PushString(event->GetString("entindex"));
			GlobalLUA->SetField(-2, "entindex");
		}
		else if (strcmp(EventName, "host_quit") == 0) {
		}
		else if (strcmp(EventName, "player_activate") == 0) {
			GlobalLUA->PushNumber(event->GetInt("userid"));
			GlobalLUA->SetField(-2, "userid");
		}
		else if (strcmp(EventName, "player_changename") == 0) {
			GlobalLUA->PushNumber(event->GetInt("userid"));
			GlobalLUA->SetField(-2, "userid");
			GlobalLUA->PushString(event->GetString("oldname"));
			GlobalLUA->SetField(-2, "oldname");
			GlobalLUA->PushString(event->GetString("newname"));
			GlobalLUA->SetField(-2, "newname");
		}
		else if (strcmp(EventName, "player_connect") == 0) {
			GlobalLUA->PushNumber(event->GetInt("bot"));
			GlobalLUA->SetField(-2, "bot");
			GlobalLUA->PushString(event->GetString("networkid"));
			GlobalLUA->SetField(-2, "networkid");
			GlobalLUA->PushString(event->GetString("name"));
			GlobalLUA->SetField(-2, "name");
			GlobalLUA->PushNumber(event->GetInt("userid"));
			GlobalLUA->SetField(-2, "userid");
			GlobalLUA->PushNumber(event->GetInt("index"));
			GlobalLUA->SetField(-2, "index");
			GlobalLUA->PushString(event->GetString("address"));
			GlobalLUA->SetField(-2, "address");
		}
		else if (strcmp(EventName, "player_connect_client") == 0) {
			GlobalLUA->PushNumber(event->GetInt("bot"));
			GlobalLUA->SetField(-2, "bot");
			GlobalLUA->PushString(event->GetString("networkid"));
			GlobalLUA->SetField(-2, "networkid");
			GlobalLUA->PushString(event->GetString("name"));
			GlobalLUA->SetField(-2, "name");
			GlobalLUA->PushNumber(event->GetInt("userid"));
			GlobalLUA->SetField(-2, "userid");
			GlobalLUA->PushNumber(event->GetInt("index"));
			GlobalLUA->SetField(-2, "index");
		}
		else if (strcmp(EventName, "player_disconnect") == 0) {
			GlobalLUA->PushNumber(event->GetInt("bot"));
			GlobalLUA->SetField(-2, "bot");
			GlobalLUA->PushString(event->GetString("networkid"));
			GlobalLUA->SetField(-2, "networkid");
			GlobalLUA->PushString(event->GetString("name"));
			GlobalLUA->SetField(-2, "name");
			GlobalLUA->PushNumber(event->GetInt("userid"));
			GlobalLUA->SetField(-2, "userid");
			GlobalLUA->PushString(event->GetString("reason"));
			GlobalLUA->SetField(-2, "reason");
		}
		else if (strcmp(EventName, "player_hurt") == 0) {
			GlobalLUA->PushNumber(event->GetInt("health"));
			GlobalLUA->SetField(-2, "health");
			GlobalLUA->PushNumber(event->GetInt("userid"));
			GlobalLUA->SetField(-2, "userid");
			GlobalLUA->PushNumber(event->GetInt("attacker"));
			GlobalLUA->SetField(-2, "attacker");
		}
		else if (strcmp(EventName, "player_info") == 0) {
			GlobalLUA->PushNumber(event->GetInt("friendsid"));
			GlobalLUA->SetField(-2, "friendsid");
			GlobalLUA->PushNumber(event->GetInt("index"));
			GlobalLUA->SetField(-2, "index");
			GlobalLUA->PushNumber(event->GetInt("bot"));
			GlobalLUA->SetField(-2, "bot");
			GlobalLUA->PushNumber(event->GetInt("userid"));
			GlobalLUA->SetField(-2, "userid");
			GlobalLUA->PushString(event->GetString("name"));
			GlobalLUA->SetField(-2, "name");
			GlobalLUA->PushString(event->GetString("networkid"));
			GlobalLUA->SetField(-2, "networkid");
		}
		else if (strcmp(EventName, "player_say") == 0) {
			GlobalLUA->PushNumber(event->GetInt("userid"));
			GlobalLUA->SetField(-2, "userid");
			GlobalLUA->PushString(event->GetString("text"));
			GlobalLUA->SetField(-2, "text");
		}
		else if (strcmp(EventName, "player_spawn") == 0) {
			GlobalLUA->PushNumber(event->GetInt("userid"));
			GlobalLUA->SetField(-2, "userid");
		}
		else if (strcmp(EventName, "ragdoll_dissolved") == 0) {
			GlobalLUA->PushNumber(event->GetInt("entindex"));
			GlobalLUA->SetField(-2, "entindex");
		}
		else if (strcmp(EventName, "server_addban") == 0) {
			GlobalLUA->PushString(event->GetString("networkid"));
			GlobalLUA->SetField(-2, "networkid");
			GlobalLUA->PushString(event->GetString("name"));
			GlobalLUA->SetField(-2, "name");
			GlobalLUA->PushNumber(event->GetInt("userid"));
			GlobalLUA->SetField(-2, "userid");
			GlobalLUA->PushString(event->GetString("ip"));
			GlobalLUA->SetField(-2, "ip");
			GlobalLUA->PushString(event->GetString("duration"));
			GlobalLUA->SetField(-2, "duration");
			GlobalLUA->PushString(event->GetString("by"));
			GlobalLUA->SetField(-2, "by");
			GlobalLUA->PushBool(event->GetBool("kicked"));
			GlobalLUA->SetField(-2, "kicked");
		}
		else if (strcmp(EventName, "server_cvar") == 0) {
			GlobalLUA->PushString(event->GetString("cvarname"));
			GlobalLUA->SetField(-2, "cvarname");
			GlobalLUA->PushString(event->GetString("cvarvalue"));
			GlobalLUA->SetField(-2, "cvarvalue");
		}
		else if (strcmp(EventName, "server_removeban") == 0) {
			GlobalLUA->PushString(event->GetString("networkid"));
			GlobalLUA->SetField(-2, "networkid");
			GlobalLUA->PushString(event->GetString("ip"));
			GlobalLUA->SetField(-2, "ip");
			GlobalLUA->PushString(event->GetString("by"));
			GlobalLUA->SetField(-2, "by");
		}
		else if (strcmp(EventName, "server_spawn") == 0) {
			GlobalLUA->PushString(event->GetString("hostname"));
			GlobalLUA->SetField(-2, "hostname");
			GlobalLUA->PushString(event->GetString("address"));
			GlobalLUA->SetField(-2, "address");
			GlobalLUA->PushNumber(event->GetInt("ip"));
			GlobalLUA->SetField(-2, "ip");
			GlobalLUA->PushNumber(event->GetInt("port"));
			GlobalLUA->SetField(-2, "port");
			GlobalLUA->PushString(event->GetString("game"));
			GlobalLUA->SetField(-2, "game");
			GlobalLUA->PushString(event->GetString("mapname"));
			GlobalLUA->SetField(-2, "mapname");
			GlobalLUA->PushNumber(event->GetInt("maxplayers"));
			GlobalLUA->SetField(-2, "maxplayers");
			GlobalLUA->PushString(event->GetString("os"));
			GlobalLUA->SetField(-2, "os");
			GlobalLUA->PushBool(event->GetBool("dedicated"));
			GlobalLUA->SetField(-2, "dedicated");
			GlobalLUA->PushBool(event->GetBool("password"));
			GlobalLUA->SetField(-2, "password");
		}

		GlobalLUA->Call(2, 0);
		GlobalLUA->Pop(2);
	}
};

static CustomGameEventListener* EventListener = new CustomGameEventListener;

LUA_FUNCTION_STATIC(Listen) {
	const char* name = LUA->CheckString(1);
	if (!gameeventmanager->FindListener(EventListener, name)) {
		gameeventmanager->AddListener(EventListener, name, false);
	}

	return 1;
}

GMOD_MODULE_OPEN()
{
	GlobalLUA = LUA;
	gameeventmanager = (IGameEventManager2*)engine_loader.GetFactory()(INTERFACEVERSION_GAMEEVENTSMANAGER2, nullptr);
	if (gameeventmanager == nullptr)
		LUA->ThrowError("unable to initialize IGameEventManager2");

	Start_Table();
		Add_Func(Listen, "Listen");
	Finish_Table("gameevent");

	LuaPrint("[GameEventManager] Added gameevent.Listen");

	return 0;
}

GMOD_MODULE_CLOSE()
{
	return 0;
}