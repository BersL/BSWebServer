#ifndef DATARESPONSE_H
#define DATARESPONSE_H

#include <Response.h>
#include <QFile>
#include <QByteArray>

namespace BS {

class DataResponse : public Response
{

protected:
    QByteArray content;

public:
    explicit DataResponse(const QString &contentType, StatusCode statusCode, const QByteArray *data = nullptr);

    QByteArray getResponseData() const override;

    void setContent(const QByteArray &content);

    QString brief() const override;
};

}

#endif // DATARESPONSE_H
