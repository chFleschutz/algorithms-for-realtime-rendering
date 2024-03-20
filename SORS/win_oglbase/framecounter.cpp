#include <QElapsedTimer>
#include <QTimer>

#include "framecounter.hpp"

FrameCounter::FrameCounter():mFrames(0), mFPS(0)
{
    // This timer drives FPS measuring
    auto timerFPS = new QTimer(this);
    connect(timerFPS, SIGNAL(timeout()), this, SLOT(calcFPS()));
    timerFPS->start(1000);
}

void FrameCounter::count()
{
    mFrames++;
}

void FrameCounter::calcFPS()
{
    static QElapsedTimer lTime;
    static bool start = true;

    if (start)
    {
        lTime.start();
        start = false;
    }
    else if (lTime.elapsed() > 0)
    {
        mFPS = (mFrames * 1000) / lTime.elapsed();
        mFrames = 0;
        lTime.restart();
    }
}

unsigned int FrameCounter::getFPS() const
{
    return mFPS;
}
