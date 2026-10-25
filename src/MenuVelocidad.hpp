#pragma once
#include <Geode/Geode.hpp>
#include <Geode/ui/Popup.hpp>
#include <Geode/ui/TextInput.hpp>

using namespace geode::prelude;

// Heredamos de Popup puro sin plantillas, igual que tu referencia
class MenuVelocidad : public geode::Popup<> {
protected:
    float m_velocidadSeleccionada;
    TextInput* m_inputCustom;
    std::vector<geode::Ref<CCMenuItemSpriteExtra>> m_botonesVelocidad;

    // Inicializador nativo compatible de Geode
    bool init(float velocidadActual);
    void onClose(cocos2d::CCObject* sender) override;

    void actualizarTexto(float valor);
    void manejarOpacidad(float valor);

public:
    static MenuVelocidad* create(float velocidadActual);
};
