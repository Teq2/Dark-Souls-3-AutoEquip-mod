# Dark Souls 3 auto-equip mod
This modification allows you to automatically equip items you get in-game and upgrade taken weapons on the fly. Weapons will no longer be +0 throughout the game - the modification randomly upgrades them up to the player's maximum, which is obtained by manually reinforcing weapons at the blacksmith. The mod was created to make random runs more fun and even more random. All mod features are customizable and optional.

## Features
- Auto-equip weapons, armor and rings. Any category can be excluded
- Weapons can be automatically upgraded up to player's maximum when picked up
- Weapon can get a random infusion with a certain chance (customizable)
- Weapons stat requirements can be reduced (halving or disabling requirements is supported)
- For even faster runs there is an option to get high weapon upgrades only 

### Details
- Only newly obtained weapons are affected
- Weapons are equipped in the right hand, shields in the left, ranged weapons or magic catalysts may be equipped in the right or left hand depending on the settings
- Shop weapons can also be optionally reinforced after buying
- To increase maximum upgrade level you have to manually reinforce a weapon at blacksmith, you won't receive an upgraded weapon if your maximum is 0 (this is an in-game thing for pvp matchmarking). Special weapons can be upgraded up to 5, reguars up to 10.
- Rings are shifted to the right on equipping, new ring always placed in the leftmost slot

## Installation
The mod is just a DLL (`d3d9.dll`) with a configuration file (`AutoEquipSettings.ini`). Put them in the game directory where `DarkSoulsIII.exe` is located. 

The mod has no UI, to configure it you need to edit `AutoEquipSettings.ini` (it's easy, settings are human-readable). Every feature can be turned off, everything is optional.

To use the mod with "*ModEngine 2*" you can put `d3d9.dll` in  "*ModEngine 2*"  folder where `launchmod_darksouls3.bat` is located, optionally renaming the DLL in any name: `d3d9.dll` -> `myautoequipmod.dll` for example. Just don't forget to edit `config_darksouls3.toml`. If you want the mod to load with "*ModEngine 2*" edit the setting "`with external_dlls`". Example: `with external_dlls = ["myautoequipmod.dll", ]`

## Compatibility
Compatible with the latest version of Dark Souls III on Steam*™* (1.15.2)\
The mod must be compatible with any mod that does not interfere with the main game code and stick to the vanilla item id scheme. You can play with static Item and enemy randomizer, which is the recommended way.

## Credits
- LukeYui for his mod, on which this one was originally based: https://github.com/LukeYui/DS3-Item-Randomiser-OS

### 3rd-party libraries 
- MinHook: https://github.com/TsudaKageyu/minhook
- INIReader: https://github.com/benhoyt/inih