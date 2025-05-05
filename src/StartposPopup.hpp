#pragma once

#include <Geode/Geode.hpp>
#include <capeling/TextInputNode.hpp>

class StartposPopup : public geode::Popup<LevelSettingsObject*> {
protected:
    class Toggle : public cocos2d::CCMenu {
    protected:
        cocos2d::CCLabelBMFont* m_label = nullptr;
        CCMenuItemToggler* m_btn = nullptr;
    public:
        static Toggle* create(const char* text, bool state, std::function<void(CCMenuItemToggler*)> callback);

        bool init(const char* text, bool state, std::function<void(CCMenuItemToggler*)> callback);
    };

    LevelSettingsObject* m_settings = nullptr;

    std::array<CCMenuItemSpriteExtra*, 8> m_gamemodes = {};

    std::array<CCMenuItemSpriteExtra*, 5> m_speeds = {};

    std::array<CCNode*, 8> m_toggles = {};

    std::array<CCNode*, 4> m_leftTogglesArr = {};
    std::array<CCNode*, 4> m_rightTogglesArr = {};

    capeling::TextInputNode* m_orderInput = nullptr;
    capeling::TextInputNode* m_channelInput = nullptr;
public:
    static StartposPopup* create(LevelSettingsObject* levelSettings);

    bool setup(LevelSettingsObject* levelSettings) override;

    void updateGamemodes();
    void updateSpeeds();

    CCMenuItemSpriteExtra* createGamemode(const char*, const char*, int);
    CCMenuItemSpriteExtra* createSpeed(const char*, int);
};