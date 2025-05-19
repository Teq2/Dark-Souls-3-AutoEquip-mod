
# Dark Souls 3 auto-equip mod
This standalone modification allows you to automatically equip items you get in the game. The mod was created to make random runs more fun and faster. Weapons will no longer be +0 throughout the game - the modification randomly upgrades them up to the previous player's maximum, which was obtained by manual reinforcement from the blacksmith. There is also an option to reduce weapon stat requirements. These features are optional.

## How it works

### Auto-Equip
- You able to exclude armor and rings from auto equipping, if you like Fashion Souls
- Rings are shifted to the right when equipping and only the leftmost ring is replaced
- You can reduce the weapon's requirement to the player's stats (halving or full disable is supported)

### Weapon upgrading
- Weapons can be automatically upgraded up to player's maximum when received
- Weapon can get a random infusion when upgraded
- Shop-items can also be reinforced

To increase maximum upgrade level you have to manually reinforce a weapon at blacksmith, you won't receive an upgraded weapon you your maximum is 0. Special weapons can be upgraded up to 5, reguars up to 10. Optionally you can select mode when you receive more upgraded weapons for even faster runs.

## Installation
The mod is just a DLL (*d3d9.dll*) with a configuration file (*AutoEquipSettings.ini*). Put them in the game directory where DarkSoulsIII.exe is located. 

The mod has no UI, to configure it you need to edit *AutoEquipSettings.ini* (it's easy, settings are human-readable). Every feature can be turned off, everything is optional.

To use the mod with "ModEngine 2" you can put *d3d9.dll* in  "ModEngine 2"  folder where "launchmod_darksouls3.bat" is located, optionally renaming the DLL in any name: "d3d9.dll" -> "myautoequipmod.dll" for example. Just don't forget to edit *config_darksouls3.toml* if you wan't the mod to be loaded with "ModEngine 2" by adding the setting "with external_dlls = ["*myautoequipmod.dll*", ]" to *config_darksouls3.toml*.

## Compatibility
Dark Souls III 1.15.2
The mod must be compatible with any mod that does not interfere with the main game code (file mods). And stick to the vanilla item id scheme. You can play with "DS3 Static Item and Enemy Randomizer", which is the recommended way.

### Credits
- LukeYui for his mod, on which this one was originally based: https://github.com/LukeYui/DS3-Item-Randomiser-OS

### 3rd-party libraries 
- MinHook: https://github.com/TsudaKageyu/minhook
- INIReader: https://github.com/benhoyt/inih