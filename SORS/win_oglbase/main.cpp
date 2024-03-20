#include <QtGlobal>
#include <QApplication>
#include <QSurfaceFormat>
#include <QOpenGLContext>
#include <QHBoxLayout>

#include "mainwindow.hpp"
#include "openglwidget.hpp"

int main( int argc, char* argv[] )
{
    // Specify the OpenGL format we wish to use
    // According to the documentation of QOpenGLWidget this should be done
    // before instantiation of QApplication
    QSurfaceFormat format;
    format.setDepthBufferSize(24);

//Unter Windows Verwendung von Desktop OpenGL erzwingen
//sonst bekommt man - warum auch immer - bei manchen Konfigurationen einen Kontext mit OpenGL ES
#if defined(Q_OS_WIN32) || defined(Q_OS_Win64)
    //a.setAttribute(Qt::AA_UseDesktopOpenGL);
#endif

    format.setVersion(4, 0);
    format.setProfile(QSurfaceFormat::CoreProfile);

    QSurfaceFormat::setDefaultFormat(format);

    QApplication a( argc, argv );

    MainWindow lWindow;
    OpenGLWidget lWidget(&lWindow);

    //Programmende erkennen, um Abstürze durch falsche Reihenfolge der Freigabe von
    //OpenGL-Ressourcen zu verhindern.
    QObject::connect(&a, SIGNAL(aboutToQuit()), &lWidget, SLOT(setCleanup()));

    //GLWindow als CentralWidget setzen -> dann bekommt es resize-callbacks
    lWindow.setCentralWidget(&lWidget);
    lWindow.resize(1200,800);
    lWindow.show();
    return a.exec();
}
