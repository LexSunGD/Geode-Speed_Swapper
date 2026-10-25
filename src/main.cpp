#include <Geode/Geode.hpp>
#include <Geode/modify/PlayerObject.hpp>
#include <Geode/modify/PauseLayer.hpp>
#include <Geode/modify/PlayLayer.hpp>

using namespace geode::prelude;

// Variables globales para el control de la velocidad física
bool g_velocidadCustomActiva = false;
float g_velocidadOriginalDelNivel = 0.9f;

class $modify(MyPlayerObject, PlayerObject) {
    void update(float dt) {
        // Si el botón está activado, forzamos la velocidad física exacta en cada frame
        if (g_velocidadCustomActiva) {
            double valorConfigurado = Mod::get()->getSettingValue<double>("velocidad-configurada");
            this->m_playerSpeed = static_cast<float>(valorConfigurado);
        }

        // Llamamos a la función nativa del juego pasando el 'dt' intacto sin alterar el tiempo
        PlayerObject::update(dt);
    }
};

class $modify(MyPauseLayer, PauseLayer) {
    void customSetup() {
        PauseLayer::customSetup();

        // Buscamos el contenedor izquierdo seguro con NodeIDs
        auto leftMenu = this->getChildByID("left-button-menu");
        if (!leftMenu) return;

        // Sprites de estado (Verde = Activado, Gris = Desactivado)
        auto sprOn = CircleButtonSprite::createWithSpriteFrameName("GJ_plusBtn_001.png", 1.0f, CircleBaseColor::Green);
        auto sprOff = CircleButtonSprite::createWithSpriteFrameName("GJ_plusBtn_001.png", 1.0f, CircleBaseColor::Gray);

        auto toggler = CCMenuItemToggler::create(
            sprOff, sprOn, this, menu_selector(MyPauseLayer::onToggleVelocidad)
        );
        toggler->setID("speed-swapper-toggle");
        
        // Si ya venía activo de antes (por ejemplo al morir), conserva su estado visual
        toggler->toggle(g_velocidadCustomActiva);

        leftMenu->addChild(toggler);
        leftMenu->updateLayout();
    }

    void onToggleVelocidad(CCObject* sender) {
        auto toggler = static_cast<CCMenuItemToggler*>(sender);
        g_velocidadCustomActiva = !toggler->isToggled(); 

        // Si el usuario desmarca el botón manual en medio del intento, 
        // devolvemos al cubo a la última velocidad base registrada
        if (!g_velocidadCustomActiva) {
            auto playLayer = PlayLayer::get();
            if (playLayer && playLayer->m_player1) {
                playLayer->m_player1->m_playerSpeed = g_velocidadOriginalDelNivel;
            }
        }
    }
};

class $modify(MyPlayLayer, PlayLayer) {
    // Se ejecuta cada vez que el nivel inicia o el jugador muere e inicia un nuevo intento
    bool init(GJGameLevel* level, bool useReplay, bool dontRunActions) {
        if (!PlayLayer::init(level, useReplay, dontRunActions)) return false;
        
        // Guardamos el ritmo nativo inicial del nivel (por defecto x1 = 0.9f)
        if (this->m_player1) {
            g_velocidadOriginalDelNivel = this->m_player1->m_playerSpeed;
        }
        return true;
    }

    // Se ejecuta al presionar el botón de salida al menú
    void onQuit() {
        PlayLayer::onQuit();
        g_velocidadCustomActiva = false; // Limpieza absoluta de estado al salir
    }
};
