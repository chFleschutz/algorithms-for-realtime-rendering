#ifndef KEYBOARDINPUT_H
#define KEYBOARDINPUT_H

#define SORS_MAX_KEYS 10                          /*!< Es werden Maximal 10 gleichzeitige Tastendrücke gespeichert*/
#define SORS_NO_KEY 0                             /*!< Standardwert falls keine Taste gedrückt*/

//! Erlaubt die Abfrage von Tastatureingaben.
class KeyboardInput
{
public:
    KeyboardInput();

    bool isKeyPressed(int key);                 /*!< prüft ob Taste gedrückt wurde -> enum Qt::Key*/
    bool isModifierPressed(int modifier);       /*!< prüft ob Modifier gedrückt wurde -> enum Qt::Modifier*/

    void setKeyDown(int key, int modifier);     /*!< wird beim drücken einer Taste aufgerufen*/
    void setKeyUp(int key, int modifier);       /*!< wird beim loslassen einer Taste aufgerufen*/

private:
    int mKeysDown[SORS_MAX_KEYS];                 /*!< Speicher für Tastendrücke*/
    int mModifier;                              /*!< Flags für Modifier ALt STRG SHIFT*/
};

#endif // KEYBOARDINPUT_H
