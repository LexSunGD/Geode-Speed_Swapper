#include "MenuVelocidad.hpp"

using namespace geode::prelude;

MenuVelocidad* MenuVelocidad::create(float velocidadActual) {
    auto ret = new MenuVelocidad();
    // Tamaño aproximado de la ventana de tu dibujo (280x200)
    if (ret && ret->initAnchored(280.f, 200.f, velocidadActual)) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

bool MenuVelocidad::setup(float velocidadActual) {
    m_velocidadSeleccionada = velocidadActual;
    this->setTitle("Select Speed");

    auto mainLayer = m_mainLayer;

    // 1. Crear el menú para los botones
    auto menuBotones = CCMenu::create();
    menuBotones->setPosition({0.f, 0.f});
    mainLayer->addChild(menuBotones);

    // Nombres de los sprites de las flechas dentro del juego (RobTop Textures)
    std::vector<std::string> spriteNames = {
        "speed00_001.png", // 0.5x (Naranja <)
        "speed01_001.png", // 1x   (Azul >)
        "speed02_001.png", // 2x   (Verde >>)
        "speed03_001.png", // 3x   (Rosa >>>)
        "speed04_001.png"  // 4x   (Rojo >>>>)
    };

    // Mapeo de Tags a valores flotantes exactos solicitados
    // Tag 0 = 0.7f, Tag 1 = 0.9f, Tag 2 = 1.1f, Tag 3 = 1.3f, Tag 4 = 1.6f
    std::vector<float> valoresVelocidad = { 0.7f, 0.9f, 1.1f, 1.3f, 1.6f };

    float startX = 40.f;
    float spacing = 50.f;

    for (size_t i = 0; i < spriteNames.size(); ++i) {
        auto spr = CCSprite::createWithSpriteFrameName(spriteNames[i].c_str());
        if (!spr) continue;
        
        auto btn = CCMenuItemSpriteExtra::create(
            spr, this, menu_selector(MenuVelocidad::onSelectSpeed)
        );
        btn->setTag(static_cast<int>(i));
        // Posicionar horizontalmente en el centro del popup
        btn->setPosition({ startX + (i * spacing), 110.f });
        
        menuBotones->addChild(btn);
        m_botonesVelocidad.push_back(btn);
    }

    // 2. Crear texto secundario "Custom Speed"
    auto labelCustom = CCLabelBMFont::create("Custom Speed", "goldFont.fnt");
    labelCustom->setScale(0.4f);
    labelCustom->setPosition({ 140.f, 65.f });
    mainLayer->addChild(labelCustom);

    // 3. Crear el Input de Texto (El cuadro "Num" de tu dibujo)
    // Geode provee TextInput que maneja automáticamente las cajas de texto nativas
    m_inputCustom = TextInput::create(80.f, "Num", "bigFont.fnt");
    m_inputCustom->setPosition({ 140.f, 40.f });
    m_inputCustom->setFilter("0123456789."); // Solo permitir números y punto decimal
    
    // Configurar el callback cuando el usuario escribe manualmente
    m_inputCustom->setCallback([this](std::string const& text) {
        this->onTextChanged(text);
    });
    mainLayer->addChild(m_inputCustom);

    // 4. Botón de Guardar / Aplicar
    auto btnSpr = ButtonSprite::create("Apply", "goldFont.fnt", "GJ_button_01.png", 0.7f);
    auto btnGuardar = CCMenuItemSpriteExtra::create(
        btnSpr, this, menu_selector(MenuVelocidad::onGuardar)
    );
    btnGuardar->setPosition({ 140.f, -15.f }); // Abajo, saliéndose ligeramente del contenedor base
    menuBotones->addChild(btnGuardar);

    // Inicializar los estados visuales según la velocidad actual del cubo
    actualizarInputTexto(m_velocidadSeleccionada);
    seleccionarBotonPorVelocidad(m_velocidadSeleccionada);

    return true;
}

// Cuando se presiona una de las flechas de velocidad
void MenuVelocidad::onSelectSpeed(CCObject* sender) {
    int tag = sender->getTag();
    std::vector<float> valoresVelocidad = { 0.7f, 0.9f, 1.1f, 1.3f, 1.6f };
    
    if (tag >= 0 && tag < valoresVelocidad.size()) {
        m_velocidadSeleccionada = valoresVelocidad[tag];
        actualizarInputTexto(m_velocidadSeleccionada);
        seleccionarBotonPorVelocidad(m_velocidadSeleccionada);
    }
}

// Cuando el usuario escribe manualmente en el cuadro numérico
void MenuVelocidad::onTextChanged(std::string const& text) {
    if (text.empty()) return;
    
    try {
        float valor = std::stof(text);
        m_velocidadSeleccionada = valor;
        // Apaga o enciende las flechas si coincide exactamente con una velocidad nativa
        seleccionarBotonPorVelocidad(m_velocidadSeleccionada);
    } catch (...) {
        // Manejar errores si ponen un formato de número inválido
    }
}

// Escribe el número actual en la interfaz
void MenuVelocidad::actualizarInputTexto(float valor) {
    // Formatea el flotante a 1 decimal (ej: 0.9)
    std::stringstream stream;
    stream << std::fixed << std::setprecision(1) << valor;
    m_inputCustom->setString(stream.str());
}

// Resalta visualmente la flecha seleccionada (le da opacidad total y reduce a las demás)
void MenuVelocidad::seleccionarBotonPorVelocidad(float valor) {
    std::vector<float> valoresVelocidad = { 0.7f, 0.9f, 1.1f, 1.3f, 1.6f };
    
    for (size_t i = 0; i < m_botonesVelocidad.size(); ++i) {
        // Si el valor actual es igual al del botón (con margen de error float mínimo)
        if (std::fabs(valoresVelocidad[i] - valor) < 0.01f) {
            m_botonesVelocidad[i]->setOpacity(255); // Totalmente brillante
            m_botonesVelocidad[i]->setScale(1.1f);  // Un poco más grande
        } else {
            m_botonesVelocidad[i]->setOpacity(125); // Opaco / Desactivado
            m_botonesVelocidad[i]->setScale(0.9f);
        }
    }
}

void MenuVelocidad::onGuardar(CCObject* sender) {
    // Guardar definitivamente el valor actual en las configuraciones del Mod
    Mod::get()->setSettingValue("multiplicador-velocidad", static_cast<double>(m_velocidadSeleccionada));
    Notification::create("Speed updated!", NotificationIcon::Success)->show();
    this->onClose(sender);
}
