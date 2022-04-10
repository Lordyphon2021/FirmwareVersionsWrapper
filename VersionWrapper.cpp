#include "VersionWrapper.h"
#include "ui_VersionWrapper.h"
#include "SetUrlDialog.h"
#include "networker.h"
#include <QMessageBox>
#include <QFile>
#include <QDir>
#include <QFileDialog>
#include <QPalette>
#include <QPixmap>
#include <QStandardPaths>
#include "servercredentialsdialog.h"
#include "separator_dialog.h"
#include <QTimer>


VersionWrapper::VersionWrapper(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{

    ui->setupUi(this);
    this->setWindowTitle("VersionWrapper 1.01");

    //setup menu bar
    QAction* source_path_triggered = new QAction("set source dir", this);
    QAction* url_path_triggered = new QAction("set upload URL and filename", this);
    QAction* server_credentials_triggered = new QAction("set server credentials", this);
    QAction* separator_string_triggered = new QAction("set separator string", this);
    QAction* about_triggered = new QAction("about", this);
    QMenu* preferences = menuBar()->addMenu("&preferences");

    //add trigger actions
    preferences->addAction(source_path_triggered);
    preferences->addAction(url_path_triggered);
    preferences->addAction(server_credentials_triggered);
    preferences->addAction(separator_string_triggered);
    preferences->addAction(about_triggered);
    ui->pushButton->hide();
    
    //wire up
    connect(ui->WrapButton, SIGNAL(clicked()), this, SLOT(OnWrapButton()));
    connect(source_path_triggered, SIGNAL(triggered()), this, SLOT(OnActionSetSourceDir()));
    connect(url_path_triggered, SIGNAL(triggered()), this, SLOT(OnActionSetUrl()));
    connect(separator_string_triggered, SIGNAL(triggered()), this, SLOT(OnActionSeparatorString()));
    connect(about_triggered, SIGNAL(triggered()), this, SLOT(OnActionAbout()));
    connect(server_credentials_triggered, SIGNAL(triggered()), this, SLOT(OnActionServer()));
    
    //setup folders
    QDir pref(QDir::homePath() + "/VersionWrapper/Preferences");
    if (!pref.exists())
        pref.mkpath(".");

    QDir converted(QDir::homePath() + "/VersionWrapper/Converted");
    if (!converted.exists())
        converted.mkpath(".");

    //get saved path to source folder from preferences
    QFile file(QDir::homePath() + "/VersionWrapper/Preferences/pref.txt");
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    path_to_dir = file.readLine();
    ui->label_source_folder->setText(path_to_dir);
    file.close();

    if (path_to_dir.isEmpty()) {
        //if nothing there, open file dialog
        ui->label_source_folder->setText("not set");
        path_to_dir = QFileDialog::getExistingDirectory(this, "set path to firmware files", QDir::homePath());

        //save path to preferences
        QFile file(QDir::homePath() + "/VersionWrapper/Preferences/pref.txt");
        file.open(QIODevice::WriteOnly | QIODevice::Text);
        QTextStream out(&file);
        out << path_to_dir;
        file.close();
        //create directory
        QDir new_source(path_to_dir);
        if (!new_source.exists())
            new_source.mkpath(".");

        //update GUI
        ui->label_source_folder->setText(path_to_dir);

    }
    else {
        QDir saved_source(path_to_dir);
        if (!saved_source.exists())
            saved_source.mkpath(".");
    }

    //get saved upload url from preferences folder
    QFile url_file(QDir::homePath() + "/VersionWrapper/Preferences/url.txt");
    url_file.open(QIODevice::ReadOnly | QIODevice::Text);
    url = url_file.readLine();
    ui->url_label->setText(url);
    url_file.close();

    if (url.isEmpty())
        ui->url_label->setText("not set");

    //get saved server credentials from preferences
    QFile cred(QDir::homePath() + "/VersionWrapper/Preferences/credentials.txt");
    cred.open(QIODevice::ReadOnly | QIODevice::Text);
    user_name = cred.readLine();
    password = cred.readLine();

    //better way to do this?
    for (int i = 0; i < user_name.size(); ++i) {
        if (user_name.at(i) == '\n')
            user_name.remove(i, 1);
    }
    ui->label_server_credentials->setText("user: " + user_name + "  password: " + password);

    //get saved separator string from preferences folder
    QFile sep_file(QDir::homePath() + "/VersionWrapper/Preferences/separator.txt");
    sep_file.open(QIODevice::ReadOnly | QIODevice::Text);
    separator_string = sep_file.readLine();
    ui->label_separator->setText(separator_string);
    sep_file.close();

    if (separator_string.isEmpty())
        ui->label_separator->setText("not set");
    
    //set background photo
    QPixmap bkgrd(QCoreApplication::applicationDirPath() + "/background_smaller.jpg");
    bkgrd = bkgrd.scaled(this->size(), Qt::IgnoreAspectRatio);
    QPalette pal;
    pal.setBrush(QPalette::Background, bkgrd);
    this->setPalette(pal);

    QTimer* hot_plug_timer = new QTimer(this);
    connect(hot_plug_timer, SIGNAL(timeout()), this, SLOT(checkOnlineStatus()));
    hot_plug_timer->start(10000);

    checkOnlineStatus();
}

VersionWrapper::~VersionWrapper()
{
    delete ui;
}

void VersionWrapper::OnWrapButton() {
    download_ctr = 0;
    ui->status_label->setText("looking for files...");

    QDir source(path_to_dir);
    QStringList files_in_source = source.entryList(QStringList() << "*.HEX" << "*.hex", QDir::Files);
    qDebug() << files_in_source;
    QString seperator_string_saved = separator_string;
    int version_number = 0;
    //open destination file
    QFile destin_file(QDir::homePath() + "/VersionWrapper/Converted/firmware_collection.txt");
    destin_file.open(QIODevice::WriteOnly | QIODevice::Text);
    //create textstream
    QTextStream out(&destin_file);

    if(files_in_source.isEmpty()) {
        QMessageBox error;
        error.setText("no HEX files found!");
        error.exec();
        return;
    }
    //open each hex file and write content to destination file, divided by seperator line
    foreach(QString filename, files_in_source) {
        QString version_number_string = QString::number(version_number++);
        
        //add prefix zero 
        if (version_number < 10) {
            version_number_string = "0" + QString::number(version_number);
            separator_string +=  version_number_string; //write seperator line
        }
        else  
            separator_string += version_number_string; //write seperator line
        
        ui->status_label->setText("wrapping " + filename);
        out << separator_string << '\n';
        
        QFile sourcefile(path_to_dir + "/" + filename);

        if (!sourcefile.open(QIODevice::ReadOnly | QIODevice::Text)) { //read .HEX as text file to get rid of blank lines
            QMessageBox error;
            error.setText(" file not found!");
            error.exec();
        }

        while (!sourcefile.atEnd()) { // read from file and write lines to destination textfile
            QByteArray templine = sourcefile.readLine();
            out << templine;
        }
        sourcefile.close();
        separator_string = seperator_string_saved;
    } //END: foreach(QString filename, files_in_source){
    qDebug() << destin_file.size(); //testfile: 537092 bytes
    destin_file.close();
    ui->status_label->setText("done wrapping...");
    //make "check file" button visible
    ui->pushButton->show();

    //QString upload_url = "ftp://stefandeisenberger86881@ftp.lordyphon.com/firmware_versions/firmware_versions.txt";
    QString path_to_file = QDir::homePath() + "/VersionWrapper/Converted/firmware_collection.txt";

    
    
    Networker* try_upload = new Networker;
    connect(try_upload, SIGNAL(send_status(QString)), this, SLOT(OnNetworkStatusSignal(QString)));
   
    //QNetworkAccessMAnager runs as own thread in background
    if(connection_status == true)
        try_upload->upload(url, path_to_file);
    else {
        QMessageBox error;
        error.setText("no internet connection!");
        error.exec();
    }

}
void VersionWrapper::checkOnlineStatus() {
    download_ctr++;
    
    qDebug() << "QTimer timeout, checking for internet connection";
    Networker* network = new Networker;
    connect(network, SIGNAL(download_finished(QString)), this, SLOT(OnNetworkStatusSignal(QString)));
    
    QFile testfile(QDir::homePath() + "/VersionWrapper/testfile.txt");
    
    
    if (testfile.size() == 0 || download_ctr == 7) {
        download_ctr = 8;
        network->download("ftp://stefandeisenberger86881@ftp.lordyphon.com/firmware_versions/testfile.txt", QDir::homePath() + "/VersionWrapper/testfile.txt", user_name, password);

    }
    while(!testfile.open(QIODevice::ReadOnly | QIODevice::Text))
        ;
    
    if (testfile.readLine() == "onlinechecker") {
        connection_status = true;
        ui->status_label->setText( "online" );
    }
    else {
        connection_status = false;
        ui->status_label->setText("offline");
        
    }
    if(testfile.isOpen())
        testfile.close();

    
    //QNetworkAccessMAnager runs as own thread in background
    //network->upload(url, path_to_file);



}


// on check file button
void VersionWrapper::on_pushButton_clicked(){
    QString file_name = QFileDialog::getOpenFileName(this, "file", QDir::homePath() + "/VersionWrapper/Converted/firmware_collection.txt");
}

//preferences menu (set source dir) triggered
void VersionWrapper::OnActionSetSourceDir() {
    
    path_to_dir = QFileDialog::getExistingDirectory(this, "set path to firmware files", QDir::homePath());

    QDir new_source(path_to_dir);
    if (!new_source.exists())
        new_source.mkpath(".");

    ui->label_source_folder->setText(path_to_dir);
    QFile file(QDir::homePath() + "/VersionWrapper/Preferences/pref.txt");
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream out(&file);
    out << path_to_dir;
    file.close();
}

//preferences menu (set upload URL) triggered
void VersionWrapper::OnActionSetUrl() {

    SetUrlDialog* urldlg = new SetUrlDialog;
    connect(urldlg, SIGNAL(sendPathString(QString)), SLOT(OnUrlSignal(QString)));
    urldlg->exec();

}
void VersionWrapper::OnActionSeparatorString() {
    
    Separator_dialog* sep_dial = new Separator_dialog;
    connect(sep_dial, SIGNAL(SeparatorStringSignal(QString)), this, SLOT(OnSeparatorStringSignal(QString)));
    sep_dial->exec();
}

//preferences menu (about) triggered
void VersionWrapper::OnActionAbout() {

    QMessageBox info;
    info.setText("this piece of junk was written by the mighty lord deisen");
    info.exec();
}

void VersionWrapper::OnActionServer() {

    ServerCredentialsDialog* serverdlg = new ServerCredentialsDialog;
    connect(serverdlg, SIGNAL(ServerCredentialsStrings(QString, QString)), this, SLOT(onCredentialsSignal(QString, QString)));
    serverdlg->exec();
}

//not needed...
QString VersionWrapper::backslashreplacer(QString input) {
    
    for (size_t i = 0; i < input.size(); ++i) {
        if (input.at(i) == '\\')
            input.replace(i, 1, '/');
    }
    return input;
}

//get URL string from SetUrlDialog's line edit user input
void VersionWrapper::OnUrlSignal(QString _url) {

    if (!_url.isEmpty()) {  //nothing changes on idle input
        QFile file(QDir::homePath() + "/VersionWrapper/Preferences/url.txt");
        file.open(QIODevice::WriteOnly | QIODevice::Text);
        QTextStream out(&file);
        //save new URL to preferences
        out << _url;
        file.close();
        //update member variable
        url = _url;
        //update GUI
        ui->url_label->setText(_url);
    }
}

void VersionWrapper::onCredentialsSignal(QString _user, QString _password) {

    if (!_user.isEmpty() || _password.isEmpty()) {  //nothing changes on idle input
        QFile file(QDir::homePath() + "/VersionWrapper/Preferences/credentials.txt");
        file.open(QIODevice::WriteOnly | QIODevice::Text);
        QTextStream out(&file);
        //save new URL to preferences
        out << _user << '\n';
        out << _password;
        file.close();
        //update member variable
        user_name = _user;
        password = _password;
        //update gui
        ui->label_server_credentials->setText("user: " + user_name + "  password: " + password);
    }
}

//forward status strings from networker class
void VersionWrapper::OnNetworkStatusSignal(QString status) {

    ui->status_label->setText(status);

}

void VersionWrapper::OnSeparatorStringSignal(QString _separator_string) {
    if (!_separator_string.isEmpty()) {  //nothing changes on idle input
        QFile file(QDir::homePath() + "/VersionWrapper/Preferences/separator.txt");
        file.open(QIODevice::WriteOnly | QIODevice::Text);
        QTextStream out(&file);
        //save new URL to preferences
        out << _separator_string;
        file.close();
        //update member variable
        separator_string = _separator_string;

        ui->label_separator->setText(separator_string);
    }
}

