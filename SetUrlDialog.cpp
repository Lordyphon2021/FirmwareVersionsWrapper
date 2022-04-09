#include "SetUrlDialog.h"
#include <QMessageBox>



SetUrlDialog::SetUrlDialog(QDialog *parent)
    : QDialog(parent)
{
    ui_seturl.setupUi(this);

    connect(ui_seturl.buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(ui_seturl.buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
    connect(ui_seturl.buttonBox, SIGNAL(accepted()),this, SLOT(onReturnPressed()));

}

SetUrlDialog::~SetUrlDialog()
{
}

void SetUrlDialog::onReturnPressed(){
    //get URL from user
    url = ui_seturl.UploadPathLineEdit->displayText();
    //send to MainWindow via signal
    emit sendPathString(url);

    if(url.isEmpty()){
        QMessageBox error;
        error.setText("no input");
        error.exec();
    }
}
