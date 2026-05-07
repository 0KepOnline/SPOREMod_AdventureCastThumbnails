#pragma once
#include "Global.h"



class ScenarioCastThumbnailUI : public UTFWin::IWinProc, public DefaultRefCounted
{
public:
	static const uint32_t SPUI = id(PrivateName("UI"));

	static const uint32_t WIN_THUMBNAIL_UI_NORMAL = id(PrivateName("Window_Normal"));
	static const uint32_t WIN_THUMBNAIL_UI_SKINNED = id(PrivateName("Window_Skinned"));
	static const uint32_t WIN_CAST_PREVIEW = 0x76c61c8;
	static const uint32_t WIN_SKINNING = 0x3791003;
	static const uint32_t WIN_SKINNING_DEFAULT_THUMBNAIL = 0x3791001;
	static const uint32_t BTN_SKINNING_REMOVE = id(PrivateName("RemoveSkinning"));
	static const uint32_t SND_SKINNING_REMOVE = id("editor_trash");

	static const uint32_t MSG_UPDATE_CAST_PALETTE = 0x795b639;
	static const uint32_t MSG_SHOW_BEHAVIOR_EDIT_UI = 0x7465518;
	static const uint32_t MSG_HIDE_BEHAVIOR_EDIT_UI = 0x7d11fe0;

	static ScenarioCastThumbnailUI* castThumbnailUI;

	ScenarioCastThumbnailUI();
	~ScenarioCastThumbnailUI();

	int AddRef() override;
	int Release() override;
	void* Cast(uint32_t type) const override;
	int GetEventFlags() const override;
	bool HandleUIMessage(UTFWin::IWindow* win, const UTFWin::Message& msg) override;

	void InitializeUI(UTFWin::IWindow* win, Simulator::cScenarioClass* target, int index);
private:
	UTFWin::IWindow* skinningWinNormal = nullptr;
	UTFWin::IWindow* skinningWinSkinned = nullptr;
	UTFWin::UILayout skinningLayout;
	UTFWin::IButton* skinningRemoveBtn = nullptr;
	Simulator::cScenarioClass* currentTarget = nullptr;
	int currentTargetIndex = -1;

	void UpdateUI(UTFWin::IWindow* win);
	ResourceKey* GetSkinningKey();
	void UpdateDefaultThumbnail();
	void RemoveSkinning();
};
