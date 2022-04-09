#include "servercredentialsdialog.h"
#include <QMessageBox>


ServerCredentialsDialog::ServerCredentialsDialog(QDialog *parent)
    : QDialog(parent)
{
    ui_setserver.setupUi(this);

    connect(ui_setserver.ButtonBox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(ui_setserver.ButtonBox, SIGNAL(rejected()), this, SLOT(reject()));
    connect(ui_setserver.ButtonBox, SIGNAL(accepted()),this, SLOT(onReturnPressed()));

}

ServerCredentialsDialog::~ServerCredentialsDialog()
{
}

void ServerCredentialsDialog::onReturnPressed(){
    //get URL from user
    user = ui_setserver.lineEdit_user->displayText();
    password = ui_setserver.lineEdit_password ->displayText();

    //send to MainWindow via signal
    emit ServerCredentialsStrings(user, password);

    if(password.isEmpty() || user.isEmpty()){
        QMessageBox error;
        error.setText("no input");
        error.exec();
    }
}
