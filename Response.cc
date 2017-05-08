#include "Response.h"
#include <QTextStream>
#include <QMap>
#include <CommonUtils.h>

static const char *kContent_type = "Content-Type";
static const char *kContent_length = "Content-Length";
static const char *kConnection = "Connection";
static const char *kDate = "Date";

static const char *statusCodeDescriptions [] = {
    "Continue",
    "SwitchingProtocols",
    "Processing",

    "OK",
    "Created",
    "Accepted",
    "NonAuthoritativeInformation",
    "NoContent",
    "ResetContent",
    "PartialContent",
    "MultiStatus",
    "AlreadyReported",

    "MultipleChoices",
    "MovedPermanently",
    "Found",
    "SeeOther",
    "NotModified",
    "UseProxy",
    "TemporaryRedirect",
    "PermanentRedirect",

    "BadRequest",
    "Unauthorized",
    "PaymentRequired",
    "Forbidden",
    "NotFound",
    "MethodNotAllowed",
    "NotAcceptable",
    "ProxyAuthenticationRequired",
    "RequestTimeout",

    "InternalServerError",
    "NotImplemented",
    "BadGateway",
    "ServiceUnavailable",
    "GatewayTimeout",
};

static const int prevStatusCodeGroupCount[] = {
    0, 3, 9, 9, 9
};

namespace BS {

const char * descriptionFromStatusCode(StatusCode code) {
    int codeVal = static_cast<int>(code);
    if (codeVal < 100 || codeVal > 504) return "";
    int group = codeVal / 100 - 1;
    int index = codeVal % 100;
    return statusCodeDescriptions[prevStatusCodeGroupCount[group] + index];
}

Response::Response(const QString &contentType, StatusCode statusCode) : contentLength(0), contentType(contentType), statusCode(statusCode)
{

}

Response::~Response() {

}

unsigned long Response::getContentLength() const {
    return this->contentLength;
}

QString Response::currentGMTTime() const {
   return getGMTTime(QDateTime::currentDateTime());
}

void Response::setHeader(QString key, QString value) {
    if (key == kContent_type) return;
    if (key == kContent_length) return;
    this->additionalHeaders[key] = value;
}

QString Response::header() const {
    auto streamStr = new QString;
    QTextStream sstream(streamStr);

    sstream << "HTTP/1.1 " << static_cast<int>(this->statusCode) << " " << descriptionFromStatusCode(this->statusCode) << "\r\n"
            << kContent_type << ": " << this->contentType << "\r\n"
            << kDate << ": " << this->currentGMTTime() << "\r\n"
            << kContent_length << ": " << this->contentLength << "\r\n"
            << kConnection << ": Keep-Alive" << "\r\n";

    for (auto it = this->additionalHeaders.begin(); it != this->additionalHeaders.end(); ++it) {
        sstream << it.key() << ": " << it.value() << "\r\n";
    }
    sstream << "\r\n";

    auto headerStr = sstream.readAll();
    delete streamStr;

    return headerStr;
}

QByteArray Response::getResponseData() const {
    return this->header().toUtf8();
}

QString Response::brief() const {
    return QString("status code: %1").arg(static_cast<int>(this->statusCode));
}

}

