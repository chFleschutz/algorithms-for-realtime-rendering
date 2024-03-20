#include "mouseinput.hpp"

MouseInput::MouseInput() :
    mMousePos(QPoint(0, 0)),
    mMouseButtons(0)
{}

const QPoint& MouseInput::getMousePosition()
{
    return mMousePos;
}

QVector2D MouseInput::getMouseMove()
{
    QVector2D ret(mMousePos - mMouseOldPos);
    mMouseOldPos = mMousePos; // Move ist "verbraucht" - wird 0
    return (ret);
}

bool MouseInput::isMouseButtonPressed(int mouseButton)
{
    return (mMouseButtons & mouseButton) != 0;
}

int MouseInput::getMouseWheelDelta()
{
    int ret = mMouseWheelDelta;
    mMouseWheelDelta = 0;
    return ret;
}

void MouseInput::setMousePosition(const QPoint& pos)
{
    mMouseOldPos = mMousePos;
    mMousePos = pos;
}

void MouseInput::setMouseButtons(int mouseButtons)
{
    mMouseButtons = mouseButtons;
}

void MouseInput::addMouseWheelDelta(int p)
{
    mMouseWheelDelta += p;
}
