#ifndef TIMERUNIFORM_HPP
#define TIMERUNIFORM_HPP
#include <QElapsedTimer>
#include "irenderableproperty.hpp"

//! Diese Klasse realisiert einen einfachen Timer, der als RenderableProperty an Renderables
//! angehängt werden kann.
//! Die zugehörigen Shader erhalten dann den Wert von mTime über die Uniform Time
//! mTime bewegt sich in mMsecsPerIteration von 0 nach 1 und beginnt dann wieder von 0
class TimerUniform: public IRenderableProperty
{
public:
    TimerUniform(){mTimer.start();}
    TimerUniform(unsigned pmSecsPerIteration): mMsecsPerIteration(pmSecsPerIteration) {}

    void preRender(Shader &pShader) override {
        auto elapsed = mTimer.elapsed();
        elapsed %= mMsecsPerIteration;

        mTime = abs(elapsed) / static_cast<float>(mMsecsPerIteration);
        pShader.shaderProgram()->setUniformValue("Time", mTime);
    }

protected:
    GLfloat mTime{0.0f};
    QElapsedTimer mTimer;
    unsigned mMsecsPerIteration{1000};
};

#endif // TIMERUNIFORM_HPP
