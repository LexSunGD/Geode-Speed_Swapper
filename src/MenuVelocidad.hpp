#pragma once
#include <Geode/Geode.hpp>
#include <Geode/ui/TextInput.hpp>

class MenuVelocidad : public geode::Popup<float> {
protected:
    float m_velocidadSeleccionada;
    geode::TextInput* m_inputCustom;
    
    // Lista de botones para poder aplicarles efectos visuales (saber cuál está activo)
    std::vector<CCMenuItemSpriteExtra*> m_botonesVelocidad;

    bool setup(float velocidadActual) override;
    
    // Métodos internos para manejar la lógica
    void actualizarInputTexto(float valor);
    void seleccionarBotonPorVelocidad(float valor);

public:
    static MenuVelocidad* create(float velocidadActual);
    
    // Callbacks
    void onSelectSpeed(CCObject* sender);
    void onTextChanged(std::string const& text);
    void onGuardar(CCObject* sender);
};
