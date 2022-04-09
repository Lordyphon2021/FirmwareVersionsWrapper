

#pragma once
#include <QApplication>
#include <QDialog>
#include <qdebug.h>
#include "ui_ServerCredentialsDialog.h"


class ServerCredentialsDialog : public QDialog, public Ui::ServerCredentialsDialog
{

Q_OBJECT;
public:
    ServerCredentialsDialog(QDialog *parent = Q_NULLPTR);
    ~ServerCredentialsDialog();

signals:
    void ServerCredentialsStrings(QString user, QString password);

public slots:
    void onReturnPressed();

private:
    Ui::ServerCredentialsDialog ui_setserver;
    QString user;
    QString password;
};


