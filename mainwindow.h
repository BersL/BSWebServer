#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <WebServerCore.h>
#include <StateIndicator.h>

namespace BS {

class Request;
class Response;

}

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    BS::WebServerCore *server;

    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void log_connect(QString host);

    void log_disconnected(QString host);

    void log_request(QString method, QString host, QString url);

    void log_response(QString method, QString host, QString briefContent);

    void on_actionConfig_triggered();

    void on_actionServer_triggered();

    void on_actionStop_triggered();

    void on_actionClear_triggered();

    void on_actionExport_triggered();

private:
    Ui::MainWindow *ui;
    StateIndicator *indicator;
    bool serverState;

    QHostAddress addr;
    QString port;
    QString homeDirPath;

    void append_log(QString msg);

};

#endif // MAINWINDOW_H
