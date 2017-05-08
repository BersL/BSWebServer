#include "DataResponse.h"
#include <QTextStream>

namespace BS {


DataResponse::DataResponse(const QString &contentType, StatusCode statusCode, const QByteArray *data):
    Response(contentType, statusCode) {
    if (data != nullptr) {
        this->setContent(*data);
    }
}

QByteArray DataResponse::getResponseData() const {
    auto response = this->Response::getResponseData();
    response.append(this->content).append("\n");
    return response;
}

void DataResponse::setContent(const QByteArray &content) {
    this->content = content;
    this->contentLength = content.length();
}

QString DataResponse::brief() const {
    return QString("content of %1 bytes").arg(this->contentLength);
}

}
