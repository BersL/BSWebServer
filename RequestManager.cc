#include "RequestManager.h"
#include <QVector>
#include <qDebug.h>

namespace BS {

RequestManager::RequestManager(QObject *parent) : QObject(parent)
{

}

Request *RequestManager::makeRequest(const QByteArray &req) {
    QStringList reqLines = QString(req).split("\r\n");
    if (reqLines.size() == 0) return nullptr;
    auto parts = reqLines[0].split(' ');
    if (parts.size() < 3) return nullptr;

    QString method = parts[0];
    QString origionURL = QUrl::fromPercentEncoding(parts[1].toLatin1());
    QString queryString;
    auto url = QUrl(origionURL);
    if (url.hasQuery())
        queryString = url.query();
    auto requestURL = url.path();
    QString version = parts[2];

    if (requestURL.endsWith("/") ) {
        requestURL += "index.html";
    }

    auto request = new Request(method, requestURL, queryString, version);

    auto reqBodyIter = reqLines.begin() + 1;

    for (auto it = reqLines.begin() + 1; it != reqLines.end(); ++it) {
        auto pos = it->indexOf(':');
        if (pos != -1) {
            QString key = it->left(pos);
            QString value = it->right(it->length() - pos - 1);
            request->setHeader(key, value);
        } else {
            reqBodyIter = it;
            break;
        }
    }
    for (auto it = reqBodyIter + 1; it != reqLines.end(); ++it) {
        request->body += *it;
    }
    return request;
}

}

