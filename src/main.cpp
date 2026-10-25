#include <Geode/Geode.hpp>
#include <Geode/modify/PauseLayer.hpp>

using namespace geode::prelude;

class $modify(MyPauseLayer, PauseLayer) {
    // CORRECCIÓN: Ahora acepta el parámetro 'bool unfocused'
    bool init(bool unfocused) {
        // CORRECCIÓN: Se le pasa el parámetro a la función original
        if (!PauseLayer::init(unfocused)) return false;

        // Tu código para agregar el botón aquí abajo...
        auto leftMenu = this->getChildByID("left-button-menu");
        if (!leftMenu) return true;

        auto buttonSprite = CCSprite::createWithSpriteFrameName("GJ_playBtn2_001.png");
        buttonSprite->setScale(0.5f);

        auto speedButton = CCMenuItemSpriteExtra::create(
            buttonSprite,
            this,
            menu_selector(MyPauseLayer::onChangeSpeedButtonPressed)
        );

        leftMenu->addChild(speedButton);
        leftMenu->updateLayout();

        return true;
    }

    void onChangeSpeedButtonPressed(CCObject* sender) {
        auto playLayer = PlayLayer::get();
        if (playLayer) {
            if (playLayer->m_player1) {
                playLayer->m_player1->m_playerSpeed = 1.6f; 
            }
            if (playLayer->m_player2) {
                playLayer->m_player2->m_playerSpeed = 1.6f;
            }
            Notification::create("¡Velocidad Cambiada!", NotificationIcon::Success)->show();
        } else {
            Notification::create("No estás en un nivel", NotificationIcon::Error)->show();
        }
    }
};
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
