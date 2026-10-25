#include "MenuVelocidad.hpp"

using namespace geode::prelude;

MenuVelocidad* MenuVelocidad::create(float velocidadActual) {
    auto ret = new MenuVelocidad();
    if (ret && ret->initAnchored(280.f, 180.f, velocidadActual)) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

bool MenuVelocidad::setup(float velocidadActual) {
    m_velocidadSeleccionada = velocidadActual;
    this->setTitle("Select Speed");

    auto menu = CCMenu::create();
    menu->setPosition({0.f, 0.f});
    m_mainLayer->addChild(menu);

    std::vector<std::string> sprites = {"speed00_001.png", "speed01_001.png", "speed02_001.png", "speed03_001.png", "speed04_001.png"};
    float startX = 40.f;

    for (size_t i = 0; i < sprites.size(); ++i) {
        auto btn = CCMenuItemSpriteExtra::create(CCSprite::createWithSpriteFrameName(sprites[i].c_str()), this, menu_selector(MenuVelocidad::onSelectSpeed));
        btn->setTag(static_cast<int>(i));
        btn->setPosition({ startX + (i * 50.f), 105.f });
        menu->addChild(btn);
        m_botonesVelocidad.push_back(btn);
    }

    auto label = CCLabelBMFont::create("Custom Speed", "goldFont.fnt");
    label->setScale(0.4f);
    label->setPosition({ 140.f, 60.f });
    m_mainLayer->addChild(label);

    m_inputCustom = TextInput::create(80.f, "Num", "bigFont.fnt");
    m_inputCustom->setPosition({ 140.f, 35.f });
    m_inputCustom->setFilter("0123456789.");
    m_inputCustom->setCallback([this](std::string const& text) {
        if (text.empty()) return;
        try {
            m_velocidadSeleccionada = std::stof(text);
            manejarOpacidad(m_velocidadSeleccionada);
        } catch (...) {}
    });
    m_mainLayer->addChild(m_inputCustom);

    actualizarTexto(m_velocidadSeleccionada);
    manejarOpacidad(m_velocidadSeleccionada);
    return true;
}

void MenuVelocidad::onSelectSpeed(CCObject* sender) {
    std::vector<float> valores = { 0.7f, 0.9f, 1.1f, 1.3f, 1.6f };
    m_velocidadSeleccionada = valores[sender->getTag()];
    actualizarTexto(m_velocidadSeleccionada);
    manejarOpacidad(m_velocidadSeleccionada);
}

void MenuVelocidad::actualizarTexto(float valor) {
    std::stringstream ss;
    ss << std::fixed << std::setprecision(1) << valor;
    m_inputCustom->setString(ss.str());
}

void MenuVelocidad::manejarOpacidad(float valor) {
    std::vector<float> valores = { 0.7f, 0.9f, 1.1f, 1.3f, 1.6f };
    for (size_t i = 0; i < m_botonesVelocidad.size(); ++i) {
        if (std::fabs(valores[i] - valor) < 0.01f) {
            m_botonesVelocidad[i]->setOpacity(255); // Sin opacidad (activo)
        } else {
            m_botonesVelocidad[i]->setOpacity(100); // Con opacidad (inactivo / modo custom)
        }
    }
}

// Ocurre estrictamente al tocar la 'X' para cerrar
void MenuVelocidad::onClose(CCObject* sender) {
    Mod::get()->setSettingValue("multiplicador-velocidad", static_cast<double>(m_velocidadSeleccionada));
    Popup<float>::onClose(sender);
}
