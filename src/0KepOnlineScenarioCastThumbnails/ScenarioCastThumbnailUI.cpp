#include "stdafx.h"
#include "ScenarioCastThumbnailUI.h"

using namespace App;
using namespace Audio;
using namespace Simulator;
using namespace UTFWin;

#define EmptyKey ResourceKey(0, 0, 0)

// TBD: Replace these with two separate windows in SPUI
const Math::Rectangle ScenarioCastThumbnailUI::AREA_SKINNING_BUTTONS_DEFAULT =
	Math::Rectangle(0, -44, 24, -24);
const Math::Rectangle ScenarioCastThumbnailUI::AREA_SKINNING_BUTTONS_SKINNED =
	Math::Rectangle(50, -44, 95, -24);

const Math::Rectangle ScenarioCastThumbnailUI::AREA_SKINNING_BUTTONS_CHILD_DEFAULT =
	Math::Rectangle(0, 0, 24, 20);
const Math::Rectangle ScenarioCastThumbnailUI::AREA_SKINNING_BUTTONS_CHILD_SKINNED =
	Math::Rectangle(0, 0, 45, 20);

bool ScenarioCastThumbnailUI::HandleUIMessage(IWindow* win, const Message& msg)
{
	if (msg.eventType != kMsgButtonClick ||
		!currentTarget)
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

void ScenarioCastThumbnailUI::InitializeUI(IWindow* win, cScenarioClass* target, int index)
{
	currentTarget = target;
	currentTargetIndex = index;

	skinningWin = win->FindWindowByID(WIN_SKINNING);
	if (!skinningWin &&
		skinningLayout.LoadByID(SPUI_WINDOW) &&
		skinningButtonsLayout.LoadByID(SPUI_BUTTONS) &&
		skinningLayout.SetParentWindow(win))
	{
		skinningWin = win->FindWindowByID(WIN_SKINNING);
		skinningButtonsWin = skinningWin->FindWindowByID(WIN_BUTTONS);
		if (!skinningButtonsLayout.SetParentWindow(skinningButtonsWin))
			skinningWin = nullptr;
		else
		{
			skinningRemoveBtn = (IButton*)skinningButtonsWin->
				FindWindowByID(BTN_SKINNING_REMOVE);
			((IWindow*)skinningRemoveBtn)->AddWinProc(this);
		}
	}
	if (skinningWin)
		UpdateUI();
}

void ScenarioCastThumbnailUI::UpdateUI()
{
	IWindow* skinningPreviewWin = skinningWin->FindWindowByID(0x3791006);
	IWindow* skinningRemoveWin = (IWindow*)skinningRemoveBtn;
	if (currentTarget->mGameplayObjectGfxOverrideAsset.mKey == EmptyKey)
	{
		if (skinningPreviewWin->GetFlags() & kWinFlagVisible)
		{
			skinningPreviewWin->SetFlag(kWinFlagVisible, false);
			skinningRemoveWin->SetFlag(kWinFlagVisible, false);
		}
		skinningButtonsWin->
			SetArea(AREA_SKINNING_BUTTONS_DEFAULT);
		skinningRemoveWin->GetParent()->
			SetArea(AREA_SKINNING_BUTTONS_CHILD_DEFAULT);
		skinningButtonsWin->FindWindowByID(WIN_BUTTONS_BG)->
			SetArea(AREA_SKINNING_BUTTONS_CHILD_DEFAULT);
	}
	else
	{
		UpdateDefaultThumbnail();
		if (!(skinningPreviewWin->GetFlags() & kWinFlagVisible))
		{
			skinningPreviewWin->SetFlag(kWinFlagVisible, true);
			skinningRemoveWin->SetFlag(kWinFlagVisible, true);
		}
		skinningButtonsWin->
			SetArea(AREA_SKINNING_BUTTONS_SKINNED);
		skinningRemoveWin->GetParent()->
			SetArea(AREA_SKINNING_BUTTONS_CHILD_SKINNED);
		skinningButtonsWin->FindWindowByID(WIN_BUTTONS_BG)->
			SetArea(AREA_SKINNING_BUTTONS_CHILD_SKINNED);
	}
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
		skinningWin->FindWindowByID(WIN_SKINNING_DEFAULT_THUMBNAIL),
		defaultThumbnail
	);
}

void ScenarioCastThumbnailUI::RemoveSkinning()
{
	ResourceKey& skinningKey = currentTarget->mGameplayObjectGfxOverrideAsset.mKey;
	if (skinningKey != EmptyKey)
	{
		cScenarioDataPtr scenarioData = ScenarioMode.mpData;
		scenarioData->StartHistoryEntry();
		skinningKey = EmptyKey;
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
