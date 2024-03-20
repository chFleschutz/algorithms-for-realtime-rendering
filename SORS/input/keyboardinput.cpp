#include "keyboardinput.hpp"
#include <QDebug>

KeyboardInput::KeyboardInput() :
    mModifier(0)
{
    for (int i = 0; i < SORS_MAX_KEYS; i++)
        mKeysDown[i] = SORS_NO_KEY;
}

bool KeyboardInput::isKeyPressed(int key)
{
    for (int i = 0; i < SORS_MAX_KEYS; i++)
        if (mKeysDown[i] == key)
            return true;
    return false;
}

bool KeyboardInput::isModifierPressed(int modifier)
{
    return (mModifier & modifier) != 0;
}

void KeyboardInput::setKeyDown(int key, int modifier)
{
    // Taste neu eintagen falls nicht vorhanden
    bool alreadyPressed = false;
    for (int i = 0; i < SORS_MAX_KEYS; i++) {
        if (mKeysDown[i] == key) {
            alreadyPressed = true;
            break;
        }
    }

    if (!alreadyPressed) {
        for (int i = 0; i < SORS_MAX_KEYS; i++) {
            if (mKeysDown[i] == SORS_NO_KEY) {
                mKeysDown[i] = key;
                break;
            }
        }
    }

    // Modifier neu eintragen
    mModifier = modifier;
}

void KeyboardInput::setKeyUp(int key, int modifier)
{
    for (int i = 0; i < SORS_MAX_KEYS; i++)
        if (mKeysDown[i] == key)
            mKeysDown[i] = SORS_NO_KEY;
    mModifier = modifier;
}
