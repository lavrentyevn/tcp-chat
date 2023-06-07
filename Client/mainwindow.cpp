#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QRegExp>
#include <QFileDialog>
#include <QJsonObject>
#include <QBuffer>


QJsonValue jsonValFromPixmap(const QPixmap &p) {
  QBuffer buffer;
  buffer.open(QIODevice::WriteOnly);
  p.save(&buffer, "JPG");
  auto const encoded = buffer.data().toBase64();
  return {QLatin1String(encoded)};
}


QPixmap pixmapFrom(const QJsonValue &val) {
  auto const encoded = val.toString().toLatin1();
  QPixmap p;
  p.loadFromData(QByteArray::fromBase64(encoded), "JPG");
  return p;
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("Group Chat");


    check = true;
    container = new QWidget();
    hLayout1 = new QHBoxLayout();
    hLayout2 = new QHBoxLayout();
    hLayoutTest = new QHBoxLayout();
    vLayout = new QVBoxLayout();
    line = new QLineEdit();
    lineName = new QLineEdit();
    listWidget = new QListWidget();
    listWidget->setMaximumWidth(150);
    textWidget = new QListWidget();
    hLayout2->addWidget(listWidget);
    hLayout2->addWidget(textWidget);
    btnJson = new QPushButton("Save");
    btnJson->setIcon(QIcon("path_to_icon/json-file.png"));
    btnConnect = new QPushButton("Connect");
    btnConnect->setIcon(QIcon("path_to_icon/people.png"));
    btnSend = new QPushButton("Send");
    btnSend->setIcon(QIcon("path_to_icon/send.png"));
    btnImage = new QPushButton("Image");
    btnImage->setIcon(QIcon("path_to_icon/photo.png"));


    hLayoutTest->addWidget(lineName);
    hLayoutTest->addWidget(btnJson);
    hLayout1->addWidget(line);
    hLayout1->addWidget(btnImage);
    hLayout1->addWidget(btnSend);
    hLayout1->addWidget(btnConnect);
    vLayout->addLayout(hLayoutTest);
    vLayout->addLayout(hLayout2);
    vLayout->addLayout(hLayout1);
    container->setLayout(vLayout);
    setCentralWidget(container);


    connect(btnJson, &QPushButton::clicked, this, &MainWindow::slotJson);
    connect(btnConnect, &QPushButton::clicked, this, &MainWindow::slotConnect);
    connect(btnSend, &QPushButton::clicked, this, &MainWindow::slotSend);
    connect(btnImage, &QPushButton::clicked, this, &MainWindow::slotImage);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::slotConnect()
{
    if(!check) return;
    socket = new QTcpSocket();
    connect(socket, &QTcpSocket::readyRead, this, &MainWindow::slotReadyRead);
    socket->connectToHost("127.0.0.1", 5000);
    socket->write(lineName->text().toLocal8Bit());
    check = false;
}


void MainWindow::slotSend()
{
    if(!socket->isOpen()) return;
    socket->write(line->text().toLocal8Bit());
    line->clear();
}


void MainWindow::slotJson()
{
    QJsonDocument document;
            QFile file("path_to_file/document.txt");
            QJsonObject currentobject;
            for (int i = 0; i < textWidget->count(); ++i) {
                QListWidgetItem *item = textWidget->item(i);
                QJsonObject obj;


                if(!item->icon().isNull()) {
                    QIcon icon = item->icon();
                    QPixmap pix = icon.pixmap(QSize(200, 200));
                    auto json = jsonValFromPixmap(pix);
                    obj.insert("image", json);
                    currentobject.insert(QString("Message №" + QString::number(i)), obj);
                }


                else {
                    obj.insert("text", item->text());
                    currentobject.insert(QString("Message №" + QString::number(i)), obj);
                }
            }


            document.setObject(currentobject);
            QByteArray bytes = document.toJson(QJsonDocument::Indented);
            if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
                QTextStream stream(&file);
                stream << bytes;
                file.close();
            }
}


void MainWindow::slotImage()
{
    QString defaultDirectory = "path_to_file";
         QString filter = "(*.jpg)";
         filepath = QFileDialog::getOpenFileName( nullptr,
                                                   "Choose file",
                                                   defaultDirectory,
                                                   filter );
         qDebug() << filepath;
         if(!socket->isOpen()) return;
         QImage img(filepath);
         auto pix = QPixmap::fromImage(img);
         auto val = jsonValFromPixmap(pix);
         QString str = val.toString();
         QByteArray ba = str.toLatin1();
         qDebug() << ba.data();
         socket->write(ba.data());
         line->clear();
}


void MainWindow::slotReadyRead()
{
    data = socket->readAll()+'\n';
    if(data.contains("<Users>")) {
        listWidget->clear();
        QString str=data;
        QRegExp rx("(<Users>)(.*)(</Users>)");
        QString s;
        int pos=0;
        rx.indexIn(str, pos);
        s=rx.cap(2);
        QJsonDocument doc = QJsonDocument::fromJson(s.toLocal8Bit());
        QJsonArray arr = doc.array();
        for(int i = 0; i < arr.size(); i++)
        {
            listWidget->addItem(arr[i].toString());
        }
        for (int i = 0 ; i < listWidget->model()->rowCount(); ++i) {
            listWidget->item(i)->setForeground(QColor(rand()%255, rand()%255, rand()%255));
        }
        return;
    }
    if(data.contains("/Users/")) {
        QListWidgetItem *item = new QListWidgetItem();
        item->setIcon(QIcon(filepath));
        textWidget->setIconSize(QSize(400, 400));
        textWidget->addItem(item);
        filepath.clear();
        return;
    }
    if(data.length() > 100) {
        QString str = QString(data);
        auto pix = pixmapFrom(str);
        QListWidgetItem *item = new QListWidgetItem();
        item->setIcon(QIcon(pix));
        textWidget->setIconSize(QSize(400, 400));
        textWidget->addItem(item);
        return;
    }
    textWidget->addItem(data);
}
