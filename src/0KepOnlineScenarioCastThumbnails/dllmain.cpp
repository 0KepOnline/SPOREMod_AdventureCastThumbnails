// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"
#include "ScenarioCastThumbnailUI.h"

using namespace UTFWin;
using namespace Simulator;

void Initialize() {}

void Dispose() {}

static map<IWindow*, ScenarioCastThumbnailUI*> castThumbnailUIWinMap;

class cScenarioEditModeScriptUI {};

member_detour(cScenarioEditModeScriptUI_ShowBehaviorEditUI, cScenarioEditModeScriptUI, void())
{
	void detoured()
	{
		original_function(this);
		uintptr_t thisPtr = (uintptr_t)this;
		cScenarioDataPtr scenarioData = ScenarioMode.mpData;
		if (!scenarioData)
			return;

		uint32_t index = *(uint32_t*)(thisPtr + 0x20);
		cScenarioClass* scenarioClass = scenarioData->GetClass(index);
		if (!scenarioClass
			|| cScenarioClass::GetObjectType(*(ResourceKey*)scenarioClass) ==
				ScenarioObjectType::ScenarioFixedObjectGameplay)
			return;

		IWindow* activeWin = *(IWindow**)(thisPtr + 0x18);
		if (!activeWin)
			return;
		
		IWindow* castPreviewWin = activeWin->
			FindWindowByID(ScenarioCastThumbnailUI::WIN_CAST_PREVIEW);
		if (!castPreviewWin)
			return;

		ScenarioCastThumbnailUI* castThumbnailUI = nullptr;
		map<IWindow*, ScenarioCastThumbnailUI*>::iterator castThumbnailUIIterator =
			castThumbnailUIWinMap.find(activeWin);
		if (castThumbnailUIIterator != castThumbnailUIWinMap.end())
			castThumbnailUI = castThumbnailUIIterator->second;
		else
		{
			castThumbnailUI = new ScenarioCastThumbnailUI();
			castThumbnailUIWinMap[activeWin] = castThumbnailUI;
		}
		castThumbnailUI->InitializeUI(castPreviewWin, scenarioClass, index);
	}
};

void AttachDetours()
{
	cScenarioEditModeScriptUI_ShowBehaviorEditUI::attach(
		Address(ModAPI::ChooseAddress(0xeb3030, 0xedee10))
	);
}


BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		ModAPI::AddPostInitFunction(Initialize);
		ModAPI::AddDisposeFunction(Dispose);

		PrepareDetours(hModule);
		AttachDetours();
		CommitDetours();
		break;

	case DLL_PROCESS_DETACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
		break;
	}
	return TRUE;
}

