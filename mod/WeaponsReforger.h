#pragma once
#include <random>
#include <basetsd.h>

class WeaponReforger {
public:
	static void Reforge(UINT_PTR qWorldChrMan, UINT_PTR pItemBuffer, UINT_PTR pItemData);
	static int Reforge(int itemID);
private:
	template <typename T>
	static T RandomizeNumber(T dMin, T dMax);
	static inline std::random_device rd;
	static inline std::mt19937 engine{ rd() };
};