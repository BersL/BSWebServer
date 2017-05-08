#include "IncomingConnection.h"
#include "ConnectionLogger.h"
#include <QHostAddress>
#include <QFileInfo>
#include <CommonUtils.h>


namespace BS {

IncomingConnection::IncomingConnection(QTcpSocket *socket, RequestManager *req_manager, ResponseManager *res_manager, QString homeDir, QObject *parent) :
    QObject(parent), socket(socket), req_manager(req_manager), res_manager(res_manager), homeDirPath(homeDir)
{
    connect(socket, SIGNAL(readyRead()), this, SLOT(readData()));
    connect(this, SIGNAL(incoming_request(QString, QString, QString)),
            ConnectionLogger::sharedLogger(), SLOT(socket_incoming_request(QString, QString, QString)));
    connect(this, SIGNAL(sent_response(QString, QString, QString)),
            ConnectionLogger::sharedLogger(), SLOT(socket_sent_response(QString, QString, QString)));

}

IncomingConnection::~IncomingConnection() {
    if (socket) delete socket;
}

void IncomingConnection::readData() {
//    if ( socket->bytesAvailable() == 0 )
//          return;
    auto addr = hostAddresstoString(socket->peerAddress());
    auto port = socket->peerPort();
    auto data = socket->readAll();
    auto request = this->req_manager->makeRequest(data);

    if (request) {
        auto method = request->methodStr();
        emit incoming_request(method, QString("%1:%2").arg(addr).arg(port), request->requestUrl);
        auto response = this->makeResponseFromRequest(request);
        if (response) {
            auto brief = response->brief();
            socket->write(response->getResponseData());
            socket->flush();
            emit sent_response(method, QString("%1:%2").arg(addr).arg(port), brief);
        }
        delete response;
        delete request;
    }
}

Response *IncomingConnection::makeResponseFromRequest(const Request *req) {
    QString reqUrl = this->homeDirPath + req->requestUrl;
    if (req->method == Method::GET) {
        QFileInfo info(reqUrl);
        if (! info.exists()) {
            return this->res_manager->makeURLNotFoundResponse(req->requestUrl);
        }
        return this->res_manager->makeFileResponse(reqUrl, req->getAllHeaders());
    } else if (req->method == Method::POST) {
        return this->res_manager->makeInternalErrorResponse("The request method is not supported by this server currently.");
    }
    return this->res_manager->makeInternalErrorResponse("The request method is not supported by this server currently.");
}

}

