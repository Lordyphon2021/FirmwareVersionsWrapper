#pragma once
#include <QApplication>
#include <QDialog>
#include "ui_Separator_dialog.h"

class Separator_dialog : public QDialog, public Ui::Separator_dialog
{
	Q_OBJECT

public:
	Separator_dialog(QWidget *parent = Q_NULLPTR);
	~Separator_dialog();
	
signals:
    void SeparatorStringSignal(QString separator_string);

public slots:
    void onReturnPressed();

private:
	 Ui::Separator_dialog sep_dial;
    QString user;
    QString password;
};



