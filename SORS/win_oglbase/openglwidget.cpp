#include <QOpenGLVersionFunctionsFactory>
#include <QOpenGLContext>
#include <QPoint>
#include <QApplication>
#include <QKeyEvent>
#include <QElapsedTimer>
#include <QTimer>
#include <QMainWindow>

#include "opengl.hpp"
#include "openglwidget.hpp"
#include "input/inputregistry.hpp"
#include "manager.hpp"
#include "system.hpp"

SORSOGLFUNCTIONS *glFunctions = NULL;

// statische Klassenvariablen von Window initialisieren
OpenGLWidget* OpenGLWidget::sInstance = nullptr;
QWidget* OpenGLWidget::sMainWindow = nullptr;
QSet<std::shared_ptr<WantsTick>> OpenGLWidget::sTickobjects;

OpenGLWidget::OpenGLWidget(QWidget *parent, bool pSetMousePointerinWindow)
    : QOpenGLWidget(parent), mSetMousePointerinWindow(pSetMousePointerinWindow)
{
    //Note that this constructor should not contain any calls to OpenGL resources
    sInstance = this;
    mGLVersionValid = false;

    qDebug() << "Widget instantiated." << this;

    // This timer drives "idle" updates for animations
    QTimer* timerI = new QTimer(this);
    connect(timerI, SIGNAL(timeout()), this, SLOT(idle()));
    timerI->start(1);

    // This timer drives the scene updates
    QTimer* timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    timer->start(1);

    // Find Mainwindow - otherwise have to get parent widgets as parameter through whole hierarchy
    if (OpenGLWidget::sMainWindow == NULL)
    {
        foreach (QWidget* widget, qApp->topLevelWidgets())
        {
            if (widget->inherits("QMainWindow"))
            {
                OpenGLWidget::sMainWindow = widget;
            }
        }
    }
    //Gesamten Platz des Fensters belegen
    this->resize(sMainWindow->width(), sMainWindow->height());

    //Notwendig, um per Default keyEvents zu erhalten
    setFocusPolicy(Qt::StrongFocus);
}

OpenGLWidget::~OpenGLWidget()
{
}

void OpenGLWidget::initializeGL()
{
    qDebug() << "Initializing OpenGL.";

    if (context())
    {
        initializeOpenGLFunctions();
        glClearColor(0.f,0.f,0.f,1.f);
        glViewport (0, 0, (GLsizei) width(), (GLsizei) height());

        if (context()->format().majorVersion() >= 4)
        {
            glFunctions = QOpenGLVersionFunctionsFactory::get<SORSOGLFUNCTIONS>(context());
            if (glFunctions)
            {
                mGLVersionValid = true;
            }
            else
            {
                qDebug()
                  << "Die OpenGL Funktionen konten nicht initialisiert werden obwohl die openGL version hoch genug ist";
            }
        }
        else
        {
            qDebug() << "Die erkannte OpenGL Version ist zu niedrig!";
        }
        qDebug() << "Context with versions created: " << context()->format().majorVersion() << context()->format().minorVersion();
    }
    else
    {
        qDebug() << "Context is Zero.";
    }

    //Programmende erkennen, um Abstürze durch falsche Reihenfolge der Freigabe von
    //OpenGL-Ressourcen zu verhindern.
    connect(context(), SIGNAL(aboutToBeDestroyed()), this, SLOT(setCleanup()));

    if (mSetMousePointerinWindow) {
        // Cursor in die Mitte setzen, hilfreich z.B. für Kamera mit Maussteuerung
        QPoint lMiddle = QPoint(width() / 2, height() / 2);
        this->cursor().setPos(this->mapToGlobal(lMiddle));
    }
    resizeGL(width(), height());

    //Ueber den Manager Initialisierung der Anwendung aufrufen
    Manager::initialize();
}

void OpenGLWidget::paintGL()
{
    if (mGLVersionValid)
    {
        glFunctions->glEnable(GL_DEPTH_TEST);
        glFunctions->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        GameSystem::mainLoop();
    }
    else
    {
        // If no glfunctions available go into color buffer test mode
        static float sTestIntensity = 0.f;
        static int sTestRGB = 0;
        static QElapsedTimer sTimer = QElapsedTimer();
        if (sTestIntensity < 1.f)
        {
            sTestIntensity += 0.0005f * sTimer.restart();
            switch (sTestRGB)
            {
            case 0:
                glClearColor(1.f - sTestIntensity, sTestIntensity, 0.f, 1.f);
                break;
            case 1:
                glClearColor(0.f, 1.f - sTestIntensity, sTestIntensity, 1.f);
                break;
            case 2:
                glClearColor(sTestIntensity, 0.f, 1.f - sTestIntensity, 1.f);
                break;
            default:
                break;
            }
        }
        else
        {
            sTestIntensity = 0.f;
            sTestRGB = (sTestRGB + 1) % 3;
        }
        glClear(GL_COLOR_BUFFER_BIT);
    }
}

