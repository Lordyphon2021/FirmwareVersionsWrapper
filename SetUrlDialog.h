#pragma once
#include <QApplication>
#include <QDialog>
#include "ui_SetUrlDialog.h"
#include <qdebug.h>


class SetUrlDialog : public QDialog, public Ui::SetUrlDialog
{

Q_OBJECT;
public:
    SetUrlDialog(QDialog *parent = Q_NULLPTR);
    ~SetUrlDialog();

signals:
    void sendPathString(QString _url);

public slots:
    void onReturnPressed();

private:
    Ui::SetUrlDialog ui_seturl;
    QString url;
};





