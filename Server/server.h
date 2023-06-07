#ifndef SERVER_H
#define SERVER_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QDataStream>
#include <QVector>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonArray>
#include "user.h"

class Server : public QTcpServer
{
    Q_OBJECT
public:
    explicit Server(QObject *parent = nullptr);

    void slotReadyRead();
    void slotSend();
    void slotNewUser();
    QStringList getUsersName();
    void updateSocketCount(QObject* object);
    QString getUsersInJson();
    void sendDataToAllUsers(QByteArray& data, const QString& who);
signals:
    void socketsCountChanged();
    void newUser();
    // QTcpServer interface
protected:
    void incomingConnection(qintptr handle);
private:
    QVector<User*> users;
    QByteArray data;
    bool check;
};

#endif // SERVER_H
