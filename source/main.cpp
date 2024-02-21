#include <GarrysMod/InterfacePointers.hpp>
#include <GarrysMod/FactoryLoader.hpp>
#include <GarrysMod/Lua/Interface.h>
#include "GameEventListener.h"
#include "KeyValues.h"
#include "Filesystem.h"
#include "utlbuffer.h"
#include "util.h"

static SourceSDK::FactoryLoader engine_loader("engine");
static IGameEventManager2* eventmanager = nullptr;
IFileSystem* g_FileSystem = nullptr;
//KeyValues* pModGameEvents = nullptr;
KeyValues* pGameEvents = nullptr;
void PushEvent(IGameEvent* event)
{
	KeyValues* kv_event = pGameEvents->FindKey(event->GetName());
	if (!kv_event)
	{
		Msg("Invalid Event? (%s)\n", event->GetName());
		return;
	}

	KeyValues* subkey = kv_event->GetFirstSubKey();
	while (subkey)
	{
		const char* type = subkey->GetString();
		if (V_strcmp(type, "string") == 0)
		{
			GlobalLUA->PushString(event->GetString(subkey->GetName()));
		} else if (V_strcmp(type, "long") == 0 || V_strcmp(type, "short") == 0 || V_strcmp(type, "byte") == 0)
		{
			GlobalLUA->PushNumber(event->GetInt(subkey->GetName()));
		} else if (V_strcmp(type, "bool") == 0)
		{
			GlobalLUA->PushBool(event->GetBool(subkey->GetName()));
		} else {
			GlobalLUA->PushNil();
			Msg("Invalid Type?!? (%s -> %s)\n", event->GetName(), subkey->GetName());
		}

		GlobalLUA->SetField(-2, subkey->GetName());

		subkey = subkey->GetNextKey();
	}
}

class CustomGameEventListener : public IGameEventListener2
{
public:
	CustomGameEventListener() = default;

	void FireGameEvent(IGameEvent* event)
	{
		GlobalLUA->PushSpecial(GarrysMod::Lua::SPECIAL_GLOB);
		GlobalLUA->GetField(-1, "hook");
		GlobalLUA->GetField(-1, "Run");
		GlobalLUA->PushString(event->GetName());
		GlobalLUA->CreateTable();
		PushEvent(event);

		GlobalLUA->Call(2, 0);
		GlobalLUA->Pop(2);
	}
};

static CustomGameEventListener* EventListener = new CustomGameEventListener;

LUA_FUNCTION_STATIC(Listen) {
	const char* name = LUA->CheckString(1);
	if (!eventmanager->FindListener(EventListener, name)) {
		eventmanager->AddListener(EventListener, name, false);
	}

	KeyValues* kv_event = pGameEvents->FindKey(name);
	if (!kv_event)
	{
		Msg("Invalid Event? (%s)\n", name);
		return 0;
	}

	return 0;
}

const char* unlisted_events = R"V0G0N(
"otherevents"
{
	"break_breakable"
	{
		"entindex"	"short"
		"userid"	"byte"
		"material"	"long"
	}

	"break_prop"
	{
		"entindex"	"short"
		"userid"	"byte"
	}

	"entity_killed"
	{
		"entindex_inflictor"	"short"
		"entindex_attacker"		"short"
		"damagebits"			"short"
		"entindex_killed"		"byte"
	}

	"flare_ignite_npc"
	{
		"entindex"	"short"
	}

	"player_changename"
	{
		"userid"	"byte"
		"oldname"	"string"
		"newname"	"string"
	}

	"player_hurt"
	{
		"userid"	"byte"
		"health"	"short"
		"attacker"	"byte"
	}

	"player_spawn"
	{
		"userid"	"byte"
	}

	"ragdoll_dissolved"
	{
		"entindex"	"short"
	}

	"game_newmap"
	{
		"mapname"	"string"
	}
}
)V0G0N";
GMOD_MODULE_OPEN()
{
	GlobalLUA = LUA;
	eventmanager = (IGameEventManager2*)engine_loader.GetFactory()(INTERFACEVERSION_GAMEEVENTSMANAGER2, nullptr);
	if (eventmanager == nullptr)
		LUA->ThrowError("unable to initialize IGameEventManager2");

	g_FileSystem = InterfacePointers::FileSystem();

	pGameEvents = new KeyValues("resource/serverevents.res");

	CUtlBuffer buf;
	if (!g_FileSystem->ReadFile("resource/serverevents.res", "GAME", buf))
		LUA->ThrowError("unable to read resource/serverevents.res!");

	pGameEvents = new KeyValues("");
	if (!pGameEvents->LoadFromBuffer("resource/serverevents.res", buf.String()))
	{
		pGameEvents->deleteThis();
		return 0;
	}

	CUtlBuffer modbuf;
	if (!g_FileSystem->ReadFile("resource/modevents.res", "GAME", modbuf))
		LUA->ThrowError("unable to read resource/modevents.res!");

	KeyValues* pModGameEvents = new KeyValues("");
	if (!pModGameEvents->LoadFromBuffer("resource/modevents.res", modbuf.String()))
	{
		pModGameEvents->deleteThis();
		return 0;
	}
	pGameEvents->RecursiveMergeKeyValues(pModGameEvents);

	CUtlBuffer otherbuf;
	otherbuf.PutString(unlisted_events);

	KeyValues* pOtherGameEvents = new KeyValues("");
	if (!pOtherGameEvents->LoadFromBuffer("otherevents", otherbuf.String()))
	{
		pOtherGameEvents->deleteThis();
		return 0;
	}
	pGameEvents->RecursiveMergeKeyValues(pOtherGameEvents);

	Start_Table();
		Add_Func(Listen, "Listen");
	Finish_Table("gameevent");

	LuaPrint("[GameEventManager] Added gameevent.Listen");

	return 0;
}

GMOD_MODULE_CLOSE()
{
	//delete pGameEvents;
	//delete pModGameEvents;

	return 0;
}