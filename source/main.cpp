#include <GarrysMod/FactoryLoader.hpp>
#include <GarrysMod/Lua/Interface.h>
#include "GameEventListener.h"
#ifdef ARCHITECTURE_X86
#include "KeyValues.h"
#else
#include "keyvalues.h"
#endif
#include "util.h"

class CGameEventDescriptor;
class CGameEvent : public IGameEvent
{
public:

	CGameEvent( CGameEventDescriptor *descriptor );
	virtual ~CGameEvent();

	const char *GetName() const;
	bool  IsEmpty(const char *keyName = NULL);
	bool  IsLocal() const;
	bool  IsReliable() const;

	bool  GetBool( const char *keyName = NULL, bool defaultValue = false );
	int   GetInt( const char *keyName = NULL, int defaultValue = 0 );
	float GetFloat( const char *keyName = NULL, float defaultValue = 0.0f );
	const char *GetString( const char *keyName = NULL, const char *defaultValue = "" );

	void SetBool( const char *keyName, bool value );
	void SetInt( const char *keyName, int value );
	void SetFloat( const char *keyName, float value );
	void SetString( const char *keyName, const char *value );
	
	CGameEventDescriptor	*m_pDescriptor;
	KeyValues				*m_pDataKeys;
};

static SourceSDK::FactoryLoader engine_loader("engine");
static IGameEventManager2* eventmanager = nullptr;
void PushEvent(CGameEvent* event)
{
	KeyValues* subkey = event->m_pDataKeys->GetFirstSubKey();
	while (subkey)
	{
		KeyValues::types_t type = subkey->GetDataType();
		if (type == KeyValues::TYPE_STRING)
		{
			GlobalLUA->PushString(event->GetString(subkey->GetName()));
		} else if (type == KeyValues::TYPE_UINT64 || type == KeyValues::TYPE_INT)
		{
			GlobalLUA->PushNumber(event->GetInt(subkey->GetName()));
		} else if (type == KeyValues::TYPE_FLOAT)
		{
			GlobalLUA->PushNumber(event->GetFloat(subkey->GetName()));
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
		PushEvent((CGameEvent*)event);

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
 
	return 0;
}

GMOD_MODULE_OPEN()
{
	GlobalLUA = LUA;
	eventmanager = (IGameEventManager2*)engine_loader.GetFactory()(INTERFACEVERSION_GAMEEVENTSMANAGER2, nullptr);
	if (eventmanager == nullptr)
		LUA->ThrowError("unable to initialize IGameEventManager2");

	Start_Table();
		Add_Func(Listen, "Listen");
	Finish_Table("gameevent");

	LuaPrint("[GameEventManager] Added gameevent.Listen");

	return 0;
}

GMOD_MODULE_CLOSE()
{
	eventmanager->RemoveListener(EventListener);

	return 0;
}