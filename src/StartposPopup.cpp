#include <StartposPopup.hpp>

StartposPopup* StartposPopup::create(LevelSettingsObject* settings) {
    StartposPopup* ret = new StartposPopup();
    if(ret->initAnchored(440, 310, settings, "GJ_square01.png")) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

bool StartposPopup::setup(LevelSettingsObject* settings) {

    m_settings = settings;

    auto closeSpr = ButtonSprite::create("OK");
    setCloseButtonSpr(closeSpr);
    closeSpr->setScale(0.9f);
    m_closeBtn->m_pfnSelector = menu_selector(StartposPopup::onClose);
    m_closeBtn->setPosition(ccp(m_mainLayer->getContentWidth() / 2, 25));

    auto title = cocos2d::CCLabelBMFont::create("Setup Startpos Command", "bigFont.fnt");
    m_mainLayer->addChildAtPosition(title, geode::Anchor::Top, ccp(.0f, -14.f));
    title->setScale(.5f);

    // Gamemodes
    auto gamemodeLabel = cocos2d::CCLabelBMFont::create("Select Mode:", "goldFont.fnt");
    gamemodeLabel->setScale(0.8f);
    m_mainLayer->addChildAtPosition(gamemodeLabel, geode::Anchor::Center, ccp(0, 100));
    gamemodeLabel->setID("gamemode-label");

    auto gamemodeMenu = cocos2d::CCMenu::create();
    gamemodeMenu->setLayout(geode::RowLayout::create()->setGap(9));
    m_mainLayer->addChildAtPosition(gamemodeMenu, geode::Anchor::Center, ccp(0, 60));

    // Speeds
    auto speedLabel = cocos2d::CCLabelBMFont::create("Select Speed:", "goldFont.fnt");
    speedLabel->setScale(0.8f);
    m_mainLayer->addChildAtPosition(speedLabel, geode::Anchor::Center, ccp(0, 10));
    speedLabel->setID("speed-label");

    auto speedMenu = cocos2d::CCMenu::create();
    speedMenu->setLayout(geode::RowLayout::create()->setGap(10));
    m_mainLayer->addChildAtPosition(speedMenu, geode::Anchor::Center, ccp(0, -40));

    m_gamemodes[0] = createGamemode("gj_iconBtn_on_001.png", "gj_iconBtn_off_001.png", 0);
    m_gamemodes[1] = createGamemode("gj_shipBtn_on_001.png", "gj_shipBtn_off_001.png", 1);
    m_gamemodes[2] = createGamemode("gj_ballBtn_on_001.png", "gj_ballBtn_off_001.png", 2);
    m_gamemodes[3] = createGamemode("gj_birdBtn_on_001.png", "gj_birdBtn_off_001.png", 3);
    m_gamemodes[4] = createGamemode("gj_dartBtn_on_001.png", "gj_dartBtn_off_001.png", 4);
    m_gamemodes[5] = createGamemode("gj_robotBtn_on_001.png", "gj_robotBtn_off_001.png", 5);
    m_gamemodes[6] = createGamemode("gj_spiderBtn_on_001.png", "gj_spiderBtn_off_001.png", 6);
    m_gamemodes[7] = createGamemode("gj_swingBtn_on_001.png", "gj_swingBtn_off_001.png", 7);

    m_speeds[0] = createSpeed("boost_01_001.png", 1);
    m_speeds[1] = createSpeed("boost_02_001.png", 0);
    m_speeds[2] = createSpeed("boost_03_001.png", 2);
    m_speeds[3] = createSpeed("boost_04_001.png", 3);
    m_speeds[4] = createSpeed("boost_05_001.png", 4);

    for(auto gamemode : m_gamemodes) {
        if(!gamemode) continue;
        gamemodeMenu->addChild(gamemode);
    }
    gamemodeMenu->updateLayout();
    gamemodeMenu->setID("gamemode-menu");

    for(auto speed : m_speeds) {
        if(!speed) continue;
        speedMenu->addChild(speed);
    }
    speedMenu->updateLayout();
    speedMenu->setID("speed-menu");

    updateGamemodes();
    updateSpeeds();

    // Toggles
    auto leftTogglesMenu = cocos2d::CCMenu::create();
    leftTogglesMenu->setLayout(geode::ColumnLayout::create()->setGap(15)->setAutoScale(false)->setAxisReverse(true));
    m_mainLayer->addChildAtPosition(leftTogglesMenu, geode::Anchor::Left, ccp(32, 15));

    auto rightTogglesMenu = cocos2d::CCMenu::create();
    rightTogglesMenu->setLayout(geode::ColumnLayout::create()->setGap(15)->setAutoScale(false)->setAxisReverse(true));
    m_mainLayer->addChildAtPosition(rightTogglesMenu, geode::Anchor::Right, ccp(-32, 15));

    m_leftTogglesArr[0] = Toggle::create("Flip", settings->m_isFlipped, [this](CCMenuItemToggler* sender){
        m_settings->m_isFlipped = !m_settings->m_isFlipped;
    });
    m_leftTogglesArr[1] = Toggle::create("Mini", settings->m_startMini, [this](CCMenuItemToggler* sender){
        m_settings->m_startMini = !m_settings->m_startMini;
    });
    m_leftTogglesArr[2] = Toggle::create("Dual", settings->m_startDual, [this](CCMenuItemToggler* sender){
        m_settings->m_startDual = !m_settings->m_startDual;
    });
    m_leftTogglesArr[3] = Toggle::create("Mirror", settings->m_mirrorMode, [this](CCMenuItemToggler* sender){
        m_settings->m_mirrorMode = !m_settings->m_mirrorMode;
    });

    m_rightTogglesArr[0] = Toggle::create("Rotate", settings->m_rotateGameplay, [this](CCMenuItemToggler* sender){
        m_settings->m_rotateGameplay = !m_settings->m_rotateGameplay;
    });
    m_rightTogglesArr[1] = Toggle::create("Reverse", settings->m_reverseGameplay, [this](CCMenuItemToggler* sender){
        m_settings->m_reverseGameplay = !m_settings->m_reverseGameplay;
    });
    m_rightTogglesArr[2] = Toggle::create("Reset Cam", settings->m_resetCamera, [this](CCMenuItemToggler* sender){
        m_settings->m_resetCamera = !m_settings->m_resetCamera;
    });
    m_rightTogglesArr[3] = Toggle::create("Disable", settings->m_disableStartPos, [this](CCMenuItemToggler* sender){
        m_settings->m_disableStartPos = !m_settings->m_disableStartPos;
    });

    for(auto left : m_leftTogglesArr) {
        if(!left) continue;
        leftTogglesMenu->addChild(left);
    }
    leftTogglesMenu->updateLayout();
    leftTogglesMenu->setID("left-toggles-menu");

    for(auto right : m_rightTogglesArr) {
        if(!right) continue;
        rightTogglesMenu->addChild(right);
    }
    rightTogglesMenu->updateLayout();
    rightTogglesMenu->setID("right-toggles-menu");

    auto orderLabel = cocos2d::CCLabelBMFont::create("Target Order", "goldFont.fnt");
    auto channelLabel = cocos2d::CCLabelBMFont::create("Target Channel", "goldFont.fnt");

    orderLabel->setScale(0.6);
    channelLabel->setScale(0.6);

    orderLabel->setID("order-label");
    channelLabel->setID("channel-label");

    m_orderInput = capeling::TextInputNode::create("Order", "bigFont.fnt", 80);
    m_orderInput->addBackground();
    m_orderInput->setAllowedChars("0123456789");
    m_orderInput->m_maxLabelLength = 5;
    if(settings->m_targetOrder != 0) m_orderInput->setString(std::to_string(settings->m_targetOrder));
    m_orderInput->setCallback([this](auto sender, auto str) {
        m_settings->m_targetOrder = std::max(0, geode::utils::numFromString<int>(str.c_str()).unwrapOr(0));
    });
    m_orderInput->setID("order-input");
    
    m_channelInput = capeling::TextInputNode::create("Channel", "bigFont.fnt", 80);
    m_channelInput->addBackground();
    m_channelInput->setAllowedChars("0123456789");
    m_channelInput->m_maxLabelLength = 5;
    if(settings->m_targetChannel != 0) m_channelInput->setString(std::to_string(settings->m_targetChannel));
    m_channelInput->setCallback([this](auto sender, auto str) {
        m_settings->m_targetChannel = std::max(0, geode::utils::numFromString<int>(str.c_str()).unwrapOr(0));
    });
    m_channelInput->setID("channel-input");

    m_mainLayer->addChildAtPosition(orderLabel, geode::Anchor::BottomLeft, ccp(100, 57));
    m_mainLayer->addChildAtPosition(m_orderInput, geode::Anchor::BottomLeft, ccp(100, 30));

    m_mainLayer->addChildAtPosition(channelLabel, geode::Anchor::BottomRight, ccp(-100, 57));
    m_mainLayer->addChildAtPosition(m_channelInput, geode::Anchor::BottomRight, ccp(-100, 30));

    setID("startpos-popup"_spr);

    return true;
}

CCMenuItemSpriteExtra* StartposPopup::createGamemode(const char* spriteOn, const char* spriteOff, int tag) {
    auto ret = geode::cocos::CCMenuItemExt::createSpriteExtraWithFrameName(spriteOff, 0.9f, [this](CCObject* sender){
        m_settings->m_startMode = sender->getTag();
        updateGamemodes();
    });

    ret->setTag(tag);

    auto disabledImage = cocos2d::CCSprite::createWithSpriteFrameName(spriteOn);
    disabledImage->setScale(0.9f);
    
    ret->setDisabledImage(disabledImage);
    return ret;
}

CCMenuItemSpriteExtra* StartposPopup::createSpeed(const char* sprite, int tag) {
    auto normalImage = cocos2d::CCSprite::createWithSpriteFrameName(sprite);
    normalImage->setScale(0.85f);
    normalImage->setOpacity(150);
    normalImage->setColor({ 100, 100, 100 });

    auto ret = geode::cocos::CCMenuItemExt::createSpriteExtra(normalImage, [this](CCObject* sender){
        StartposPopup::m_settings->m_startSpeed = static_cast<Speed>(sender->getTag());
        updateSpeeds();
    });

    ret->setTag(tag);

    auto disabledImage = cocos2d::CCSprite::createWithSpriteFrameName(sprite);
    disabledImage->setScale(0.85f);

    ret->setDisabledImage(disabledImage);
    return ret;
}

void StartposPopup::updateGamemodes() {
    for(auto gamemode : m_gamemodes) {
        if(!gamemode) continue;
        gamemode->setEnabled(m_settings->m_startMode != gamemode->getTag());
    }
}

void StartposPopup::updateSpeeds() {
    for(auto speed : m_speeds) {
        if(!speed) continue;
        speed->setEnabled(static_cast<int>(m_settings->m_startSpeed) != speed->getTag());
    }
}

StartposPopup::Toggle* StartposPopup::Toggle::create(const char *text, bool state, std::function<void (CCMenuItemToggler *)> callback) {
    auto ret = new Toggle();
    if(ret->init(text, state, callback)) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

bool StartposPopup::Toggle::init(const char *text, bool state, std::function<void (CCMenuItemToggler *)> callback) {
    if(!CCMenu::init()) return false;

    setContentSize({35, 35});

    auto btn = geode::cocos::CCMenuItemExt::createTogglerWithStandardSprites(0.8f, callback);
    btn->toggle(state);

    auto label = cocos2d::CCLabelBMFont::create(text, "goldFont.fnt");
    label->limitLabelWidth(55, 0.5f, 0.1f);

    addChildAtPosition(btn, geode::Anchor::Center, ccp(0, 0));
    addChildAtPosition(label, geode::Anchor::Center, ccp(0, 22));

    return true;
}
