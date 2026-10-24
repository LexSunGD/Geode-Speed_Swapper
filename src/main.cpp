#include <Geode/Geode.hpp>
#include <Geode/modify/PauseLayer.hpp>

using namespace geode::prelude;

class $modify(MyPauseLayer, PauseLayer) {
    // Declaramos un puntero para actualizar el botón si es necesario
    CCMenuItemToggler* m_toggleBtn = nullptr;

    bool init() {
        if (!PauseLayer::init()) return false;

        auto playLayer = PlayLayer::get();
        if (!playLayer) return true;

        // 1. Obtener el estado actual del nivel (si ya es de 2 jugadores o no)
        bool isTwoPlayer = playLayer->m_isTwoPlayerMode;

        // 2. Buscar el menú derecho de la pausa para mantener la estética original
        auto menu = this->getChildByID("right-button-menu");
        if (!menu) {
            menu = CCMenu::create();
            menu->setPosition({0, 0});
            this->addChild(menu);
        }

        // 3. Crear los sprites para el botón circular (On/Off)
        // Usamos círculos verdes y rojos estándar de la interfaz de Geometry Dash
        auto spriteOn = CCSprite::createWithSpriteFrameName("GJ_button_01.png"); // Círculo Verde
        auto iconOn = CCSprite::createWithSpriteFrameName("GJ_editBtn_001.png"); // Ajusta el icono aquí si tienes uno de "2P"
        iconOn->setPosition(spriteOn->getContentSize() / 2);
        spriteOn->addChild(iconOn);

        auto spriteOff = CCSprite::createWithSpriteFrameName("GJ_button_02.png"); // Círculo Rojo
        auto iconOff = CCSprite::createWithSpriteFrameName("GJ_editBtn_001.png");
        iconOff->setPosition(spriteOff->getContentSize() / 2);
        spriteOff->addChild(iconOff);

        // 4. Crear el Toggler (Botón con estado On/Off alternable)
        // El tercer argumento define el estado inicial basado en el nivel
        m_fields->m_toggleBtn = CCMenuItemToggler::create(
            spriteOff, // Estado desactivado (1 jugador)
            spriteOn,  // Estado activado (2 jugadores)
            this,
            menu_selector(MyPauseLayer::onToggleTwoPlayer)
        );

        // Definir visualmente el estado guardado en el nivel
        m_fields->m_toggleBtn->toggle(isTwoPlayer);
        m_fields->m_toggleBtn->setID("toggle-2p-button"_spr);

        // 5. Añadir al menú y ordenar el diseño automáticamente
        menu->addChild(m_fields->m_toggleBtn);
        menu->updateLayout();

        return true;
    }

    void onToggleTwoPlayer(CCObject* sender) {
        auto playLayer = PlayLayer::get();
        if (!playLayer) return;

        // Invertir el estado actual
        bool newState = !playLayer->m_isTwoPlayerMode;
        
        // Aplicar el cambio al gameplay en tiempo real
        playLayer->m_isTwoPlayerMode = newState;

        // Opcional: Mostrar notificación rápida
        std::string message = newState ? "Modo 2 Jugadores Activado" : "Modo 1 Jugador Activado";
        Notification::create(message, NotificationIcon::Info)->show();
    }
};
