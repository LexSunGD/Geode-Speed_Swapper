#pragma once
#include <Geode/Geode.hpp>
#include <Geode/ui/Popup.hpp>
#include <Geode/ui/TextInput.hpp>

using namespace geode::prelude;

// Cambiamos a std::string const& para cumplir los requisitos estrictos del SDK de Geode
class MenuVelocidad : public geode::Popup<std::string const&> {
protected:
    float m_velocidadSeleccionada;
    TextInput* m_inputCustom;
    std::vector<geode::Ref<CCMenuItemSpriteExtra>> m_botonesVelocidad;

    // Firmas de inicialización adaptadas
    bool setup(std::string const& value) override;
    void onClose(cocos2d::CCObject* sender) override;

    void actualizarTexto(float valor);
    void manejarOpacidad(float valor);

public:
    static MenuVelocidad* create(float velocidadActual);
};
