#include <QtGui>

//#include <math.h>
#include <iostream>
#include <list>

using namespace std;

#include "glwidget.h"

GLWidget::GLWidget(QWidget *parent)
    : QOpenGLWidget(parent)
{
    mPointSize = 1.0;
    mModus = Punkte;
    mHeight=mWidth=400;
}

GLWidget::~GLWidget()
{
}

void GLWidget::initializeGL()
{
    initializeOpenGLFunctions();
    glClearColor(0.f,0.f,0.f,1.f);
    glViewport (0, 0, (GLsizei) mWidth, (GLsizei) mHeight);
    glMatrixMode (GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-mWidth/2, (GLdouble) mWidth/2, -mHeight/2, (GLdouble) mHeight/2, -100, 100);
    glClear(GL_COLOR_BUFFER_BIT);
    mPoints.clear();
}
void GLWidget::paintGL()
{
    QList<QVector2D>::Iterator i;

    glClear(GL_COLOR_BUFFER_BIT);
    glPointSize(mPointSize);
    glColor3f(1.0, 1.0, 1.0);

    if (mModus == Punkte)
    {
            glBegin(GL_POINTS);
    }
    else if (mModus == Linien)
    {
            glBegin(GL_LINES);
    }

    //Abhängig vom oben gesetzten Modus Linien/Punkte zeichnet diese Methode alle Punkte, die in mPoints enthalten sind
    for (i=mPoints.begin(); i!=mPoints.end(); i++)
    {
        glVertex3d((*i).x(),(*i).y(), 0);
    }
    glEnd();
}
void GLWidget::resizeGL(int width, int height)
{
    mWidth=width;
    mHeight=height;
    glViewport (0, 0, (GLsizei) mWidth, (GLsizei) mHeight);
    glMatrixMode (GL_PROJECTION);
    glLoadIdentity ();
    glOrtho(-mWidth/2, (GLdouble) mWidth/2, -mHeight/2, (GLdouble) mHeight/2, -100, 100);
    glClear(GL_COLOR_BUFFER_BIT);
}
void GLWidget::mousePressEvent(QMouseEvent *event)
{
    std::cout << "Mouse press at " << event->pos().x() << " " << event->pos().y() << std::endl;

    //Hier müssen Sie was tun...
    update();
}
void GLWidget::mouseReleaseEvent(QMouseEvent *event)
{
    std::cout << "Mouse release at " << event->pos().x() << " " << event->pos().y() << std::endl;
    update();
}
void GLWidget::mouseMoveEvent(QMouseEvent *event)
{
    std::cout << "Mouse move to " << event->pos().x() << " " << event->pos().y() << std::endl;

    //Hier müssen Sie was tun...
    update();
}
void GLWidget::setPointSize(int size)
{
    mPointSize = size/10.F;
    glPointSize(mPointSize);
    emit pointSizeChanged(size);
}

void GLWidget::modeToggled(bool points)
{
    if (points)
        mModus = Punkte;
    else
        mModus = Linien;
}
