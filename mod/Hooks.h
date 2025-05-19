#pragma once

#include "MinHook/include/MinHook.h"

// The whole purpose of this module is to encapsulate MinHook lib

namespace Hooks
{
    template <typename Target, typename Detour>
    bool SetHook(Target target, Detour detour, Detour* orig)
    {
        // execute once
        static auto libInit = MH_Initialize();

        auto status = MH_CreateHook(reinterpret_cast<LPVOID>(target),
            detour, reinterpret_cast<LPVOID*>(orig));
        if (status == MH_STATUS::MH_OK) {
            if (status = MH_EnableHook(reinterpret_cast<LPVOID>(target));
                status == MH_STATUS::MH_OK) {
                return true;
            }
        }
        return false;
    }

    template <typename Target, typename Detour>
    bool SetHook(Target target, Detour detour)
    {
        if (MH_CreateHook(reinterpret_cast<LPVOID>(target), reinterpret_cast<LPVOID>(detour), 0) != MH_OK) return false;
        if (MH_EnableHook(reinterpret_cast<LPVOID>(target)) != MH_OK) return false;

        return true;
    };
} // namespace Hooks