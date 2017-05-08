#include <CommonUtils.h>

QString hostAddresstoString(const QHostAddress &addr) {
    QString peerAddr = addr.toString();
    return peerAddr.right(peerAddr.length() - peerAddr.lastIndexOf(":") - 1);
}

QString &removeFirstSlash(QString &path) {
    if (path[0] == '/')
        path.remove(0, 1);
    return path;
}


QString getGMTTime(const QDateTime &time) {
    QString format = QString("ddd, dd MMM yyyy HH:mm:ss"); // RFC1123
    QLocale loc = QLocale(QLocale::English, QLocale::UnitedStates);
    return loc.toString(time.toUTC(), format) + " GMT";
}
