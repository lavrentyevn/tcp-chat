#ifndef GUIINTERFACE_H
#define GUIINTERFACE_H

#include <QWidget>
#include <QGridLayout>
#include <QListWidget>
#include <QLabel>
#include <QDebug>

#include "server.h"

class GuiInterface : public QWidget
{
    Q_OBJECT
public:
    explicit GuiInterface(QWidget *parent = nullptr);

    void slotUpdateList();
private:
    QGridLayout* layout;
    QListWidget* listWidget;
    QLabel* label;
    QLabel *labelnew;
    Server* server;
};

#endif // GUIINTERFACE_H
