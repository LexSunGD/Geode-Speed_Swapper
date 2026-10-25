#include <Geode/Geode.hpp>
#include <Geode/modify/PlayerObject.hpp>
#include <Geode/modify/PauseLayer.hpp>
#include <Geode/modify/PlayLayer.hpp>

using namespace geode::prelude;

// Variables globales para controlar el estado del hack de velocidad
bool g_velocidadCustomActiva = false;
float g_ultimaVelocidadNativa = 0.9f;

class $modify(MyPlayerObject, PlayerObject) {
    void update(float dt) {
        // Guardamos la velocidad real impuesta por los portales nativos del nivel
        if (this->m_playerSpeed != 0.0f) {
            // Evaluamos una aproximación para no capturar nuestro propio multiplicador
            if (!g_velocidadCustomActiva) {
                g_ultimaVelocidadNativa = this->m_playerSpeed;
            }
        }

        double multiplicador = 1.0;
        if (g_velocidadCustomActiva) {
            // Lee el número exacto configurado directamente en los Ajustes del Mod (mod.json)
            multiplicador = Mod::get()->getSettingValue<double>("velocidad-configurada");
        }

        PlayerObject::update(dt * static_cast<float>(multiplicador));
    }
};

class $modify(MyPauseLayer, PauseLayer) {
    void customSetup() {
        PauseLayer::customSetup();

        // Usamos NodeIDs para buscar de manera segura el menú izquierdo de la pausa
        auto leftMenu = this->getChildByID("left-button-menu");
        if (!leftMenu) return;

        // Creamos los estados visuales del botón (Activado / Desactivado)
        auto sprOn = CircleButtonSprite::createWithSpriteFrameName("GJ_plusBtn_001.png", 1.0f, CircleBaseColor::Green);
        auto sprOff = CircleButtonSprite::createWithSpriteFrameName("GJ_plusBtn_001.png", 1.0f, CircleBaseColor::DarkGray);

        auto toggler = CCMenuItemToggler::create(
            sprOff, sprOn, this, menu_selector(MyPauseLayer::onToggleVelocidad)
        );
        toggler->setID("speed-swapper-toggle");
        toggler->toggle(g_velocidadCustomActiva);

        leftMenu->addChild(toggler);
        leftMenu->updateLayout();
    }

    void onToggleVelocidad(CCObject* sender) {
        auto toggler = static_cast<CCMenuItemToggler*>(sender);
        // Si el botón pasa a estar activado, encendemos el multiplicador
        g_velocidadCustomActiva = !toggler->isToggled(); 
    }
};

class $modify(MyPlayLayer, PlayLayer) {
    // Evento ejecutado al salir del nivel al menú principal o editor
    void onQuit() {
        PlayLayer::onQuit();
        g_velocidadCustomActiva = false; // Resetea el estado completamente
    }
};
