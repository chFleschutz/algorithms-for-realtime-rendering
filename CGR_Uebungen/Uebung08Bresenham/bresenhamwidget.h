#ifndef BRESENHAMWIDGET_H
#define BRESENHAMWIDGET_H

#include <QWidget>
#include <QImage>

class BresenhamWidget : public QWidget
{
    Q_OBJECT
public:
    explicit BresenhamWidget(QWidget *parent = 0);
    void paintEvent(QPaintEvent *event);

signals:

public slots:
    void toggleOwnCircle() {mOwnCircle=!mOwnCircle; repaint();}
    void toggleQtCircle() {mQtCircle=!mQtCircle; repaint();}
    void toggleOwnLine() {mOwnLine=!mOwnLine; repaint();}
    void toggleQtLine() {mQtLine=!mQtLine; repaint();}

private:
    QImage *mImage;
    uint mSize;
    QRgb mDrawColor;
    void RenderLine2D(QPoint start, QPoint end);
    void DrawCircleMidPoint(QPoint center, QPoint outer);
    bool mOwnLine;
    bool mOwnCircle;
    bool mQtCircle;
    bool mQtLine;
};

#endif // BRESENHAMWIDGET_H
