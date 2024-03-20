#include "inputregistry.hpp"

InputRegistry::InputRegistry() :
    m_keyIn(nullptr),
    m_mouseIn(nullptr)
{}

InputRegistry::~InputRegistry(){}

InputRegistry& InputRegistry::getInstance()
{
    static InputRegistry instance;
    return instance;
}

std::shared_ptr<KeyboardInput> InputRegistry::getKeyboardInput()
{
    if (!hasKeyboardInput())
        m_keyIn = std::make_shared<KeyboardInput>();
    return m_keyIn;
}

std::shared_ptr<MouseInput> InputRegistry::getMouseInput()
{
    if (!hasMouseInput())
        m_mouseIn = std::make_shared<MouseInput>();
    return m_mouseIn;
}
bool InputRegistry::hasKeyboardInput()
{
    return m_keyIn != nullptr;
}

bool InputRegistry::hasMouseInput()
{
    return m_mouseIn != nullptr;
}
