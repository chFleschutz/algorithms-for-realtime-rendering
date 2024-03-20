#ifndef INPUTREGISTRY_H
#define INPUTREGISTRY_H

#include "keyboardinput.hpp"
#include "mouseinput.hpp"

//! Ermöglicht Abfragen von Eingaben aus Keyboard und Tastatur. Typischerweise wird das über Callback-Events realisiert.
//! Für eine Engine ist das aber unpraktisch, da dann die Events an potenziell sehr viele Listener geschickt werden müssen.
//! Zudem müsste jeder Listener beim gleichzeitigen Drücken von mehreren Tasten selbst "Buch führen". Daher hier die Logik
//! über eine Registry, die immer den aktuellen Eingabezustand speichert und von allen Objekten abgefragt werden kann.
class InputRegistry
{
public:
    /*!
     * \brief getInstance
     * Singleton-Abruf, Thread sicher
     * \return Die Instanz der InputRegistry
     */
    static InputRegistry& getInstance();

    /*!
     * \brief getKeyboardInput
     * Erstellt eine KeyboardInput-Instanz, falls nicht vorhanden, und gibt diese zurück
     * \return Eine KeyboardInput-Instanz
     */
    std::shared_ptr<KeyboardInput> getKeyboardInput();

    /*!
     * \brief getMouseInput
     * Erstellt eine MouseInput-Instanz, falls nicht vorhanden, und gibt diese zurück
     * \return Eine MouseInput-Instanz
     */
    std::shared_ptr<MouseInput> getMouseInput();

    bool hasKeyboardInput();              /*!< Prüft, ob eine KeyboardInput-Instanz vorhanden ist */
    bool hasMouseInput();                 /*!< Prüft, ob eine MouseInput-Instanz vorhanden ist */
private:
    explicit InputRegistry();
    virtual ~InputRegistry();

    std::shared_ptr<KeyboardInput> m_keyIn;
    std::shared_ptr<MouseInput> m_mouseIn;
};

#endif // INPUTREGISTRY_H
