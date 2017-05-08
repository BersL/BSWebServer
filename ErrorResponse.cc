#include "ErrorResponse.h"

static const char *htmlContentType = "text/html; charset=utf-8";

namespace BS {

ErrorResponse::ErrorResponse(StatusCode code, const QString &message):
    ErrorResponse(code, QString("An Error Occured For Code %1").arg(static_cast<int>(code)) ,message)
{

}

ErrorResponse::ErrorResponse(StatusCode code, const QString &title, const QString &message):
    DataResponse(QString(htmlContentType), code), errorMessage(message)
{
    QString HTMLTemplate("<!DOCTYPE html><html lang=\"en\"><head><meta charset=\"utf-8\"><title>Error %1</title></head><body style=\"padding-top: 100px; text-align: center; \"><h1>%2</h1><p>%3</p></body></html>");
    this->setContent(HTMLTemplate.arg(static_cast<int>(code)).arg(title).arg(message).toUtf8());
}

QString ErrorResponse::brief() const {
    return QString("error: %1").arg(this->errorMessage);
}

}
