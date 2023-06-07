#include "user.h"

User::User(QObject *parent)
    : QTcpSocket{parent}, name{""}
{

}

User::User(const QString& name, QObject *parent)
    : QTcpSocket{parent}, name{name}
{

}


void User::setUserName(const QString &name)
{
    this->name = name;
}


QString User::getUserName() const
{
    return name;
}
