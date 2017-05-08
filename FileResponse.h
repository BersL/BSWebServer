#ifndef FILERESPONSE_H
#define FILERESPONSE_H

#include <DataResponse.h>

namespace BS {

enum class FileType : unsigned {
    HTML = 0, TXT, CSS,
    JS, JSON, PDF, XML,
    JPEG, PNG, GIF, TIF, ICO,
    MP3, MP4
};

FileType fileTypeWithExtName(const char*);

const char *contentTypeWithFileType(FileType type);

class FileResponse : public DataResponse
{
public:
    FileType fileType;
    FileResponse(QString filePath, StatusCode status, QString &errorString, QString ifModifiedSince = "", const unsigned rangestart = -1, const unsigned rangeend = -1, unsigned maxAge = 120);

    QString getFilePath();

    QString brief() const override;
private:
    QString filePath;
};

}

#endif // FILERESPONSE_H
