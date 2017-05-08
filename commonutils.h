#ifndef COMMONUTILS_H
#define COMMONUTILS_H

#include <QHostAddress>
#include <QString>
#include <QDateTIme>

QString hostAddresstoString(const QHostAddress &addr);

QString& removeFirstSlash(QString &path);

QString getGMTTime(const QDateTime &time);

#endif // COMMONUTILS_H
