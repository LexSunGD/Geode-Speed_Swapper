#include <Geode/Geode.hpp>
#include <Geode/modify/PauseLayer.hpp>

using namespace geode::prelude;

class $modify(MyPauseLayer, PauseLayer) {
    bool init() {
        if (!PauseLayer::init()) return false;

        // 1. Obtener el menú de botones derecho o izquierdo (usualmente el de la derecha tiene espacio)
        // O puedes crear un nuevo CCMenu y añadirlo a la capa
        auto menu = this->getChildByID("right-button-menu");
        if (!menu) {
            // Si no se encuentra por ID, usamos el método alternativo creando un menú propio
            menu = CCMenu::create();
            menu->setPosition({0, 0});
            this->addChild(menu);
        }

        // 2. Crear el sprite del botón (puedes usar texturas del juego como "GJ_editBtn_001.png" o texto)
        auto buttonSprite = CCSprite::createWithSpriteFrameName("GJ_editBtn_001.png");
        
        // 3. Crear el botón y asignarle la función (callback)
        auto toggle2PBtn = CCMenuItemSpriteExtra::create(
            buttonSprite,
            this,
            menu_selector(MyPauseLayer::onToggleTwoPlayer)
        );

        // 4. Posicionar el botón de forma segura si creaste un menú propio
        // Si lo añadiste a "right-button-menu", puedes usar menu->addChild(toggle2PBtn); y luego menu->updateLayout();
        toggle2PBtn->setPosition({100, 100}); // Ajusta las coordenadas según prefieras
        toggle2PBtn->setID("toggle-2p-button"_spr);

        menu->addChild(toggle2PBtn);

        return true;
    }

    void onToggleTwoPlayer(CCObject* sender) {
        // 5. Acceder al PlayLayer actual (la capa del nivel en ejecución)
        auto playLayer = PlayLayer::get();
        if (!playLayer) return;

        // 6. Cambiar el estado de la variable de dos jugadores
        // m_levelSettings guarda la configuración inicial, pero m_isTwoPlayerMode maneja las físicas en tiempo real
        bool currentState = playLayer->m_isTwoPlayerMode;
        playLayer->m_isTwoPlayerMode = !currentState;

        // 7. Mostrar una notificación en pantalla para avisar al usuario
        std::string message = "Modo 2 Jugadores: " + std::string(!currentState ? "ACTIVADO" : "DESACTIVADO");
        Notification::create(message, NotificationIcon::Info)->show();
    }
};
