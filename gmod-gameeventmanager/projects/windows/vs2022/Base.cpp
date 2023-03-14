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

		Msg(event->GetName());
		Msg("\n");
		Msg(event->GetString("message"));
		Msg("\n");
		GlobalLUA->PushSpecial(GarrysMod::Lua::SPECIAL_GLOB);
		GlobalLUA->GetField(-1, "hook");
		GlobalLUA->GetField(-1, "Run");
			GlobalLUA->PushString(event->GetName());
			GlobalLUA->CreateTable();
			GlobalLUA->PushString("Hello World");
			GlobalLUA->SetField(-2, "Test");
			GlobalLUA->SetField(-2, "Baum");
			GlobalLUA->Pop();
		GlobalLUA->Call(2, 0);
		GlobalLUA->Pop(3);
	}
};

LUA_FUNCTION_STATIC(Listen) {
	gameeventmanager->AddListener(new CustomGameEventListener, LUA->CheckString(0), false);

	return 1;
}

GMOD_MODULE_OPEN()
{
	GlobalLUA = LUA;
	gameeventmanager = (IGameEventManager2*)engine_loader.GetFactory()(INTERFACEVERSION_GAMEEVENTSMANAGER2, nullptr);
	if (gameeventmanager == nullptr)
		LUA->ThrowError("unable to initialize IVEngineClient");

	Start_Table();
		Add_Func(Listen, "Listen");
	Finish_Table("gameevent");
	
	gameeventmanager->AddListener(new CustomGameEventListener, "OnRequestFullUpdate", false);
	gameeventmanager->AddListener(new CustomGameEventListener, "host_quit", false);
	gameeventmanager->AddListener(new CustomGameEventListener, "client_fullconnect", false);
	gameeventmanager->AddListener(new CustomGameEventListener, "client_connected", false);
	gameeventmanager->AddListener(new CustomGameEventListener, "client_beginconnect", false);
	gameeventmanager->AddListener(new CustomGameEventListener, "client_disconnect", false);
	gameeventmanager->AddListener(new CustomGameEventListener, "player_say", false);
	gameeventmanager->AddListener(new CustomGameEventListener, "player_activate", false);
	gameeventmanager->AddListener(new CustomGameEventListener, "player_disconnect", false);
	gameeventmanager->AddListener(new CustomGameEventListener, "player_info", false);
	gameeventmanager->AddListener(new CustomGameEventListener, "player_connect_client", false);
	gameeventmanager->AddListener(new CustomGameEventListener, "player_connect", false);

	gameeventmanager->AddListener(new CustomGameEventListener, "player_death", false);
	gameeventmanager->AddListener(new CustomGameEventListener, "player_hurt_ex", false);
	gameeventmanager->AddListener(new CustomGameEventListener, "achievement_earned", false);
	gameeventmanager->AddListener(new CustomGameEventListener, "achievement_event", false);
	gameeventmanager->AddListener(new CustomGameEventListener, "show_freezepanel", false);
	gameeventmanager->AddListener(new CustomGameEventListener, "hide_freezepanel", false);
	gameeventmanager->AddListener(new CustomGameEventListener, "freezecam_started", false);

	LuaPrint("Worked");

	return 0;
}

GMOD_MODULE_CLOSE()
{
	return 0;
}