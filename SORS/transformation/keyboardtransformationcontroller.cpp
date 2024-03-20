#include "keyboardtransformationcontroller.hpp"
#include "manager.hpp"
#include "input/inputregistry.hpp"

const int KeyboardTransformationController::NoKey = -1;

KeyboardTransformationController::KeyboardTransformationController(entt::entity pEntity):
    MoveController(), mEntity(pEntity)
{
    if (mEntity == entt::null)
        mEntity = ECS.create();
    Q_UNUSED(ECS.get_or_emplace<Transformation>(mEntity));
}

//! Umso höher der Wert s, umso schneller verschiebt sich das jeweilige Objekt.
//! Da keine weiteren Timing-Klassen implementiert sind, ist das eine einfache
//! Möglichkeit die Geschwindigkeit zu beeinflussen.
void KeyboardTransformationController::setTransspeed(float s)
{
    mMoveSpeed = s;
}

//! Siehe SetTransspeed
void KeyboardTransformationController::setRotspeed(float s)
{
    mRotSpeed = s;
}

//! Angabe aller Codes zum steuern. Übergeben werden in der Regel Buchstaben wie 'a'.
//! Die Parameter sind int, da damit auch die Specialkeys von GLUT behandelt werden
//! können, z.B. ALT-XXX. Siehe hierzu auch SetRotModifier und SetTransModifier
//!\param x1 Verschieben in positiver x-Richtung
//!\param x2 Verschieben in negativer x-Richtung
//!\param y1 Verschieben in positiver y-Richtung
//!\param y2 Verschieben in negativer y-Richtung
//!\param z1 Verschieben in positiver z-Richtung
//!\param z2 Verschieben in negativer z-Richtung
void KeyboardTransformationController::setTransKeys(int x1, int x2, int y1, int y2, int z1, int z2)
{
    mTranskeys[0] = x1;
    mTranskeys[1] = x2;
    mTranskeys[2] = y1;
    mTranskeys[3] = y2;
    mTranskeys[4] = z1;
    mTranskeys[5] = z2;
}

//! Angabe einfacher Buchstaben zum steuern, Großbuchstabe ist Gegenrichtung.
//! D.h. setTransKeysUpper('a','b','c') entspricht dem Aufruf
//! setTransKeys('a','A','b','B','c','C').
void KeyboardTransformationController::setTransKeysUpper(int x, int y, int z)
{
    setTransKeys(x, toupper(x), y, toupper(y), z, toupper(z));
}

//! Angabe aller Codes zum steuern
//!\param x1 Key, um mathematisch positiv um die X-Achse zu drehen
//!\param x2 Key, um mathematisch negativ um die X-Achse zu drehen
//!\param y1 Key, um mathematisch positiv um die y-Achse zu drehen
//!\param y2 Key, um mathematisch negativ um die y-Achse zu drehen
//!\param z1 Key, um mathematisch positiv um die z-Achse zu drehen
//!\param z2 Key, um mathematisch negativ um die z-Achse zu drehen
void KeyboardTransformationController::setRotKeys(int x1, int x2, int y1, int y2, int z1, int z2)
{
    mRotkeys[0] = x1;
    mRotkeys[1] = x2;
    mRotkeys[2] = y1;
    mRotkeys[3] = y2;
    mRotkeys[4] = z1;
    mRotkeys[5] = z2;
}

//! Angabe einfacher Buchstaben zum steuern, Großbuchstabe ist Gegenrichtung
void KeyboardTransformationController::setRotKeysUpper(int x, int y, int z)
{
    setRotKeys(x, toupper(x), y, toupper(y), z, toupper(z));
}

void KeyboardTransformationController::setRotModifier(int s)
{
    mRotmodifier = s;
}

void KeyboardTransformationController::selectandActTrans(Transformation &pTrans, std::shared_ptr<KeyboardInput> pKeyIn)
{
    if ((!mTransmodifier) || pKeyIn->isModifierPressed(mTransmodifier))
    {
        if (pKeyIn->isKeyPressed(mTranskeys[0]))
        {
            pTrans.translate(mMoveSpeed, 0.0, 0.0);
        }
        else if (pKeyIn->isKeyPressed(mTranskeys[1]))
        {
            pTrans.translate(-mMoveSpeed, 0.0, 0.0);
        }
        else if (pKeyIn->isKeyPressed(mTranskeys[2]))
        {
            pTrans.translate(0.0, mMoveSpeed, 0.0);
        }
        else if (pKeyIn->isKeyPressed(mTranskeys[3]))
        {
            pTrans.translate(0.0, -mMoveSpeed, 0.0);
        }
        else if (pKeyIn->isKeyPressed(mTranskeys[4]))
        {
            pTrans.translate(0.0, 0.0, mMoveSpeed);
        }
        else if (pKeyIn->isKeyPressed(mTranskeys[5]))
        {
            pTrans.translate(0.0, 0.0, -mMoveSpeed);
        }
    }
}

void KeyboardTransformationController::selectandActRot(Transformation &pTrans, std::shared_ptr<KeyboardInput> pKeyIn)
{
    if ( (!mRotmodifier) || pKeyIn->isModifierPressed(mRotmodifier))
    {
        if (pKeyIn->isKeyPressed(mRotkeys[0]))
        {
            pTrans.rotate(mRotSpeed, 1.0, 0.0, 0.0);
        }
        else if (pKeyIn->isKeyPressed(mRotkeys[1]))
        {
            pTrans.rotate(-mRotSpeed, 1.0, 0.0, 0.0);
        }
        else if (pKeyIn->isKeyPressed(mRotkeys[2]))
        {
            pTrans.rotate(mRotSpeed, 0.0, 1.0, 0.0);
        }
        else if (pKeyIn->isKeyPressed(mRotkeys[3]))
        {
            pTrans.rotate(-mRotSpeed, 0.0, 1.0, 0.0);
        }
        else if (pKeyIn->isKeyPressed(mRotkeys[4]))
        {
            pTrans.rotate(mRotSpeed, 0.0, 0.0, 1.0);
        }
        else if (pKeyIn->isKeyPressed(mRotkeys[5]))
        {
            pTrans.rotate(-mRotSpeed, 0.0, 0.0, 1.0);
        }
    }
}

void KeyboardTransformationController::doControl()
{
    auto lKeyIn = InputRegistry::getInstance().getKeyboardInput();
    Q_ASSERT(ECS.all_of<Transformation>(mEntity));

    auto &lTrans = ECS.get<Transformation>(mEntity);
    selectandActTrans(lTrans, lKeyIn);
    selectandActRot(lTrans, lKeyIn);
}
