// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"
#include "ScenarioCastThumbnailUI.h"

using namespace App;
using namespace UTFWin;
using namespace Simulator;

class cScenarioEditModeScriptUI {};



void Initialize() {}

void Dispose() {}


member_detour(cScenarioEditModeScriptUI_ShowBehaviorEditUI, cScenarioEditModeScriptUI, void())
{
    void detoured()
    {
        original_function(this);
        uintptr_t thisPtr = (uintptr_t)this;
        cScenarioDataPtr scenarioData = ScenarioMode.mpData;
        if (!scenarioData)
            return;

        uint32_t index = (uint32_t)field(thisPtr, 0x20);
        cScenarioClass* scenarioClass = scenarioData->GetClass(index);
        if (!scenarioClass
            || cScenarioClass::GetObjectType(*(ResourceKey*)scenarioClass) ==
                ScenarioObjectType::ScenarioFixedObjectGameplay)
            return;

        IWindow* behaviorEditUIWin = (IWindow*)field(thisPtr, 0x18);
        if (!behaviorEditUIWin)
            return;
        
        IWindow* castPreviewWin = behaviorEditUIWin->
            FindWindowByID(ScenarioCastThumbnailUI::WIN_CAST_PREVIEW);
        if (!castPreviewWin)
            return;

        ScenarioCastThumbnailUI* castThumbnailUI = ScenarioCastThumbnailUI::Get();
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

