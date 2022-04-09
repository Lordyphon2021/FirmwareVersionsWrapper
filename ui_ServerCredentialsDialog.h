/********************************************************************************
** Form generated from reading UI file 'ServerCredentialsDialog.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SERVERCREDENTIALSDIALOG_H
#define UI_SERVERCREDENTIALSDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>

QT_BEGIN_NAMESPACE

class Ui_ServerCredentialsDialog
{
public:
    QDialogButtonBox *ButtonBox;
    QLabel *label;
    QLabel *label_2;
    QLineEdit *lineEdit_user;
    QLineEdit *lineEdit_password;

    void setupUi(QDialog *ServerCredentialsDialog)
    {
        if (ServerCredentialsDialog->objectName().isEmpty())
            ServerCredentialsDialog->setObjectName(QString::fromUtf8("ServerCredentialsDialog"));
        ServerCredentialsDialog->resize(400, 300);
        ButtonBox = new QDialogButtonBox(ServerCredentialsDialog);
        ButtonBox->setObjectName(QString::fromUtf8("ButtonBox"));
        ButtonBox->setGeometry(QRect(190, 240, 181, 32));
        ButtonBox->setOrientation(Qt::Horizontal);
        ButtonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
        label = new QLabel(ServerCredentialsDialog);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(50, 40, 291, 16));
        label_2 = new QLabel(ServerCredentialsDialog);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(50, 130, 301, 16));
        lineEdit_user = new QLineEdit(ServerCredentialsDialog);
        lineEdit_user->setObjectName(QString::fromUtf8("lineEdit_user"));
        lineEdit_user->setGeometry(QRect(50, 70, 311, 21));
        lineEdit_password = new QLineEdit(ServerCredentialsDialog);
        lineEdit_password->setObjectName(QString::fromUtf8("lineEdit_password"));
        lineEdit_password->setGeometry(QRect(50, 160, 311, 21));

        retranslateUi(ServerCredentialsDialog);
        QObject::connect(ButtonBox, SIGNAL(accepted()), ServerCredentialsDialog, SLOT(accept()));
        QObject::connect(ButtonBox, SIGNAL(rejected()), ServerCredentialsDialog, SLOT(reject()));

        QMetaObject::connectSlotsByName(ServerCredentialsDialog);
    } // setupUi

    void retranslateUi(QDialog *ServerCredentialsDialog)
    {
        ServerCredentialsDialog->setWindowTitle(QCoreApplication::translate("ServerCredentialsDialog", "Dialog", nullptr));
        label->setText(QCoreApplication::translate("ServerCredentialsDialog", "set user name: (don't use newline character)", nullptr));
        label_2->setText(QCoreApplication::translate("ServerCredentialsDialog", "set password:", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ServerCredentialsDialog: public Ui_ServerCredentialsDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SERVERCREDENTIALSDIALOG_H
