#ifndef ERRORRESPONSE_H
#define ERRORRESPONSE_H

#include <DataResponse.h>

namespace BS {

class ErrorResponse : public DataResponse
{
public:
    ErrorResponse(StatusCode code, const QString &message);
    ErrorResponse(StatusCode code, const QString &title, const QString &message);

    QString brief() const override;

private:
    QString errorMessage;
};

}



#endif // ERRORRESPONSE_H
