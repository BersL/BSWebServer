#ifndef REQUESTMANAGER_H
#define REQUESTMANAGER_H

#include <QObject>
#include <Request.h>

namespace BS {

class RequestManager : public QObject
{
    Q_OBJECT
public:
    explicit RequestManager(QObject *parent = 0);

    Request *makeRequest(const QByteArray &req);

signals:

public slots:
};

}



#endif // REQUESTMANAGER_H
