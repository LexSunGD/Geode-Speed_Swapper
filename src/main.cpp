#include <Geode/Geode.hpp>
#include <Geode/modify/PlayerObject.hpp>
#include <Geode/modify/PauseLayer.hpp>
#include "MenuVelocidad.hpp"

using namespace geode::prelude;

class $modify(MyPlayerObject, PlayerObject) {
    void update(float dt) {
        double speedMultiplier = Mod::get()->getSettingValue<double>("multiplicador-velocidad");
        PlayerObject::update(dt * static_cast<float>(speedMultiplier));
    }
};

class $modify(MyPauseLayer, PauseLayer) {
    void customSetup() {
        PauseLayer::customSetup();
        auto rightMenu = this->getChildByID("right-button-menu");
        if (!rightMenu) return;

        auto btn = CCMenuItemSpriteExtra::create(
            CircleButtonSprite::createWithSpriteFrameName("GJ_plusBtn_001.png", 1.0f, CircleBaseColor::Green),
            this, menu_selector(MyPauseLayer::onAbrirMenuVelocidad)
        );
        rightMenu->addChild(btn);
        rightMenu->updateLayout();
    }

    void onAbrirMenuVelocidad(CCObject* sender) {
        float velocidad = 0.9f;
        auto playLayer = PlayLayer::get();
        
        if (playLayer && playLayer->m_player1) {
            float v = playLayer->m_player1->m_playerSpeed;
            if (v < 0.8f) velocidad = 0.7f;
            else if (v < 1.0f) velocidad = 0.9f;
            else if (v < 1.2f) velocidad = 1.1f;
            else if (v < 1.5f) velocidad = 1.3f;
            else velocidad = 1.6f;
        } else {
            velocidad = static_cast<float>(Mod::get()->getSettingValue<double>("multiplicador-velocidad"));
        }

        if (auto popup = MenuVelocidad::create(velocidad)) {
            popup->show();
        }
    }
};
