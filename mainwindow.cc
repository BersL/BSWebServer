#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <ConfigDialog.h>
#include <QMessageBox>
#include <unistd.h>
#include <QDir>
#include <ConnectionLogger.h>
#include <QDateTime>
#include <CommonUtils.h>
#include <QFileDialog>

static const char *ui_red = "#CD0000";
static const char *ui_green = "#00CD00";
static const char *ui_yellow = "#CDCD00";

static QString coloredText(QString text, QString hexColor) {
    return QString("<span style=\"color:%1\">%2</span>").arg(hexColor).arg(text);
}

static QString boldedText(QString text) {
    return QString("<strong>%1</strong>").arg(text);
}


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    serverState(false)
{
    ui->setupUi(this);
    this->setWindowTitle(QString("BSWebServer"));
    ui->statusBar->showMessage("Server not started.");
    this->indicator = new StateIndicator(this);
    this->indicator->setMaximumWidth(21);
    this->indicator->setMaximumHeight(21);
    ui->statusBar->setMinimumHeight(26);
    ui->statusBar->addPermanentWidget(this->indicator, 1);

    this->addr = QHostAddress::Any;
    this->port = "80";
    this->homeDirPath = QDir::homePath();

    connect(BS::ConnectionLogger::sharedLogger(), SIGNAL(connected(QString)),
            this, SLOT(log_connect(QString)));
    connect(BS::ConnectionLogger::sharedLogger(), SIGNAL(disconnected(QString)),
            this, SLOT(log_disconnected(QString)));
    connect(BS::ConnectionLogger::sharedLogger(), SIGNAL(incoming_request(QString, QString, QString)),
            this, SLOT(log_request(QString, QString, QString)));
    connect(BS::ConnectionLogger::sharedLogger(), SIGNAL(sent_response(QString, QString, QString)),
            this, SLOT(log_response(QString, QString, QString)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::log_connect(QString host) {
    this->append_log(QString("%1%2").arg("Connected to: ").arg(host));
}

void MainWindow::log_disconnected(QString host) {
    this->append_log(QString("%1%2").arg("Disconnected to: ").arg(host));
}

void MainWindow::log_request(QString method, QString host, QString url) {
    this->append_log(QString("%1 %2 request from %3 for \"%4\"").arg(coloredText("Received", ui_yellow)).arg(method).
                                                                 arg(host).arg(url));
}

void MainWindow::log_response(QString method, QString host, QString briefContent) {
    this->append_log(QString("%1 %2 request from %3 with <span>%4</span>").arg(coloredText("Response", ui_green)).arg(method).
                                                              arg(host).arg(briefContent));
}

void MainWindow::append_log(QString msg) {
    auto currentTime =  QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    auto log = QString("%1 %2").arg(currentTime).arg(msg);
    ui->logTextEdit->append(log);
}

void MainWindow::on_actionConfig_triggered()
{
    auto dialog = new ConfigDialog(this);
    dialog->initialize(addr.toString(), port, this->homeDirPath);
    connect(dialog, &ConfigDialog::configUpdated, [&](QHostAddress addr, unsigned short port, QString homedir) {
        this->addr = addr;
        this->port = QString::number(port);
        this->homeDirPath = homedir;
    });
    dialog->show();
}

void MainWindow::on_actionServer_triggered()
{
    if (!this->serverState) {
        if (port.toShort() < 1024 && getuid() != 0) {
            QMessageBox::critical(this, "无法运行", "端口号小于1024，请以root用户运行本程序");
            return;
        }
        if (this->server->startServer(port.toShort(), addr, this->homeDirPath) != 0) {
            ui->statusBar->showMessage(QString("Unable to listen on %1:%2, %3").arg(addr.toString()).arg(port.toInt()).arg(this->server->errorString()));
            return;
        }
        auto listenaddr = hostAddresstoString(addr);
        auto listenport = port.toUShort();
        ui->statusBar->showMessage(QString("Listening on %1:%2 ...").arg(listenaddr).arg(listenport));
        this->append_log(QString("Server start listening on %1:%2").arg(listenaddr).arg(listenport));

        ui->actionStop->setEnabled(true);
        ui->actionServer->setEnabled(false);
        indicator->setIndicatorColor(QColor(ui_green));
        this->serverState = true;
    }

}

void MainWindow::on_actionStop_triggered()
{
    if (this->serverState) {
        this->server->stopServer();
        ui->statusBar->showMessage("Server not started.");
        this->append_log("Server stopped listening");
        ui->actionServer->setEnabled(true);
        ui->actionStop->setEnabled(false);
        indicator->setIndicatorColor(QColor(ui_red));
        this->serverState = false;
    }
}

void MainWindow::on_actionClear_triggered()
{
    auto res = QMessageBox::question(this, "清除确认", "清除所有log信息？");
    if (res == QMessageBox::Yes)
        ui->logTextEdit->clear();
}

void MainWindow::on_actionExport_triggered()
{
    QString log = ui->logTextEdit->toPlainText();
    QString defaultName = QDateTime::currentDateTime().toString("yyyy-MM-dd-hh-mm-ss");
    auto fileName = QFileDialog::getSaveFileName(this, "导出到", QString("%1/%2.log").arg(this->homeDirPath).arg(defaultName), tr("Log files (*.log)"));
    if (!fileName.length()) return;
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "导出失败", QString("无法打开文件：%1").arg(fileName));
        return;
    }
    QTextStream out(&file);
    out << log;
    file.close();
    QMessageBox::information(this, "导出成功", QString("已保存至：%2").arg(fileName));
}
