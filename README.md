This Project adds [gameevent.Listen](https://wiki.facepunch.com/gmod/gameevent.Listen) to the Menu State.  
With this module, you also get access to the [client_beginconnect](https://wiki.facepunch.com/gmod/gameevent/client_beginconnect), [client_connect](https://wiki.facepunch.com/gmod/gameevent/client_connected) and [server_spawn](https://wiki.facepunch.com/gmod/gameevent/server_spawn) gameevents.  

Note: This Project uses [Garry's Mod Common](https://github.com/danielga/garrysmod_common)

## How to Install
1. You need to download the `gmsv_clearcache_win32.dll` from the latest [release](https://github.com/RaphaelIT7/gmod-clearcache/releases)  
2. Move the `gmsv_clearcache_win32.dll` into your `garrysmod/lua/bin` folder.
3. You need insert

Windows:
```lua
if file.Exists("lua/bin/gmsv_gameevent_win32.dll", "GAME") then
  require("gameevent")
end
```

Linux:
```lua
if file.Exists("lua/bin/gmsv_gameevent_linux.dll", "GAME") then
  require("gameevent")
end
```

Into the `garrysmod/lua/menu/menu.lua` file at the bottom.

Note: If you change your Gmod branch or an update is made, your changes could get removed.
