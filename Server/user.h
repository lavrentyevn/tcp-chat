#ifndef USER_H
#define USER_H

#include <QTcpSocket>

class User : public QTcpSocket
{
    Q_OBJECT
public:
    explicit User(QObject *parent = nullptr);
    explicit User(const QString& name, QObject *parent = nullptr);

    void setUserName(const QString& name);
    QString getUserName() const;
private:
    QString name;

};

#endif // USER_H
