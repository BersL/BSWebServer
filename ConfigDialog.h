#ifndef CONFIGDIALOG_H
#define CONFIGDIALOG_H

#include <QDialog>
#include <QHostAddress>
#include <QString>

namespace Ui {
class ConfigDialog;
}

class ConfigDialog : public QDialog
{
    Q_OBJECT
    QString homeDir;

public:
    explicit ConfigDialog(QWidget *parent = 0);
    ~ConfigDialog();
    void initialize(QString host, QString port, QString homeDir);

signals:
    void configUpdated(QHostAddress addr, unsigned short port, QString homeDir);

private slots:

    void on_saveButton_clicked();

    void on_homedirPickButton_clicked();

private:
    Ui::ConfigDialog *ui;
};

#endif // CONFIGDIALOG_H
