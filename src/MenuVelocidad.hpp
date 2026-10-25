#pragma once
#include <Geode/Geode.hpp>
#include <Geode/ui/Popup.hpp>
#include <Geode/ui/TextInput.hpp>

class MenuVelocidad : public geode::Popup<float> {
protected:
    float m_velocidadSeleccionada;
    geode::TextInput* m_inputCustom;
    std::vector<cocos2d::CCMenuItemSpriteExtra*> m_botonesVelocidad;

    // Firmas de inicialización nativa de Geode Popup
    bool setup(float velocidadActual) override;
    void onClose(cocos2d::CCObject* sender) override;

    void actualizarTexto(float valor);
    void manejarOpacidad(float valor);

public:
    static MenuVelocidad* create(float velocidadActual);
    void onSelectSpeed(cocos2d::CCObject* sender);
};
