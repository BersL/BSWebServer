#include "ResponseManager.h"
#include <QTextStream>

namespace BS {

static const char * kIfModifiedSince = "If-Modified-Since";

ResponseManager::ResponseManager(QObject *parent) : QObject(parent)
{

}

Response *ResponseManager::makeFileResponse(const QString &path, const QMap < QString, QString > &reqHeaders, unsigned maxAge) {
    unsigned rangestart = 0;
    unsigned rangeend = 0;
    QString range = reqHeaders.count("Range") ? reqHeaders["Range"] : "";
    if (range.indexOf('=') != -1) {
        QString bytesRange = range.section('=', -1);
        if (range.indexOf('-') != -1) {
            QString start = bytesRange.section('-', 0, 0);
            QString end = bytesRange.section('-', 1, 1);
            bool ok;
            rangestart = start.toUInt(&ok);
            if (!ok) rangestart = 0;
            rangeend = end.toUInt(&ok);
            if (!ok) rangestart = 0;
        }
    }
    if (rangeend < rangestart) {
        auto response = new ErrorResponse(StatusCode::BadRequest, QString("Bad Request"), QString("Range header error. Request file cannot < 0.").toHtmlEscaped());
        return response;
    }
    QString errorString;
    QString ifModifiedSince = reqHeaders.count(kIfModifiedSince) ? reqHeaders[kIfModifiedSince] : "";
    auto response = new FileResponse(path, StatusCode::OK, errorString, ifModifiedSince, rangestart, rangeend, maxAge);
    if (errorString != "") {
        emit error(errorString);
        return new ErrorResponse(StatusCode::ServiceUnavailable, QString("Error 503"), errorString);
    }
    return response;
}

DataResponse *ResponseManager::makeJSONResponse(const QByteArray *jsonObject) {
    return new DataResponse(contentTypeWithFileType(FileType::JSON), StatusCode::OK, jsonObject);
}

DataResponse *ResponseManager::makeTextResponse(const QString &text) {
    auto bytes = new QByteArray(text.toUtf8());
    auto response = new DataResponse(contentTypeWithFileType(FileType::TXT), StatusCode::OK, bytes);
    delete bytes;
    return response;
}

ErrorResponse *ResponseManager::makeURLNotFoundResponse(const QString &URL) {
    return new ErrorResponse(StatusCode::NotFound, QString("URL Not Found"), QString("The requested URL %1 was not found on this server.").arg(URL));
}

ErrorResponse *ResponseManager::makeInternalErrorResponse(const QString &msg) {
    return new ErrorResponse(StatusCode::InternalServerError, QString("Internal Server Error"), msg);
}

Response *ResponseManager::makeResponse() {
    return new Response(contentTypeWithFileType(FileType::TXT), StatusCode::OK);
}

}

