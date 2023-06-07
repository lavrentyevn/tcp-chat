#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLineEdit>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QTextEdit>
#include <QTcpSocket>
#include <QListWidget>
#include <QJsonDocument>
#include <QJsonArray>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void slotConnect();
    void slotSend();
    void slotJson();
    void slotImage();
    void slotReadyRead();

private:
    Ui::MainWindow *ui;
    QLineEdit* line, *lineName;
    QPushButton* btnConnect, *btnSend, *btnJson, *btnImage;
    QWidget* container;
    QHBoxLayout* hLayout1, *hLayout2, *hLayoutTest;
    QListWidget* listWidget;
    QVBoxLayout* vLayout;
    QTextEdit* textEdit;
    QString filepath;
    QListWidget *textWidget;
    QTcpSocket* socket;
    bool check;
    QByteArray data;
};
#endif // MAINWINDOW_H
