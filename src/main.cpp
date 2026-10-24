#include <Geode/Geode.hpp>
#include <Geode/modify/PauseLayer.hpp>

using namespace geode::prelude;

class $modify(PauseLayer) {
    bool init() {
        if (!PauseLayer::init()) return false;

        auto playLayer = PlayLayer::get();
        if (!playLayer) return true;

        // 1. Buscar el menú derecho original de la pausa
        auto menu = this->getChildByID("right-button-menu");
        if (!menu) return true;

        // 2. Crear los sprites circulares (Verde para Activo, Rojo para Desactivo)
        auto spriteOn = CCSprite::createWithSpriteFrameName("GJ_button_01.png"); 
        auto iconOn = CCSprite::createWithSpriteFrameName("GJ_editBtn_001.png");
        iconOn->setPosition(spriteOn->getContentSize() / 2);
        spriteOn->addChild(iconOn);

        auto spriteOff = CCSprite::createWithSpriteFrameName("GJ_button_02.png"); 
        auto iconOff = CCSprite::createWithSpriteFrameName("GJ_editBtn_001.png");
        iconOff->setPosition(spriteOff->getContentSize() / 2);
        spriteOff->addChild(iconOff);

        // 3. Crear el botón interruptor e iniciarlo según el estado del nivel
        auto toggleBtn = CCMenuItemToggler::create(
            spriteOff, 
            spriteOn, 
            this,
            menu_selector(PauseLayer::onToggleTwoPlayer)
        );
        toggleBtn->toggle(playLayer->m_isTwoPlayerMode);
        toggleBtn->setID("toggle-2p-button"_spr);

        // 4. Añadir al menú lateral
        menu->addChild(toggleBtn);
        menu->updateLayout();

        return true;
    }

    // 5. Función ultracorta para alternar el estado
    void onToggleTwoPlayer(CCObject* sender) {
        auto playLayer = PlayLayer::get();
        if (playLayer) {
            playLayer->m_isTwoPlayerMode = !playLayer->m_isTwoPlayerMode;
        }
    }
};
