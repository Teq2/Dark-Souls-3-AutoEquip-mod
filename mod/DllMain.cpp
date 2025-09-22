#include <windows.h>
#include "Core.h"
#include <mutex>

extern "C" __declspec(dllexport) DWORD WINAPI D3DPERF_GetStatus(void)
{
	using fnType = decltype(&D3DPERF_GetStatus);
	static fnType orgD3DPERF_GetStatus;
	static std::once_flag init;
	
	std::call_once(init, []() 
		{
			char dllpath[MAX_PATH + 9];
			if (GetSystemDirectory(dllpath, MAX_PATH) < MAX_PATH)
			{
				strcat_s(dllpath, "\\d3d9.dll");
				auto hinstDLL = LoadLibrary(dllpath);

				if (!hinstDLL)
				{
					MessageBoxA(NULL, "Failed to load original DLL (d3d9.dll)", "[AutoEquip - Fatal Error]", MB_ICONERROR);
					ExitProcess(0);
				};
				orgD3DPERF_GetStatus = (fnType)GetProcAddress(hinstDLL, "D3DPERF_GetStatus");
			}
		}
	);

	return orgD3DPERF_GetStatus();
}

static DWORD WINAPI Begin(LPVOID lpParam)
{
	return ModCore::Start(static_cast<HINSTANCE>(lpParam));
};
#include <filesystem>

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
	switch (fdwReason)
	{
	case DLL_PROCESS_ATTACH:
		DisableThreadLibraryCalls(hinstDLL);
		CreateThread(NULL, NULL, Begin, hinstDLL, NULL, NULL);
		break;

	case DLL_PROCESS_DETACH:
		break;
	}

	return TRUE;
}
