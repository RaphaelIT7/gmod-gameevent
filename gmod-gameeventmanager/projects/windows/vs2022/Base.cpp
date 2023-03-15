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

		GlobalLUA->Call(1, 0);
		GlobalLUA->Pop(2);
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

	LuaPrint("[GameEventManager] Added gameevent.Listen");

	return 0;
}

GMOD_MODULE_CLOSE()
{
	return 0;
}