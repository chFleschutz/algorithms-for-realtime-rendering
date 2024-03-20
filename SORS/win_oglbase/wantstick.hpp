#ifndef _WANTSTICK_H_
#define _WANTSTICK_H_

#include <memory>

//! Basisklasse für alle Objekte, die bei idle fuer Animationen gerufen werden wollen
//! Gerufen wird die Methode doIt, in der die ableitenden Klassen für jeden "Tick" ihre Aktionen vornehmen können
//! Wichtig: Die Registrierung bei OpenGLWidget muss jeweils eine Manager-Klasse vornehmen,
//! da die Übergabe von this als shared_ptr im Konstruktur nicht möglich ist
class WantsTick
{
public:
    WantsTick() {}
    virtual ~WantsTick(){}
    virtual void getTick() {}
};

#endif
