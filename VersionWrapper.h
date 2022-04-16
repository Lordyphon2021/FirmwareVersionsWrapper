#ifndef VERSIONWRAPPER_H
#define VERSIONWRAPPER_H

#include <QMainWindow>
#include<qdebug.h>
#include <qpushbutton.h>
#include "SetUrlDialog.h"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class VersionWrapper : public QMainWindow
{
    Q_OBJECT

public:
    VersionWrapper(QWidget *parent = nullptr);
    ~VersionWrapper();
    void allowConnection(){};

public slots:
    //push buttons
    void OnWrapButton();
    void on_pushButton_clicked();
    //QTimer
    void checkOnlineStatus();
    //menu
    void OnActionSetUrl();
    void OnActionSetSourceDir();
    void OnActionAbout();
    void OnActionServer();
   
    //data from dialog and network status strings
    void OnUrlSignal(QString _url);
    void OnNetworkStatusSignal(QString status);
    void onCredentialsSignal(QString user, QString password);
   
private:
    Ui::MainWindow *ui;
    QString url;
    QString path_to_dir;
    QString path_to_dir_last;
    QString user_name;
    QString password;
    SetUrlDialog* seturldlg = nullptr;
    QString backslashreplacer(QString input);
    bool connection_status = false;
    int download_ctr = 0;


};
#endif // VERSIONWRAPPER_H
