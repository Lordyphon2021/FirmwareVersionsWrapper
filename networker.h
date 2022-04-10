
#pragma once


#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QFile>
#include <QListWidget>
#include <QApplication>
#include <QMessageBox>



class Networker : public QObject
{
    Q_OBJECT

public:
    Networker(QObject* parent = nullptr);

void download(QString location, QString path, QString username, QString password); //main method
void upload(QString location, QString path);

signals:
    void no_data();
    void download_finished(QString message);
    void send_status(QString message);


private slots:
    void readyRead();
    void finished(QNetworkReply* reply);
     void uploadProgress(qint64 bytesReceived, qint64 bytesTotal);
    void downloadProgress(qint64 bytesReceived, qint64 bytesTotal);
    void error(QNetworkReply::NetworkError code);

private:
    QNetworkAccessManager manager;
    QFile file;
    QListWidget* debug_list = nullptr;
    QMessageBox error_message;
    void wire(QNetworkReply* reply);

};

