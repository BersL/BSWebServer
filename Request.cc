#include "Request.h"

namespace BS {

Request::Request(const QString &method, const QString &requestUrl, const QString &queryString, const QString &version) :
    method(Method::UNSUPPORTED), requestUrl(requestUrl), queryString(queryString), version(version)
{
    if (method.toUpper() == "GET") {
        this->method = Method::GET;
    } else if (method.toUpper() == "POST") {
        this->method = Method::POST;
    }
}

QString Request::methodStr() const {
    if (this->method == Method::GET) {
        return "GET";
    } else if (this->method == Method::POST) {
        return "POST";
    }
    return "Unsupported Method";
}

QString Request::getHeader(const QString &key) const {
    if (this->headers.count(key))
        return this->headers[key];
    else
        return "";
}

QMap < QString, QString > Request::getAllHeaders() const {
    return this->headers;
//    QStringList headerlist;
//    for (auto it = this->headers.begin(); it != this->headers.end(); ++ it) {
//        headerlist.append(QString("%1:%2").arg(it.key()).arg(it.value()));
//    }
//    return headerlist;
}

void Request::setHeader(const QString &key, const QString value) {
    this->headers[key] = value.trimmed();
}


}
