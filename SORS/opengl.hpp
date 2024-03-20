#ifndef OPENGL_H
#define OPENGL_H

#include <QtGlobal>
#include <QOpenGLFunctions>

//!Aktuell wird OpenGL 4.0 verwendet und somit eine instanz von
//! QOpenGLFunctions_4_0_Core erstellt.
#include <qopenglfunctions_4_0_core.h>
extern QOpenGLFunctions_4_0_Core* glFunctions;
#define SORSOGLFUNCTIONS QOpenGLFunctions_4_0_Core

#endif // OPENGL_H
