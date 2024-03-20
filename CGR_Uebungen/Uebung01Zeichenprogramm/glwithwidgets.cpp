#include "glwithwidgets.h"
//Vom Qt-Designer generierter Header, enthält die Definition der GUI und Verbindung von Signals und Slots
#include "ui_glwithwidgets.h"

GLwithWidgets::GLwithWidgets(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::GLwithWidgets)
{
    //Initialisieren der GUI
    ui->setupUi(this);
}

GLwithWidgets::~GLwithWidgets()
{
    delete ui;
}
