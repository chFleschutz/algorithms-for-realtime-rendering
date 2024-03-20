#ifndef FRAMECOUNTER_H
#define FRAMECOUNTER_H

#include "qobject.h"

//! Einfache Klasse, die FPS ermittelt und an Empfänger schicken kann.
class FrameCounter: public QObject
{
    Q_OBJECT
public:
    FrameCounter();
    void count();

    unsigned int getFPS() const;

signals:
    //! Schickt die aktuelle Framerate
    void sigFPS(int);

protected slots:
    void calcFPS();


private:
    unsigned int mFrames;
    unsigned int mFPS;

};

#endif // FRAMECOUNTER_H
