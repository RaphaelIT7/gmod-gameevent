#include "GarrysMod/Lua/Interface.h"
#include "util.h"
#include <string>

GarrysMod::Lua::ILuaBase* GlobalLUA;

void Start_Table() {
	GlobalLUA->PushSpecial(SPECIAL_GLOB);
	GlobalLUA->CreateTable();
}

void Add_Func(CFunc Func, char* Name) {
	GlobalLUA->PushCFunction(Func);
	GlobalLUA->SetField(-2, Name);
}

void Add_Func(CFunc Func, std::string Name) {
	GlobalLUA->PushCFunction(Func);
	GlobalLUA->SetField(-2, Name.c_str());
}

void Finish_Table(char* Name) {
	GlobalLUA->SetField(-2, Name);
	GlobalLUA->Pop();
}

void Finish_Table(std::string Name) {
	GlobalLUA->SetField(-2, Name.c_str());
	GlobalLUA->Pop();
}

// should never be used outside of the main thread
void Call_Func(std::string func) {
	GlobalLUA->PushSpecial(SPECIAL_GLOB);
	GlobalLUA->GetField(-1, func.c_str());
	GlobalLUA->Call(0, 0);
}

// should never be used outside of the main thread
void Call_Func(char* func) {
	GlobalLUA->PushSpecial(SPECIAL_GLOB);
	GlobalLUA->GetField(-1, func);
	GlobalLUA->Call(0, 0);
}


/*
	All Print functions.
*/

// should never be used outside of main thread!!! what happends: memory access violation
void LuaPrint(char* Text) {
	GlobalLUA->PushSpecial(SPECIAL_GLOB);
	GlobalLUA->GetField(-1, "print");
	GlobalLUA->PushString(("[MG Module] " + (std::string)Text).c_str());
	GlobalLUA->Call(1, 0);
}

// should never be used outside of main thread!!! what happends: memory access violation
void LuaPrint(std::string Text) {
	GlobalLUA->PushSpecial(SPECIAL_GLOB);
	GlobalLUA->GetField(-1, "print");
	GlobalLUA->PushString(("[MG Module] " + Text).c_str());
	GlobalLUA->Call(1, 0);
}

void DeveloperPrint(char* Text) {
	ConDMsg(("[MG Module] " + (std::string)Text + "\n").c_str());
}

void DeveloperPrint(std::string Text) {
	ConDMsg(("[MG Module] " + Text + "\n").c_str());
}

void ConsolePrint(char* Text) {
	ConMsg(("[MG Module] " + (std::string)Text + "\n").c_str());
}

void ConsolePrint(std::string Text) {
	ConMsg(("[MG Module] " + Text + "\n").c_str());
}