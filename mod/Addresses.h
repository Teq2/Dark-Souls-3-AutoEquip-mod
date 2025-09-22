#pragma once
#include <Windows.h>
#include <winternl.h>

#define DS3_1_15_2

class Addresses
{
public:
	static void Rebase()
	{
		auto newBase = (reinterpret_cast<PPEB>(__readgsqword(0x60)))->Reserved3[1];
		baseDelta = reinterpret_cast<UINT_PTR>(newBase) - ds3ImageBase;
	}

	static UINT_PTR GetSoloParamRepository()
	{
		return gSoloParamRepositoryAddr + baseDelta;
	}

	static UINT_PTR GetLocalPlayer()
	{
		return gLocalPlayerAddr + baseDelta;
	}

	static UINT_PTR GetWorldChrMan()
	{
		return gWorldChrManAddr + baseDelta;
	}

	static UINT_PTR GetItemPickup()
	{
		return itemPickupAddr + baseDelta;
	}

	static UINT_PTR GetItemBuy()
	{
		return itemBuyAddr + baseDelta;
	}

	static UINT_PTR GetEquipItem()
	{
		return equipItemAddr + baseDelta;
	}

	static UINT_PTR GetFindEquippedSlot()
	{
		return findEquippedSlotAddr + baseDelta;
	}

	static UINT_PTR GetGetWeaponParams()
	{
		return getWeaponParamsAddr + baseDelta;
	}

	static UINT_PTR GetGetMagicParams()
	{
		return getMagicParamsAddr + baseDelta;
	}

private:

	static const UINT_PTR ds3ImageBase = 0x140000000;

#ifdef DS3_1_15_2

	// globals
	static const UINT_PTR gSoloParamRepositoryAddr = 0x144798108;
	static const UINT_PTR gLocalPlayerAddr = 0x1447572b8;
	static const UINT_PTR gWorldChrManAddr = 0x14477fdb8;

	// hooks
	static const UINT_PTR itemPickupAddr = 0x1407c4080;
 	static const UINT_PTR itemBuyAddr = 0x140b034f0;
 	static const UINT_PTR getWeaponParamsAddr = 0x140e41670;
 	static const UINT_PTR getMagicParamsAddr = 0x140e32960;

	// callables
	static const UINT_PTR equipItemAddr = 0x140b064e0;
	static const UINT_PTR findEquippedSlotAddr = 0x1405842e0;

#endif // DS3_1_15_2

#ifdef DS3_1_15
	static const UINT_PTR gSoloParamRepositoryAddr = 0x144782838;
	static const UINT_PTR gLocalPlayerAddr = 0x144740178;
	static const UINT_PTR gWorldChrManAddr = 0x144768E78;
	static const UINT_PTR itemPickupAddr = ; // 0x1407BBA80 - near the beginning
	static const UINT_PTR equipItemAddr = 0x140AFBBB0;
	static const UINT_PTR findEquippedSlotAddr = 0x140582850;
	static const UINT_PTR getStrRequirementsAddr = ; // 0x140C073B9 - agi

	// OFF_GETEQUIPPARAMWEAPON 0x140E33420
	// OFF_GETEQUIPPARAMPROTECTOR 0x140E2C980
	// OFF_GETEQUIPPARAMACCESSORY 0x140E16A50
	// OFF_GETEQUIPPARAMGOODS 0x140E22270
#endif

	static inline UINT_PTR baseDelta;
};
