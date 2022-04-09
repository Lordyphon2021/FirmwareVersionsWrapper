#include "separator_dialog.h"
#include <qmessagebox.h>

Separator_dialog::Separator_dialog(QWidget *parent)
	: QDialog(parent)
{
	sep_dial.setupUi(this);

	connect(sep_dial.buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
	connect(sep_dial.buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
	connect(sep_dial.buttonBox, SIGNAL(accepted()), this, SLOT(onReturnPressed()));
}

Separator_dialog::~Separator_dialog()
{
}

void Separator_dialog::onReturnPressed() {
    //get string from user
    QString separator_string = sep_dial.lineEditSeparator->displayText();
   

    //send to MainWindow via signal
    emit SeparatorStringSignal(separator_string);

    if (separator_string.isEmpty()) {
        QMessageBox error;
        error.setText("no input");
        error.exec();
    }
}
