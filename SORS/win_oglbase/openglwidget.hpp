#ifndef OPENGLWIDGET_H
#define OPENGLWIDGET_H

#include "opengl.hpp"
#include <QOpenGLWidget>
#include "wantstick.hpp"
#include "framecounter.hpp"

class QOpenGLContext;

//! Ein Qt-Widget, das OpenGL zeichnen kann. Diese Klasse ist die Verbindung zwischen OpenGL und dem Windowing-System,
//! in unserem Fall Qt. Qt kümmert sich um Aufruf der Input-Methoden (z.B. keyPressEvent) und um die Methoden zur Initialisierung
//! (initializeGL()) und Rendering (paintGL()).
class OpenGLWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT

public:
    OpenGLWidget(QWidget* parent = 0, bool pSetMousePointerinWindow = false);
    virtual ~OpenGLWidget();
    //! Event Callbacks für Maus und Tastatur
    //! Diese werden von Qt aufgerufen. In den Methoden erfolgt die Weitergabe an InputRegistry
    virtual void keyPressEvent(QKeyEvent* e) override;
    virtual void keyReleaseEvent(QKeyEvent* event) override;
    virtual void mouseMoveEvent(QMouseEvent* event) override;
    virtual void mousePressEvent(QMouseEvent* event) override;
    virtual void mouseReleaseEvent(QMouseEvent* event) override;
    virtual void wheelEvent(QWheelEvent* e) override;

    //! Instanz des OpenGLWidgets holen, dort kann OpenGL gerendert werden
    static OpenGLWidget* getInstance();
    //! Zugriff auf Mainwindow, um globale Einstellungen zu bekommen, z.B. Fokusverhalten, etc.
    static QWidget *getMainWindow();

    //! OpenGL Kontext zurückgeben. Der Kontext hält übergreifende Einstellungen für OpenGL
    //! Z.B. Bittiefe des Tiefenpuffers, OpenGL Version
    auto getContext() {return context();}

    //! Objekte registrieren, die timergesteuert aufgerufen werden wollen
    void addTickObject(std::shared_ptr<WantsTick> pTickObject);
    void removeTickObject(std::shared_ptr<WantsTick> pTickObject);
    //! Gibt zurück, ob das Widget gerade freigegeben/beendet wird. Siehe setCleanup
    bool getIsInCleanup() const;
private:
    //! Wird von Qt gerufen, bevor der Renderloop (von Qt) gestartet wird
    void initializeGL() override;

protected slots:
    //! Wird von Qt gerufen, wenn sich die Fenstergröße geändert hat
    virtual void resizeGL(int w, int h) override;

    //! Slot, der von Qt angestoßen wird um neu zu zeichnen. Prüft, ob ein passender Kontext erzeugt wurde. Falls ja,
    //! wird das Rendering angestoßen. Falls nein, werden wechselnde Farben angezeigt, um
    //! das Problem zu visualisieren ;-).
    //! Achtung: Mainwindow und Widget sind dafür vorgesehen auf dem Stack angelegt zu werden -> keine Smart Pointer
    virtual void paintGL() override;
    void idle();
    //! Ermöglicht herauszufinden, ob das Widget gerade freigegeben wird. In diesem Fall werden
    //! OpenGL-Ressourcen nicht freigegeben, da die richtige Reihenfolge nicht sinnvoll eingehalten werden kann
    void setCleanup();

private:
    QPoint convertCoord(const QPoint& pPoint);
    static OpenGLWidget* sInstance;
    static QWidget* sMainWindow;
    static QSet<std::shared_ptr<WantsTick>> sTickobjects;
    bool mGLVersionValid;
    bool mIsInCleanup{false};
    FrameCounter mFPS;
    bool mSetMousePointerinWindow{false};
};

#endif // OPENGLWIDGET_H
