#pragma once
#include "Global.h"

class ScenarioCastThumbnailUI : public UTFWin::IWinProc
{
public:
	static const uint32_t SPUI_WINDOW = id(PrivateName("Window"));
	static const uint32_t SPUI_BUTTONS = id(PrivateName("Buttons"));

	static const uint32_t WIN_BUTTONS = SPUI_BUTTONS;
	static const uint32_t WIN_BUTTONS_BG = id(PrivateName("Buttons_Background"));
	static const uint32_t WIN_CAST_PREVIEW = 0x76c61c8;
	static const uint32_t WIN_SKINNING = 0x3791003;
	static const uint32_t WIN_SKINNING_DEFAULT_THUMBNAIL = 0x3791001;
	static const uint32_t BTN_SKINNING_REMOVE = id(PrivateName("RemoveSkinning"));
	static const uint32_t SND_SKINNING_REMOVE = id("editor_trash");

	static const uint32_t MSG_UPDATE_CAST_PALETTE = 0x795b639;
	static const uint32_t MSG_SHOW_BEHAVIOR_EDIT_UI = 0x7465518;
	static const uint32_t MSG_HIDE_BEHAVIOR_EDIT_UI = 0x7d11fe0;

	static const Math::Rectangle AREA_SKINNING_BUTTONS_DEFAULT;
	static const Math::Rectangle AREA_SKINNING_BUTTONS_SKINNED;

	static const Math::Rectangle AREA_SKINNING_BUTTONS_CHILD_DEFAULT;
	static const Math::Rectangle AREA_SKINNING_BUTTONS_CHILD_SKINNED;

	int AddRef() override { return 1; }
	int Release() override { return 1; }
	void* Cast(uint32_t type) const override { return nullptr; }
	int GetEventFlags() const override { return UTFWin::kEventFlagBasicInput; }
	bool HandleUIMessage(UTFWin::IWindow* win, const UTFWin::Message& msg) override;

	void InitializeUI(UTFWin::IWindow* win, Simulator::cScenarioClass* target, int index);
private:
	UTFWin::IWindow* skinningWin = nullptr;
	UTFWin::UILayout skinningLayout;
	UTFWin::IWindow* skinningButtonsWin = nullptr;
	UTFWin::UILayout skinningButtonsLayout;
	UTFWin::IButton* skinningRemoveBtn = nullptr;
	Simulator::cScenarioClass* currentTarget = nullptr;
	int currentTargetIndex;

	void UpdateUI();
	void UpdateDefaultThumbnail();
	void RemoveSkinning();
};

