#pragma once

float CalcWeaponDamage(DWORD, std::uint32_t, float, std::uint64_t, std::uint64_t, std::uint64_t, float, BYTE);

extern decltype(&CalcWeaponDamage) CalcWeaponDamage_org;