#include "stdafx.h"
#include "ScenarioCastThumbnailUI.h"

using namespace App;
using namespace Audio;
using namespace Simulator;
using namespace UTFWin;

#define EmptyKey ResourceKey(0, 0, 0)

bool ScenarioCastThumbnailUI::HandleUIMessage(IWindow* win, const Message& msg)
{
	if (msg.eventType != kMsgButtonClick || !currentTarget)
		return false;

	switch (win->GetControlID())
	{
		case BTN_SKINNING_REMOVE:
			RemoveSkinning();
			PlayAudio(SND_SKINNING_REMOVE);
			return true;
		default:
			return false;
	}
}

void ScenarioCastThumbnailUI::InitializeUI(
	IWindow* win,
	cScenarioClass* target,
	int index
)
{
	currentTarget = target;
	currentTargetIndex = index;

	if (!win->FindWindowByID(WIN_SKINNING))
	{
		if (!skinningLayout.LoadByID(SPUI))
			return;
		skinningLayout.SetParentWindow(win);
		skinningWinNormal = skinningLayout.FindWindowByID(WIN_THUMBNAIL_UI_NORMAL);
		skinningWinSkinned = skinningLayout.FindWindowByID(WIN_THUMBNAIL_UI_SKINNED);
		if (!skinningWinNormal || !skinningWinSkinned)
			return;
		skinningRemoveBtn = (IButton*)skinningWinSkinned->
			FindWindowByID(BTN_SKINNING_REMOVE);
		((IWindow*)skinningRemoveBtn)->AddWinProc(this);
	}
	UpdateUI(win);
}

void ScenarioCastThumbnailUI::UpdateUI(IWindow* win)
{
	bool isSkinned = *GetSkinningKey() != EmptyKey;
	if (isSkinned)
		UpdateDefaultThumbnail();
	skinningWinNormal->SetFlag(kWinFlagVisible, !isSkinned);
	skinningWinSkinned->SetFlag(kWinFlagVisible, isSkinned);
}

ResourceKey* ScenarioCastThumbnailUI::GetSkinningKey()
{
	if (!currentTarget)
		return nullptr;
	return &currentTarget->mGameplayObjectGfxOverrideAsset.mKey;
}

void ScenarioCastThumbnailUI::UpdateDefaultThumbnail()
{
	ResourceKey defaultThumbnail;
	CALL(
		Address(ModAPI::ChooseAddress(0xef92f0, 0xf24fe0)),
		void,
		Args(ResourceKey*, ResourceKey*),
		Args(&currentTarget->mAsset.mKey, &defaultThumbnail)
	);
	Image::SetBackgroundByKey(
		skinningWinSkinned->FindWindowByID(WIN_SKINNING_DEFAULT_THUMBNAIL),
		defaultThumbnail
	);
}

void ScenarioCastThumbnailUI::RemoveSkinning()
{
	ResourceKey* skinningKey = GetSkinningKey();
	if (*skinningKey != EmptyKey)
	{
		cScenarioDataPtr scenarioData = ScenarioMode.mpData;
		scenarioData->StartHistoryEntry();
		*skinningKey = EmptyKey;
		currentTarget->mGfxOverrideType = ScenarioGfxOverrideType::Invisible;
		scenarioData->CommitHistoryEntry();

		IMessageManager& messageManager = MessageManager;
		messageManager.MessageSend(
			MSG_UPDATE_CAST_PALETTE,
			(void*)currentTargetIndex,
			nullptr
		);
		messageManager.MessageSend(
			MSG_HIDE_BEHAVIOR_EDIT_UI,
			nullptr,
			nullptr
		);
		messageManager.MessageSend(
			MSG_SHOW_BEHAVIOR_EDIT_UI,
			&currentTargetIndex,
			nullptr
		);
	}
}
