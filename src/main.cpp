#include <Geode/Geode.hpp>
#include <Geode/modify/PlayerObject.hpp>
#include <Geode/modify/PauseLayer.hpp>
#include "MenuVelocidad.hpp"

using namespace geode::prelude;

// HOOK 1: Aplica la velocidad en las físicas del cubo
class $modify(MyPlayerObject, PlayerObject) {
    void update(float dt) {
        double speedMultiplier = Mod::get()->getSettingValue<double>("multiplicador-velocidad");
        PlayerObject::update(dt * static_cast<float>(speedMultiplier));
    }
};

// HOOK 2: Abre la interfaz desde la pausa detectando la velocidad del cubo
class $modify(MyPauseLayer, PauseLayer) {
    void customSetup() {
        PauseLayer::customSetup();

        auto rightMenu = this->getChildByID("right-button-menu");
        if (!rightMenu) return;

        auto sprite = CircleButtonSprite::createWithSpriteFrameName("GJ_plusBtn_001.png", 1.0f, CircleBaseColor::Green);
        auto btnCambiarVelocidad = CCMenuItemSpriteExtra::create(
            sprite, this, menu_selector(MyPauseLayer::onAbrirMenuVelocidad)
        );

        rightMenu->addChild(btnCambiarVelocidad);
        rightMenu->updateLayout();
    }

    void onAbrirMenuVelocidad(CCObject* sender) {
        float velocidadDetectada = 0.9f; // Por defecto x1 velocidad (0.9f)

        // Intentamos obtener el nivel activo y el objeto del jugador en tiempo real
        auto playLayer = PlayLayer::get();
        if (playLayer && playLayer->m_player1) {
            float nativeSpeed = playLayer->m_player1->m_playerSpeed;

            // Mapeo inverso: Si la velocidad del juego equivale a las internas de RobTop,
            // le pasamos a nuestra interfaz tus valores personalizados.
            // Valores nativos aproximados de RobTop: x0.5=0.7, x1=0.9, x2=1.14, x3=1.39, x4=1.72
            if (nativeSpeed < 0.8f) velocidadDetectada = 0.7f;      // x0.5
            else if (nativeSpeed < 1.0f) velocidadDetectada = 0.9f; // x1
            else if (nativeSpeed < 1.2f) velocidadDetectada = 1.1f; // x2
            else if (nativeSpeed < 1.5f) velocidadDetectada = 1.3f; // x3
            else velocidadDetectada = 1.6f;                         // x4
        } else {
            // Si se abre desde fuera de un nivel, lee el último valor guardado por el Mod
            velocidadDetectada = static_cast<float>(Mod::get()->getSettingValue<double>("multiplicador-velocidad"));
        }

        // Crear la UI enviándole la velocidad autodetectada
        auto popup = MenuVelocidad::create(velocidadDetectada);
        popup->show();
    }
};
