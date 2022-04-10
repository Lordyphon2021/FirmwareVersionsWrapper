#include "networker.h"


Networker::Networker(QObject* parent):QObject(parent){

    connect(&manager, &QNetworkAccessManager::finished, this, &Networker::finished);
}


void Networker::download(QString location, QString path, QString username, QString password){ //location and path via CTOR

    file.setFileName(path); //download destination

    if (!file.open(QIODevice::WriteOnly)){
       emit send_status(file.errorString());
       return;
    }

    QUrl url(location);
    url.setPort(21);
    //url.setUserName("stefandeisenberger86881");
    //url.setPassword("3333Sync!!!");
    url.setUserName(username);
    url.setPassword(password);

    QNetworkRequest request = QNetworkRequest(url);
    QNetworkReply* reply = manager.get(request);
    wire(reply);
}

void Networker::readyRead(){

    QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());

    if (reply){
        QByteArray data = reply->readAll();
        file.write(data); // download data to file
    }
    else {
        emit send_status( "readyread error" );
    }
}

void Networker::finished(QNetworkReply* reply){

    file.close();
    reply->close();
    emit download_finished("transfer finished");
    //emit send_status( "done..." );
}

void Networker::uploadProgress(qint64 bytesSent, qint64 bytesTotal){

    if (bytesTotal <= 0) {
        emit send_status(" check internet connection");
        return;
    }
    emit send_status("Uploading file: " + QString::number(bytesSent) + " of " +  QString::number(bytesTotal));

    
}

void Networker::downloadProgress(qint64 bytesReceived, qint64 bytesTotal){

    if (bytesTotal <= 0) {

        emit send_status ("no data received... ");
        emit no_data();
        file.close();
       
        return;
        
    }
    else
        emit send_status("downloading... ");
}

void Networker::upload(QString location, QString path){
    file.setFileName(path);

    if(!file.open(QIODevice::ReadOnly)){
        emit send_status(file.errorString());
        return;
    }

    QUrl url(location);
    url.setPort(21);
    url.setUserName("stefandeisenberger86881");
    url.setPassword("3333Sync!!!");

    QNetworkRequest request = QNetworkRequest(url);
    QNetworkReply* reply = manager.put(request, &file);
    wire(reply);

}

void Networker::error(QNetworkReply::NetworkError code){

    emit send_status( "error" + QString::number(code));
}

void Networker::wire(QNetworkReply* reply){

    connect(reply, &QNetworkReply::readyRead, this, &Networker::readyRead);
    connect(reply, &QNetworkReply::downloadProgress, this, &Networker::downloadProgress);
    connect(reply, &QNetworkReply::uploadProgress, this, &Networker::uploadProgress);
    connect(reply, QOverload<QNetworkReply::NetworkError>::of(&QNetworkReply::error), this, &Networker::error);
    //connect(reply, &QNetworkReply::errorOccurred, this, &Networker::error);
}

