#ifndef RESPONSE_H
#define RESPONSE_H

#include <QString>
#include <StatusCode.h>
#include <QMap>
#include <QByteArray>

namespace BS {


class Response
{
protected:
    unsigned long contentLength;

public:
    QString contentType;
    StatusCode statusCode;

    unsigned long getContentLength() const;

    Response(const QString &contentType, StatusCode statusCode);
    virtual ~Response();

    void setHeader(QString key, QString value);
    QString header() const;
    virtual QByteArray getResponseData() const;

    virtual QString brief() const;


private:
    QMap < QString, QString > additionalHeaders;
    QString currentGMTTime() const;

};

}


#endif // RESPONSE_H
