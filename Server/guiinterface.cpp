#include "guiinterface.h"

GuiInterface::GuiInterface(QWidget *parent)
    : QWidget{parent}
{

    layout = new QGridLayout();
    listWidget = new QListWidget();
    label = new QLabel();
    label->setText("List Of Users:");
    labelnew = new QLabel();
    labelnew->setText("Last Updated: ");
    layout->addWidget(label, 0, 0);
    layout->addWidget(labelnew, 0, 1);
    layout->addWidget(listWidget, 1, 0, 1, 2);
    this->setLayout(layout);


    server = new Server;
    connect(server, &Server::newUser, this, &GuiInterface::slotUpdateList);
    connect(server, &Server::socketsCountChanged, this, &GuiInterface::slotUpdateList);
    if(server->listen(QHostAddress::LocalHost, 5000)) {
        qDebug() << "started...";
    }
    else {
        qDebug() << "error";
    }
}


void GuiInterface::slotUpdateList()
{
    listWidget->clear();
    for(const auto& item : server->getUsersName()) {
        listWidget->addItem(item);
    }
    qDebug() << QDateTime::currentDateTime().toString();
    labelnew->setText("Last Updated: " + QTime::currentTime().toString());
    for (int i = 0 ; i < listWidget->model()->rowCount(); ++i) {
        listWidget->item(i)->setForeground(QColor(rand()%255, rand()%255, rand()%255));
    }
}
