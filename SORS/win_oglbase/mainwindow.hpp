#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

//! Ist der Startpunkt für die gesamte Applikation. Im Konstruktor muss die GUI und alle sonstigen Objekte instanziert
//! und initialisiert werden.
class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    virtual ~MainWindow() {}
};

#endif // MAINWINDOW_H