void OpenGLWidget::resizeGL(int w, int h)
{
    int lWidth = this->devicePixelRatio() * w;
    int lHeight = this->devicePixelRatio() * h;
    glViewport (0, 0, (GLsizei) lWidth, (GLsizei) lHeight);
    //TODO Give Infos to Entities about resize
}

void OpenGLWidget::idle()
{
    if (mIsInCleanup)
        return;
    mFPS.count();
    QString str = QString(QApplication::instance()->applicationName() + " - " + " (%1 fps)").arg(mFPS.getFPS());
    qobject_cast<QMainWindow*>(sMainWindow)->setWindowTitle(str);
    const auto lTickObjects = sTickobjects;
    for (auto lTickers: lTickObjects)
    {
        lTickers->getTick();
    }
}

void OpenGLWidget::setCleanup() {
    mIsInCleanup=true;
    //Alle Entities löschen, v.a. um OpenGL-Ressourcen vor dem Kontext freizugeben
    ECS.clear();
}

void OpenGLWidget::addTickObject(std::shared_ptr<WantsTick> pTickObject)
{
    auto lObj = std::move(pTickObject);
    OpenGLWidget::sTickobjects.insert(lObj);
}

void OpenGLWidget::removeTickObject(std::shared_ptr<WantsTick> pTickObject)
{
    OpenGLWidget::sTickobjects.remove(pTickObject);
}


QPoint OpenGLWidget::convertCoord(const QPoint& pPoint)
{
    int h = height() - 1;
    QPoint res(pPoint.x(), h - pPoint.y());
    return res;
}

bool OpenGLWidget::getIsInCleanup() const
{
    return mIsInCleanup;
}

OpenGLWidget* OpenGLWidget::getInstance()
{
    if (OpenGLWidget::sInstance == nullptr)
    {
        qDebug() << "This should never happen - OpenGLWidget is not properly allocated!";
        OpenGLWidget::sInstance = new OpenGLWidget(qobject_cast<QMainWindow*>(OpenGLWidget::sMainWindow));
    }
    return OpenGLWidget::sInstance;
}

QWidget* OpenGLWidget::getMainWindow()
{
    if (OpenGLWidget::sMainWindow == nullptr)
    {
        qDebug() << "This should never happen - OpenGLWidget has no MainWindow!";
    }
    return OpenGLWidget::sMainWindow;
}

void OpenGLWidget::keyPressEvent(QKeyEvent* e)
{
    // Koordinaten in Qt leider nicht verfuegbar...
    // Qt liefert immer Großbuchstaben und dann einen Modifier
    // Umrechnen!

    int key = e->key();

    if (e->modifiers() != Qt::ShiftModifier && key <= 'Z' && key >= 'A')
    {
        key = tolower(key);
    }

    if (InputRegistry::getInstance().hasKeyboardInput())
        InputRegistry::getInstance().getKeyboardInput()->setKeyDown(key, ( int )(e->modifiers()));

}

void OpenGLWidget::keyReleaseEvent(QKeyEvent* event)
{
    if (InputRegistry::getInstance().hasKeyboardInput()) {
        InputRegistry::getInstance().getKeyboardInput()->setKeyUp(event->key(), ( int )(event->modifiers()));
        if (event->modifiers() != Qt::ShiftModifier && event->key() <= 'Z' && event->key() >= 'A')
        {
            InputRegistry::getInstance().getKeyboardInput()->setKeyUp(tolower(event->key()), ( int )(event->modifiers()));
        }

    }
}

void OpenGLWidget::mouseMoveEvent(QMouseEvent* event)
{
    QPoint dummy = convertCoord(event->pos());

    InputRegistry::getInstance().getMouseInput()->setMousePosition(dummy);
    InputRegistry::getInstance().getMouseInput()->setMouseButtons(( int )(event->buttons()));
}

void OpenGLWidget::mouseReleaseEvent(QMouseEvent* event)
{
    QPoint dummy = convertCoord(event->pos());

    InputRegistry::getInstance().getMouseInput()->setMousePosition(dummy);
    InputRegistry::getInstance().getMouseInput()->setMouseButtons(( int )(event->buttons()));
}

void OpenGLWidget::mousePressEvent(QMouseEvent* event)
{
    QPoint dummy = convertCoord(event->pos());

    InputRegistry::getInstance().getMouseInput()->setMousePosition(dummy);
    InputRegistry::getInstance().getMouseInput()->setMouseButtons(( int )(event->buttons()));
}

void OpenGLWidget::wheelEvent(QWheelEvent* e)
{
    InputRegistry::getInstance().getMouseInput()->addMouseWheelDelta(e->angleDelta().y());
}
