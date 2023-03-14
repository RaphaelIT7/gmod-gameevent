#pragma once

#include "GarrysMod/Lua/Interface.h"
#include <string>

using namespace GarrysMod::Lua;

extern GarrysMod::Lua::ILuaBase* GlobalLUA;
extern void Start_Table();

extern void Add_Func(CFunc, char*);
extern void Add_Func(CFunc, std::string);

extern void Call_Func(std::string func);
extern void Call_Func(char* func);

extern void Finish_Table(char*);
extern void Finish_Table(std::string);


// Print functions
extern void LuaPrint(char*);
extern void LuaPrint(std::string);

extern void DeveloperPrint(char* Text);
extern void DeveloperPrint(std::string Text);

extern void ConsolePrint(char* Text);
extern void ConsolePrint(std::string Text);