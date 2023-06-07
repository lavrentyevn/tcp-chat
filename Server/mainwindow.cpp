#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "guiinterface.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("Server");
    GuiInterface *interface = new GuiInterface();
    setCentralWidget(interface);

}

MainWindow::~MainWindow()
{
    delete ui;
}

