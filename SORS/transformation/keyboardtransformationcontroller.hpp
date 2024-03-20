#ifndef KEYBOARDTRANSFORMATIONCONTROLLER_HPP
#define KEYBOARDTRANSFORMATIONCONTROLLER_HPP

#include "thirdparty/entt/single_include/entt/entt.hpp"

#include "movecontroller.hpp"
#include "transformation/transformation.hpp"
#include "input/inputregistry.hpp"

//! Eine Klasse, die ein Entity mit Tastatureingaben steuern kann.
class KeyboardTransformationController: public MoveController
{
public:
    KeyboardTransformationController(entt::entity pEntity = entt::null);

    //! Ist für die Translationskeys ein Modifier (ALT, ...) erforderlich?
    void setTransModifier(int s);

    //! Ist für die Rotationskeys ein Modifier (ALT, ...) erforderlich?
    void setRotModifier(int s);

    //! Setzen der Translationskeys, wobei die Umkehrrichtung immer der passende Großbuchstabe ist
    void setTransKeysUpper(int x, int y, int z);

    //! Setzen der Keys für die Translation
    void setTransKeys(int x1, int x2, int y1, int y2, int z1, int z2);

    //! Setzen der Rotkeys, wobei die Umkehrrichtung immer der passende Großbuchstabe ist
    void setRotKeysUpper(int x, int y, int z);

    //! Setzen des Keys für die Rotation
    void setRotKeys(int x1, int x2, int y1, int y2, int z1, int z2);

    //! Wie schnell (also um welchen Offset) wird jeweils bei Tastendruck verschoben?
    void setTransspeed(float s);

    //! Wie schnell (also um welchen Winkel) wird jeweils bei Tastendruck rotiert
    void setRotspeed(float s);

    //! Callback zum Auswerten des Inputs
    virtual void doControl() override;

    //! Bezeichner für Keys, die man nicht belegen will (Parameter für SetRotKeys, etc.)
    static const int NoKey;

protected:
    entt::entity mEntity{entt::null};
    int mTranskeys[6]{NoKey, NoKey, NoKey, NoKey, NoKey, NoKey};
    int mTransmodifier{0};
    int mRotkeys[6]{NoKey, NoKey, NoKey, NoKey, NoKey, NoKey};
    int mRotmodifier{0};
    void selectandActTrans(Transformation &pTrans, std::shared_ptr<KeyboardInput> pKeyIn);
    void selectandActRot(Transformation &pTrans, std::shared_ptr<KeyboardInput> pKeyIn);
};

#endif // KEYBOARDTRANSFORMATIONCONTROLLER_HPP
