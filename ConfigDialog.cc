#include "ConfigDialog.h"
#include "ui_ConfigDialog.h"
#include <QMessageBox>
#include <QPropertyAnimation>
#include <QFileDialog>

static void shake(QWidget *obj) {
    QPropertyAnimation *pAnimation = new QPropertyAnimation(obj, "pos");
    pAnimation->setDuration(300);
    pAnimation->setLoopCount(1);
    auto y = obj->geometry().y();
    pAnimation->setKeyValueAt(0, QPoint(obj->geometry().x() - 2, y));
    pAnimation->setKeyValueAt(0.1, QPoint(obj->geometry().x() + 4, y));
    pAnimation->setKeyValueAt(0.2, QPoint(obj->geometry().x() - 4, y));
    pAnimation->setKeyValueAt(0.3, QPoint(obj->geometry().x() + 4, y));
    pAnimation->setKeyValueAt(0.4, QPoint(obj->geometry().x() - 4, y));
    pAnimation->setKeyValueAt(0.5, QPoint(obj->geometry().x() + 4, y));
    pAnimation->setKeyValueAt(0.6, QPoint(obj->geometry().x() - 4, y));
    pAnimation->setKeyValueAt(0.7, QPoint(obj->geometry().x() + 4, y));
    pAnimation->setKeyValueAt(0.8, QPoint(obj->geometry().x() - 4, y));
    pAnimation->setKeyValueAt(0.9, QPoint(obj->geometry().x() + 4, y));
    pAnimation->setKeyValueAt(1, QPoint(obj->geometry().x() - 2, y));
    pAnimation->start(QAbstractAnimation::DeleteWhenStopped);
}

ConfigDialog::ConfigDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConfigDialog)
{
    ui->setupUi(this);
    this->setWindowTitle(QString("服务器配置"));
}

ConfigDialog::~ConfigDialog()
{
    delete ui;
}

void ConfigDialog::on_saveButton_clicked()
{
    QString ipStr = ui->ipLineEdit->text();
    QHostAddress addr = QHostAddress(ipStr);
    if (ipStr == "") {
        addr = QHostAddress::Any;
    }
    if (addr.isNull()) {
        QMessageBox::critical(this, "输入错误", "IP地址格式不正确");
        shake(ui->ipLineEdit);
        return;
    }
    QString portStr = ui->portLineEdit->text();
    bool succ;
    int port = portStr.toInt(&succ);
    if (!succ || port < 0 || port > 65535) {
        QMessageBox::critical(this, "输入错误", "端口应为0~65535之间的整数");
        shake(ui->portLineEdit);
        return;
    }
    if (this->homeDir == "") {
        QMessageBox::critical(this, "输入错误", "请选择主目录");
        return;
    }
    emit configUpdated(addr, port, this->homeDir);
    this->close();
}

void ConfigDialog::initialize(QString host, QString port, QString homeDir) {
    ui->ipLineEdit->setText(host);
    ui->portLineEdit->setText(port);
    ui->currentHomedir->setText(homeDir);
    this->homeDir = homeDir;
}

void ConfigDialog::on_homedirPickButton_clicked()
{
    auto path = QFileDialog::getExistingDirectory(this, "请选择主目录路径", ui->currentHomedir->text());
    if (path != "") {
        this->homeDir = path;
        ui->currentHomedir->setText(path);
    }
}
