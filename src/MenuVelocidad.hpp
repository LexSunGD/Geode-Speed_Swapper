#pragma once
#include <Geode/Geode.hpp>
#include <Geode/ui/Popup.hpp>
#include <Geode/ui/TextInput.hpp>

// Usamos el preludio de Geode para que reconozca los componentes nativos de Cocos2d como CCMenuItemSpriteExtra
using namespace geode::prelude;

class MenuVelocidad : public Popup<float> {
protected:
    float m_velocidadSeleccionada;
    TextInput* m_inputCustom;
    std::vector<CCMenuItemSpriteExtra*> m_botonesVelocidad;

    // Métodos obligatorios heredados correctamente
    bool setup(float velocidadActual) override;
    void onClose(cocos2d::CCObject* sender) override;

    void actualizarTexto(float valor);
    void manejarOpacidad(float valor);

public:
    static MenuVelocidad* create(float velocidadActual);
    void onSelectSpeed(cocos2d::CCObject* sender);
};
