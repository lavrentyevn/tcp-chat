#include "server.h"
#include <QRegExp>

Server::Server(QObject *parent)
    : QTcpServer{parent}
{
    connect(this, &Server::newUser, this, &Server::slotNewUser);
}


void Server::slotReadyRead()
{
    User* user = reinterpret_cast<User*>(sender());
    data = user->readAll();
    if(check)
    {
        user->setUserName(data);
        data.clear();
        check = false;
        emit newUser();
        return;
    }

    sendDataToAllUsers(data, user->getUserName());
}


void Server::slotNewUser()
{
    for(const auto& user : users)
    {
        user->write("<Users>" + getUsersInJson().toLocal8Bit() + "</Users>");
    }
}


QStringList Server::getUsersName()
{
    QStringList lst;
    for(const auto& item : users) {
        lst.push_back(item->getUserName());
    }
    return lst;
}


void Server::updateSocketCount(QObject* object)
{
    User* u = reinterpret_cast<User*>(object);
    users.removeOne(u);
    emit socketsCountChanged();
}


QString Server::getUsersInJson()
{
    QJsonDocument doc;
    QJsonArray arr;
    for(const auto& user : users)
    {
        arr.append(user->getUserName());
    }
    doc.setArray(arr);
    return doc.toJson();
}


void Server::incomingConnection(qintptr handle)
{
    check = true;
    User* user = new User();
    user->setSocketDescriptor(handle);
    connect(user, &QTcpSocket::readyRead, this, &Server::slotReadyRead);
    connect(user, &QTcpSocket::disconnected, user, &QTcpSocket::deleteLater);
    connect(user, &QTcpSocket::destroyed, this, &Server::updateSocketCount);
    users.push_back(user);
    qDebug() << handle;
}


void Server::sendDataToAllUsers(QByteArray& data, const QString& who)
{
    if(data.length() > 100) {
        for(auto& user : users) {
            user->write(data);
            qDebug() << "data";
            qDebug() << users.size();
        }
    }
    for(auto& user : users)
    {
        if(data.contains("/Users/")) {
            user->write(data);
            qDebug() << data;
            return;
        }
        user->write(who.toLocal8Bit() + ": "+ data);
        qDebug() << data;
    }
}
