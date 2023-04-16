#pragma once

#include "GarrysMod/Lua/Interface.h"
#include <string>

using namespace GarrysMod::Lua;

extern GarrysMod::Lua::ILuaBase* GlobalLUA;
extern void Start_Table();

extern void Add_Func(CFunc, char*);
extern void Add_Func(CFunc, std::string);

extern void Finish_Table(char*);
extern void Finish_Table(std::string);

extern void LuaPrint(char*);
extern void LuaPrint(std::string);