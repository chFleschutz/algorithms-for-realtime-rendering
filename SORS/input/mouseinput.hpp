#ifndef MOUSEINPUT_H
#define MOUSEINPUT_H

#include <QPoint>
#include <QVector3D>

//! Erlaubt die Abfrage von Mausinteraktionen.
class MouseInput
{
public:
    MouseInput();

    const QPoint& getMousePosition();           /*!< gibt die Mauskoordinaten von der linken oberen Ecke aus zurück*/
    QVector3D getRayFromMousePosition();        /*!< gibt einen 3D Vector im Worldspace der Szene zurück, in Richtung
                                                      der aktuellen Mauskoordinaten*/
    QVector2D getMouseMove();                   /*!< gibt einen Vektor der letzten Mausbewegung zurück*/
    bool isMouseButtonPressed(int mouseButton); /*!< prüft ob Maustaste gedrückt wurde -> enum
                                                      Qt::MouseButtons*/
    int getMouseWheelDelta();                   /*!< gibt die Mausbewegung seit dem letzen Aufruf von getMouseWheelDelta zurück*/

    void setMousePosition(const QPoint& pos);   /*!< wird beim bewegen der Maus und drücken der Maustaste aufgerufen*/
    void setMouseButtons(int mouseButtons);     /*!< wird beim drücken der Maustaste aufgerufen*/
    void addMouseWheelDelta(int p);

private:
    QPoint mMousePos;                           /*!< Speicher für Mausposition*/
    QPoint mMouseOldPos;                        /*!< Speicher für letzte Mausposition, um Bewegungsvektor auszurechnen*/
    int mMouseButtons;                          /*!< Flags für Mouse Buttons*/
    int mMouseWheelDelta;                       /*!< Mausraddrehung in Grad seit letztem Aufruf */
};

#endif // MOUSEINPUT_H
