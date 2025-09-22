#pragma once

void GetMagicParams(UINT_PTR* buff, std::uint32_t spellId);
extern decltype(&GetMagicParams) GetMagicParams_org;

void GetWeaponParams(UINT_PTR* buff, std::uint32_t wepId);
extern decltype(&GetWeaponParams) GetWeaponParams_org;