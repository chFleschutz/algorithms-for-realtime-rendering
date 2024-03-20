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

void BresenhamWidget::RenderLine2D(QPoint start, QPoint end) {

    bool b1 = false, b2 = false, b3 = false;
    int inc1;
    int inc2;
        int pt1[]={start.x(),start.y()}, pt2[]={end.x(),end.y()};
        int pt[] = {0,0}, out[2] = {0,0};
    int	dx;
    int	dy;
    float E;

    // voraussetzungen ueber Lage der Punkte pr¸fen
    int tmp;
    // (1) dx = x2 - x1 >= 0
    if (pt2[0]-pt1[0] < 0) {
        tmp = pt1[0];
        pt1[0] = pt2[0];
        pt2[0] = tmp;
        b1 = true;
    }

    // (2) dy = y2 - y1 >= 0
    if (pt2[1]-pt1[1] < 0) {
        tmp = pt1[1];
        pt1[1] = pt2[1];
        pt2[1] = tmp;
        b2 = true;
    }

    // (3) dx >= dy
    if (pt2[0]-pt1[0] < pt2[1]-pt1[1]) {
        tmp = pt1[0];
        pt1[0] = pt1[1];
        pt1[1] = tmp;
        tmp = pt2[0];
        pt2[0] = pt2[1];
        pt2[1] = tmp;
        b3 = true;
    }

    dx = pt2[0] - pt1[0];
    dy = pt2[1] - pt1[1];

    // Entscheidungsvariable initialisieren und Inkremente vorab berechnen
    E = (dy<<1) - dx;
    inc1 = (dy<<1);
    inc2 = (dy<<1) - (dx<<1);

    while(dx > pt[0]) {
        if (E <= 0) {

            pt[0] += 1;

            E += inc1;

        } else { // E < 0

            pt[0] += 1;
            pt[1] += 1;

            E += inc2;

        }

        // zeichnen
        if (!b1 && !b2 && !b3) {
            out[0] = pt1[0] + pt[0];
            out[1] = pt1[1] + pt[1];
        } else if (!b1 && !b2 && b3) {
            out[0] = pt1[1] + pt[1];
            out[1] = pt1[0] + pt[0];
        } else if (b1 && !b2 && b3) {
            out[0] = pt2[1] - pt[1];
            out[1] = pt1[0] + pt[0];
        } else if (b1 && !b2 && !b3) {
            out[0] = pt2[0] - pt[0];
            out[1] = pt1[1] + pt[1];
        } else if (b1 && b2 && !b3) {
            out[0] = pt2[0] - pt[0];
            out[1] = pt2[1] - pt[1];
        } else if (b1 && b2 && b3) {
            out[0] = pt2[1] - pt[1];
            out[1] = pt2[0] - pt[0];
        } else if (!b1 && b2 && b3) {
            out[0] = pt1[1] + pt[1];
            out[1] = pt2[0] - pt[0];
        } else if (!b1 && b2 && !b3) {
            out[0] = pt1[0] + pt[0];
            out[1] = pt2[1] - pt[1];
        }

        mImage->setPixel(out[0], out[1], mDrawColor);
    }
}

//! Zeichnet Kreis in die bitplane
//! center: Mittelpunkt; outer: Pkt auf Kreisbogen
void BresenhamWidget::DrawCircleMidPoint(QPoint center, QPoint outer) {

    int pt[] = { 0, 0 }, out[2];
        int ptM[] = {center.x(), center.y()}, ptR[]={outer.x(), outer.y()};
    float r;
    float border;
    float E = 0;

    // r wird von 0/0 aus gemessen
    // ptM fungiert als offset f¸r sp‰tere positionierung
    ptR[0] -= ptM[0];
    ptR[1] -= ptM[1];	r = sqrt( ((ptR[0])*(ptR[0]) + (ptR[1])*(ptR[1])));
    border = r/sqrt(2);

    if (r<2) return;

    // radius berechnet; berechnungen gehen von (0/r) bis (r/r)
    ptR[0] = 0;
    ptR[1] = r;


    // 0 zeichnen
    out[0] = ptM[0] + r;
    out[1] = ptM[1];
    mImage->setPixel(out[0], out[1], mDrawColor);

    // PI/2 zeichnen
    out[0] = ptM[0];
    out[1] = ptM[1] + r;
    mImage->setPixel(out[0], out[1], mDrawColor);

    // PI zeichnen
    out[0] = ptM[0] - r + 1;
    out[1] = ptM[1];
    mImage->setPixel(out[0], out[1], mDrawColor);

    // 3/2PI zeichnen
    out[0] = ptM[0];
    out[1] = ptM[1] - r + 1;
    mImage->setPixel(out[0], out[1], mDrawColor);

    E = 1.25-r;

    while(ptR[0]<=border) {

        if (E <= 0) {

            E += (ptR[0]<<1)+3;
            ptR[0] += 1;

        } else {

            E += (ptR[0]<<1)-(ptR[1]<<1)+5;
            ptR[0] += 1;
            ptR[1] -= 1;

        }

        // OCT 1
        out[0] = ptM[0] + ptR[1] + pt[1];
        out[1] = ptM[1] + ptR[0] + pt[0];
        mImage->setPixel(out[0], out[1], mDrawColor);

        // OCT 2: normal zeichnen
        out[0] = ptM[0] + ptR[0] + pt[0];
        out[1] = ptM[1] + ptR[1] + pt[1];
        mImage->setPixel(out[0], out[1], mDrawColor);

        // OCT 3
        out[0] = ptM[0] - ptR[0] - pt[0];
        out[1] = ptM[1] + ptR[1] + pt[1];
        mImage->setPixel(out[0], out[1], mDrawColor);

        // OCT 4
        out[0] = ptM[0] - ptR[1] - pt[1];
        out[1] = ptM[1] + ptR[0] - pt[0];
        mImage->setPixel(out[0], out[1], mDrawColor);

        // OCT 5
        out[0] = ptM[0] - ptR[1] - pt[1];
        out[1] = ptM[1] - ptR[0] + pt[0];
        mImage->setPixel(out[0], out[1], mDrawColor);

        // OCT 6
        out[0] = ptM[0] - ptR[0] - pt[0];
        out[1] = ptM[1] - ptR[1] - pt[1];
        mImage->setPixel(out[0], out[1], mDrawColor);

        // OCT 7
        out[0] = ptM[0] + ptR[0] + pt[0];
        out[1] = ptM[1] - ptR[1] - pt[1];
        mImage->setPixel(out[0], out[1], mDrawColor);

        // OCT 8
        out[0] = ptM[0] + ptR[1] + pt[1];
        out[1] = ptM[1] - ptR[0] - pt[0];
        mImage->setPixel(out[0], out[1], mDrawColor);

    }
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
