#include "bresenhamwidget.h"
#include <cmath>
#include <qpainter.h>
#include <qdebug.h>

void BresenhamWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    //create a QPainter and pass a pointer to the device.
    //A paint device can be a QWidget, a QPixmap or a QImage
    QPainter painter(this);
    QPen pen(QColor(0,255,0));
    painter.setPen(pen);
    //painter.setCompositionMode(QPainter::RasterOp_SourceOrDestination);
    mImage->fill(0);
    if (mOwnLine)
        RenderLine2D(QPoint(0,0), QPoint(mSize-1,mSize/2-1));
    if (mOwnCircle)
        DrawCircleMidPoint(QPoint(200,200), QPoint(200,300));
    painter.drawImage(QPoint(0,0), *mImage);
    if (mQtCircle)
    {
            QRectF rectangle(100.0, 100.0, 200.0, 200.0);
            painter.drawArc(rectangle, 0, 16 * 360);
    }
    if (mQtLine)
    {
        painter.drawLine(0,0,mSize-1, mSize/2-1);
    }
}

void BresenhamWidget::RenderLine2D(QPoint start, QPoint end)
{
    for (auto i = 0; i < mSize; i++)
    {
        //Hier Berechnung der Gerade einfügen
        mImage->setPixel(i, i, mDrawColor);
    }
}

//! Zeichnet Kreis in die bitplane
//! center: Mittelpunkt; outer: Pkt auf Kreisbogen
void BresenhamWidget::DrawCircleMidPoint(QPoint center, QPoint outer) {
//Hier Berechnung des Kreises einfügen
}

BresenhamWidget::BresenhamWidget(QWidget *parent) :
    QWidget(parent)
{
    mSize = 400;
    mImage = new QImage(400, 600, QImage::Format_RGB32);
    QColor lC(255,0,0);
    mDrawColor = lC.rgb();
    mOwnCircle=mOwnLine=mQtCircle=mQtLine=false;
}
