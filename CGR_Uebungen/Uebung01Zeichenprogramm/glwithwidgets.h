#ifndef GLWITHWIDGETS_H
#define GLWITHWIDGETS_H

#include <QMainWindow>

namespace Ui {
    class GLwithWidgets;
}

class GLwithWidgets : public QMainWindow
{
    Q_OBJECT

public:
    explicit GLwithWidgets(QWidget *parent = 0);
    ~GLwithWidgets();

private:
    Ui::GLwithWidgets *ui;
};

#endif // GLWITHWIDGETS_H
