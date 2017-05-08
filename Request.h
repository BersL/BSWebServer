#ifndef REQUEST_H
#define REQUEST_H

#include <QString>
#include <QMap>
#include <QStringList>
#include <QUrl>

namespace BS {

enum class Method {
    GET,
    POST,
    UNSUPPORTED
};

class Request
{
public:
    Method method; // Only support "GET" and "POST"
    QString requestUrl;
    QString queryString;
    QString version;

    QString body;

    QString methodStr() const;

    Request(const QString &method, const QString &requestUrl, const QString &queryString, const QString &version);

    QString getHeader(const QString &key) const;
    QMap < QString, QString > getAllHeaders() const;
    void setHeader(const QString &key, const QString value);

private:
    QMap < QString, QString > headers;
};


}


#endif // REQUEST_H
