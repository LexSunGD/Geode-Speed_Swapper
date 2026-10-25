#include <Geode/Geode.hpp>
#include <Geode/modify/PauseLayer.hpp>

using namespace geode::prelude;

class $modify(MyPauseLayer, PauseLayer) {
    bool init() {
        if (!PauseLayer::init()) return false;

        // 1. Obtener el menú de botones de la izquierda (donde suelen ir los botones extra)
        auto leftMenu = this->getChildByID("left-button-menu");
        if (!leftMenu) return true; // Si no existe, previene un crash

        // 2. Crear el sprite del botón (puedes usar un icono existente del juego)
        auto buttonSprite = CCSprite::createWithSpriteFrameName("GJ_playBtn2_001.png");
        buttonSprite->setScale(0.5f); // Ajustar tamaño ya que el sprite original es grande

        // 3. Crear el botón que llamará a nuestra función personalizada
        auto speedButton = CCMenuItemSpriteExtra::create(
            buttonSprite,
            this,
            menu_selector(MyPauseLayer::onChangeSpeedButtonPressed)
        );

        // 4. Agregar el botón al menú y ordenar los elementos
        leftMenu->addChild(speedButton);
        leftMenu->updateLayout();

        return true;
    }

    // Función que se ejecuta al presionar el botón
    void onChangeSpeedButtonPressed(CCObject* sender) {
        // Obtener la instancia activa de PlayLayer (el nivel en progreso)
        auto playLayer = PlayLayer::get();
        
        if (playLayer) {
            // Modificar la velocidad del Jugador 1
            if (playLayer->m_player1) {
                // 1.6f es la velocidad por defecto (1x). 
                // Puedes cambiarlo a 1.25f (0.5x), 2.1f (2x), 2.5f (3x), 3.1f (4x)
                playLayer->m_player1->m_playerSpeed = 1.6f; 
            }
            
            // Modificar la velocidad del Jugador 2 (Modo Dual)
            if (playLayer->m_player2) {
                playLayer->m_player2->m_playerSpeed = 1.6f;
            }

            // Notificación visual dentro del juego para confirmar el cambio
            Notification::create("¡Velocidad Cambiada!", NotificationIcon::Success)->show();
        } else {
            Notification::create("No estás en un nivel", NotificationIcon::Error)->show();
        }
    }
};
