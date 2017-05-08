#include "FileResponse.h"
#include <string.h>
#include <QFileInfo>
#include <CommonUtils.h>
#include <string.h>
#include <magic.h>

#define MIN(A, B) ((A) < (B) ? (A) : (B))

namespace BS {

static const char *kLastModified = "Last-Modified";
static const char *kContentRange = "Content-Range";
static const char *kCacheControl = "Cache-Control";
static const char *kAcceptRanges = "Accept-Ranges";

static const char *content_types [] = {
    "text/html; charset=utf-8", "text/plain; charset=utf-8", "text/css; charset=utf-8",
    "application/javascript", "application/json", "application/pdf", "application/xml",
    "image/jpeg", "image/png", "image/gif", "image/tiff", "image/x-icon",
    "audio/mp3", "video/mp4"
};

const char *contentTypeWithFileType(FileType type) {
    return content_types[static_cast<int>(type)];
}

FileType fileTypeWithExtName(const char *type) {
    if (!strcmp(type, "htm") || !strcmp(type, "html"))  return FileType::HTML;
    if (!strcmp(type, "txt"))                           return FileType::TXT;
    if (!strcmp(type, "css"))                           return FileType::CSS;
    if (!strcmp(type, "js"))                            return FileType::JS;
    if (!strcmp(type, "json"))                          return FileType::JSON;
    if (!strcmp(type, "pdf"))                           return FileType::PDF;
    if (!strcmp(type, "xml"))                           return FileType::XML;
    if (!strcmp(type, "jpg") || !strcmp(type, "jpeg"))  return FileType::JPEG;
    if (!strcmp(type, "png"))                           return FileType::PNG;
    if (!strcmp(type, "gif"))                           return FileType::GIF;
    if (!strcmp(type, "tif")|| !strcmp(type, "tiff"))   return FileType::TIF;
    if (!strcmp(type, "ico"))                           return FileType::ICO;
    if (!strcmp(type, "mp3"))                           return FileType::MP3;
    if (!strcmp(type, "mp4"))                           return FileType::MP4;
    else                                                return FileType::TXT;

}

FileResponse::FileResponse(QString path, StatusCode status, QString &errorString, QString ifModifiedSince, const unsigned rangestart, const unsigned rangeend, unsigned maxAge) :
    DataResponse(contentTypeWithFileType(FileType::TXT), status), filePath(path)
{
    QFileInfo fileInfo(path);
    if (fileInfo.isDir()) {
        path += "/index.html";
        fileInfo = QFileInfo(path);
        this->filePath = path;
    }
    FileType type = fileTypeWithExtName(fileInfo.suffix().toLatin1());
    this->fileType = type;
    this->contentType = contentTypeWithFileType(type);
    QString lastModifiedData = getGMTTime(fileInfo.lastModified());
    if (lastModifiedData.toLower() == ifModifiedSince.toLower()) {
        this->statusCode = StatusCode::NotModified;
        return;
    }
    this->setHeader(kLastModified, getGMTTime(fileInfo.lastModified()));
    this->setHeader(kCacheControl, maxAge ? QString("max-age=%1").arg(maxAge) : "no-cache");
    this->setHeader(kAcceptRanges, "bytes");

    auto file = new QFile(path);
    if (!file->open(QIODevice::ReadOnly)) {
        errorString = QString("Open file %1 for error: %2").arg(path).arg(file->errorString());
    } else {
        file->seek(rangestart);
        if (rangeend && rangeend > rangestart) {
            QByteArray data = file->read(rangeend - rangestart + 1);
            this->setContent(data);
            auto contentRange = QString("bytes %1-%2/%3").arg(rangestart).arg(MIN(rangeend, file->size())).arg(file->size());
            this->setHeader(kContentRange, contentRange);
            this->statusCode = StatusCode::PartialContent;
        } else {
            QByteArray data = file->readAll();
            if (QString(contentTypeWithFileType(type)).section('/', 0, 0) == "text") {
                QString dataStr(data);
                data = dataStr.toUtf8();
            }
            this->setContent(data);
        }
        file->close();
    }
    delete file;
}

QString FileResponse::getFilePath() {
    return this->filePath;
}

QString FileResponse::brief() const {
    if (this->statusCode == StatusCode::OK)
        return QString("file <a href=\"%1\">%2</a>").arg(this->filePath).arg(this->filePath);
    else if (this->statusCode == StatusCode::NotModified)
        return QString("status code 304, file not modified.");
    else if (this->statusCode == StatusCode::PartialContent)
        return QString("status code 206, %1 bytes of file <a href=\"%2\">%3</a>").arg(this->contentLength).arg(this->filePath).arg(this->filePath);
    else
        return DataResponse::brief();
}

}
