#ifndef RESPONSEMANAGER_H
#define RESPONSEMANAGER_H

#include <QObject>
#include <ErrorResponse.h>
#include <FileResponse.h>
#include <QMap>

namespace BS {

extern const char *htmlContentType;
extern const char *plainContentType;
extern const char *jsonContentType;

class ResponseManager : public QObject
{
    Q_OBJECT

public:
    explicit ResponseManager(QObject *parent = 0);
    DataResponse *makeJSONResponse(const QByteArray *jsonObject);
    DataResponse *makeTextResponse(const QString &text);
    Response *makeFileResponse(const QString &path, const QMap < QString, QString > &reqHeaders, unsigned maxAge = 120); // may return ErrorResponse
    ErrorResponse *makeURLNotFoundResponse(const QString &URL);
    ErrorResponse *makeInternalErrorResponse(const QString &msg);
    Response *makeResponse();

signals:
    void error(QString info);

public slots:
};


}


#endif // RESPONSEMANAGER_H
